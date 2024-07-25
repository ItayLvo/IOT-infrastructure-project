#define _XOPEN_SOURCE 700		/* sigaction struct */


#include <stddef.h>		/* size_t */
#include <assert.h>		/* assert */
#include <stdio.h>		/* printf */
#include <sys/types.h>	/* pid_t */
#include <unistd.h>		/* waitpid, fork, write */
#include <sys/wait.h>	/* waitpid */
#include <signal.h>		/* sigaction */
#include <fcntl.h>		/* O_* constants (O_CREAT)*/
#include <semaphore.h>	/* POSIX semaphore functions and definitions */
#include <pthread.h>	/* POSIX pthreads */
#include <stdatomic.h>	/* atomic types */
#include <stdlib.h>		/* exit, strtoul, setenv */
#include <string.h>		/* sprintf , snprintf */

#include "watch_dog.h"
#include "scheduler.h"
#include "uid.h"

#define SEMAPHORE_NAME "/wd_sem"
#define MMI_ACTIVE 1
#define MMI_DISABLED 0
#define WD_ALIVE 1
#define WD_DEAD 0
#define DECIMAL_BASE 10


/* global static variables */
static volatile atomic_int repetition_counter = ATOMIC_VAR_INIT(0);
static pid_t g_user_pid = 0;
static atomic_int g_mmi_active = MMI_DISABLED;
static int g_is_wd_alive = WD_DEAD;		/* is this needed? because also using repetition_counter... */
static sem_t *process_sem;
static scheduler_t *scheduler;
static char *user_exec_path;
static size_t max_repetitions;
static size_t interval;


/* forward function declerations */
static void InitSignalHandlers(void);
static int InitScheduler(void);
static int InitEnvVar(void);

static void SignalHandleReceivedLifeSign(int signum);
static void SignalHandleReceivedDNR(int signum);

static int SchedulerActionIncreaseCounter(void *param);
static int SchedulerActionSendSignal(void *param);

static int SchedulerActionReviveClient(void *param);	/* make this non-scheduler func? */
static int CreateClientProcess(void);


int main(int argc, char *argv[])
{
	user_exec_path = argv[1];
	g_user_pid = getppid();
	/* convert argv arguments back to size_t */
	interval = strtoul(argv[2], NULL, DECIMAL_BASE);  
    max_repetitions = strtoul(argv[3], NULL, DECIMAL_BASE);
    
	InitSignalHandlers();
	/* set environment variable to indicate WD process is running */
	InitEnvVar();
	
	if (InitScheduler() != 0)
	{
		exit(1);
	}
	
	
	/* open the existing IPC semaphore and post to it, notifying client process that WD is ready */
    process_sem = sem_open(SEMAPHORE_NAME, 0);
    if (process_sem == SEM_FAILED)
    {
        perror("sem_open failed\n");
		exit(EXIT_FAILURE);
    }
    printf("Loading WD process. Data:\ninterval = %lu\nmax rep = %lu\nclient pid = %u\nuser exec path = %s\n\n", interval, max_repetitions, g_user_pid, user_exec_path);
    printf("WD process, main func, before posting to semaphore\n");
	sem_post(process_sem);
	printf("WD process, main func, after posting to semaphore. starting scheduler\n");
	/* run scheduler */
	SchedulerRun(scheduler);
	
	printf("WD process, after ShedulerRun...?\n");
	
	/* destroy scheduler - will only reach this part after receiving SIGUSR2 (DNR) */
	SchedulerDestroy(scheduler);
	
	(void)argc;
	return 0;
}



static int InitEnvVar(void)
{
	pid_t wd_pid = getpid();
	char string_pid[36];
	
	sprintf(string_pid, "%d", wd_pid);
	/* set "WD_RUNNING" environment variable to the WD PID */
    setenv("WD_RUNNING", string_pid, 1);
    
    return 0;
}


static int InitScheduler(void)
{
	ilrd_uid_t task_signal_life_sign = {0};
	ilrd_uid_t task_watchdog_tick = {0};
	
	scheduler = SchedulerCreate();
	if (scheduler == NULL)
	{
		return 1;
	}
	
	task_signal_life_sign = SchedulerAddTask(scheduler, SchedulerActionSendSignal, NULL, NULL, interval);
	task_watchdog_tick = SchedulerAddTask(scheduler, SchedulerActionIncreaseCounter, NULL, NULL, interval);

	return 0;
}


static void InitSignalHandlers()
{
	struct sigaction sa1 = {0};
	struct sigaction sa2 = {0};
	
	sa1.sa_handler = SignalHandleReceivedLifeSign;
    sigaction(SIGUSR1, &sa1, 0);
    sa2.sa_handler = SignalHandleReceivedDNR;
    sigaction(SIGUSR2, &sa2, 0);
}


static void SignalHandleReceivedLifeSign(int signum)		/* maybe make this extern? */
{
	if (signum == SIGUSR1)
	{
		printf("WD process, signal handler - received life sign from client. zeroing counter\n");
/*		g_is_wd_alive = WD_ALIVE;*/
		atomic_store(&repetition_counter, 0);
	}
}


static void SignalHandleReceivedDNR(int signum)				/* maybe make this extern? */
{
	if (signum == SIGUSR2)
	{
		atomic_store(&g_mmi_active, MMI_DISABLED);	/* not sure */
		atomic_store(&repetition_counter, 0);
/*		kill(g_user_pid, SIGUSR2);		*/
		SchedulerStop(scheduler);
	}
}



static int SchedulerActionSendSignal(void *param)
{
	printf("WD process, sending SIGUSR to client process\n");
	kill(g_user_pid, SIGUSR1);
	
	(void)param;
	return 0;
}



static int SchedulerActionIncreaseCounter(void *param)
{
	int current_count = 0;
	
	atomic_fetch_add(&repetition_counter, 1);
	current_count = atomic_load(&repetition_counter);
	printf("WD process. current count = %d\n", current_count);
    if ((size_t)current_count == max_repetitions)		/* make this thread safe */
    {
        printf("WD process - repetition counter reached max! = %d, creating new client\n", current_count);
/*        SchedulerAddTask(scheduler, SchedulerActionReviveWD, NULL, NULL, 0);*/
		SchedulerActionReviveClient(NULL);
    }
    
    (void)param;
	return 0;
}



static int SchedulerActionReviveClient(void *param)
{
	/* reset counter and scheduler and wait for new WD process */
	SchedulerStop(scheduler);
	atomic_store(&repetition_counter, 0);
	
	CreateClientProcess();	/* check for fail? */
	
	/* wait for the client process to initialize */
	sem_wait(process_sem);
	
	/* resume scheduler */
	SchedulerRun(scheduler);
	
	(void)param;
	return 0;
}


static int CreateClientProcess(void)
{
	int return_status = 0;
    
	g_user_pid = fork();
	
	if (g_user_pid == 0)	/* in child (client) process */
	{
		return_status = execl(user_exec_path, user_exec_path, NULL);	/* add user argv here!!! */
		printf("WD Exec (trying to create client) failed. return status = %d\n", return_status);
	}
	else				/* in parent process */
	{
		return return_status;
	}
	
	return return_status;
}

