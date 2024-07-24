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
#include <stdlib.h>		/* exit */

#include "watch_dog.h"
#include "scheduler.h"
#include "uid.h"

#define SEMAPHORE_NAME "/wd_sem"
#define MMI_ACTIVE 1
#define MMI_DISABLED 0
#define WD_ALIVE 1
#define WD_DEAD 0

/* global static variables */
static atomic_int repetition_counter = ATOMIC_VAR_INIT(0);
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

static void SignalHandleReceivedLifeSign(int signum);
static void SignalHandleReceivedDNR(int signum);

static int SchedulerActionIncreaseCounter(void *param);
static int SchedulerActionSendSignal(void *param);





int main(int argc, char *argv[])
{
	user_exec_path = argv[1];
	interval = *(size_t *)argv[2];
	max_repetitions = *(size_t *)argv[3];
	g_user_pid = getppid();
	
	InitSignalHandlers();
	
	if (InitScheduler() != 0)
	{
		return 1;
	}
	
	
	/* open the existing IPC semaphore and post to it, notifying client process that WD is ready */
    process_sem = sem_open(SEMAPHORE_NAME, 0);
    if (process_sem == SEM_FAILED)
    {
        perror("sem_open failed\n");
		exit(EXIT_FAILURE);
    }
    
    printf("WD process, main func, before posting to semaphore\n");
	sem_post(process_sem);
	printf("WD process, main func, after posting to semaphore. starting scheduler\n");
	/* run scheduler */
	SchedulerRun(scheduler);
	
	/* destroy scheduler - will only reach this part after receiving SIGUSR2 (DNR) */
	SchedulerDestroy(scheduler);
	
	(void)argc;
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
/*		g_is_wd_alive = WD_ALIVE;*/
		atomic_store(&repetition_counter, 0);
	}
}


static void SignalHandleReceivedDNR(int signum)				/* maybe make this extern? */
{
	if (signum == SIGUSR2)
	{
		atomic_store(&g_mmi_active, MMI_DISABLED);
		atomic_store(&repetition_counter, 0);
		kill(g_user_pid, SIGUSR2);		/* not sure */
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
	printf("WD process, action func, sending SIGUSR1 to client process. current count = %d\n", current_count);
    if ((size_t)current_count == max_repetitions)		/* make this thread safe */
    {
        printf("Repetition counter reached max! = %d\n", current_count);
/*        SchedulerAddTask(scheduler, SchedulerActionReviveWD, NULL, NULL, 0);*/
    }
    
    (void)param;
	return 0;
}

