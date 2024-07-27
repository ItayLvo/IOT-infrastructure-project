#define _XOPEN_SOURCE 700		/* sigaction struct */


#include <stddef.h>		/* size_t */
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
#define DECIMAL_BASE 10
#define UNUSED(var) (void)(var)

/* global static variables */
static volatile atomic_int repetition_counter = ATOMIC_VAR_INIT(0);
static pid_t g_user_pid = 0;
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

static int ReviveClient(void);	/* make this non-scheduler func? */
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
	
	/* cleanup after receiving SIGUSR2 (DNR) */
	printf("WD process, after ShedulerRun...?\n");
	SchedulerDestroy(scheduler);
	atomic_store(&repetition_counter, 0);
	
	UNUSED(argc);
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
	scheduler = SchedulerCreate();
	if (scheduler == NULL)
	{
		return 1;
	}
	
	SchedulerAddTask(scheduler, SchedulerActionSendSignal, NULL, NULL, interval);
	SchedulerAddTask(scheduler, SchedulerActionIncreaseCounter, NULL, NULL, interval);

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


static void SignalHandleReceivedLifeSign(int signum)
{
	printf("WD process\t Signal Handler\t received life sign from client. zeroing counter\n");
	atomic_store(&repetition_counter, 0);
	UNUSED(signum);
}


static void SignalHandleReceivedDNR(int signum)
{
	SchedulerStop(scheduler);
	UNUSED(signum);
}



static int SchedulerActionSendSignal(void *param)
{
	printf("WD process\t Action Function\t sending SIGUSR to (client = %d) process\n", g_user_pid);
	kill(g_user_pid, SIGUSR1);
	
	UNUSED(param);
	return 0;
}



static int SchedulerActionIncreaseCounter(void *param)
{
	int current_count = 0;
	
	atomic_fetch_add(&repetition_counter, 1);
	current_count = atomic_load(&repetition_counter);
	printf("WD process\t Action Function\t increase counter func. current count = %d\n", current_count);
    if ((size_t)current_count == max_repetitions)		/* make this thread safe */
    {
        printf("****WD process\t Action Function\t repetition counter reached max! = %d, creating new client\n", current_count);
/*        SchedulerAddTask(scheduler, SchedulerActionReviveWD, NULL, NULL, 0);*/
		ReviveClient();
		printf("*******WD process\t Action Function\t returned from ReviveClient()\n");
    }
    
    UNUSED(param);
	return 0;
}



static int ReviveClient(void)
{
	/* reset counter and scheduler and wait for new WD process */
	SchedulerStop(scheduler);
	atomic_store(&repetition_counter, 0);
	
	CreateClientProcess();	/* check for fail? */
	
	/* wait for the client process to initialize */
	sem_wait(process_sem);
	printf("*******WD process\t Static Function\t revived client - after sem wait. resuming scheduler\n");
	/* resume scheduler */
	SchedulerAddTask(scheduler, SchedulerActionIncreaseCounter, NULL, NULL, interval);
	SchedulerRun(scheduler);
	
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

