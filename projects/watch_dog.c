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

#include "watch_dog.h"
#include "scheduler.h"
#include "uid.h"

#define WATCH_DOG_PATH "/home/itay/git/projects/wd_process"
#define MMI_ACTIVE 1
#define MMI_DISABLED 0
#define WD_ALIVE 1
#define WD_DEAD 0
#define SEMAPHORE_NAME "/wd_sem"
#define SEM_PERMISSIONS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) /* 0644 in octal */
	/*S_IRUSR: Read permission for the owner*/
	/*S_IWUSR: Write permission for the owner*/
	/*S_IRGRP: Read permission for the group*/
	/*S_IROTH: Read permission for others*/
	


/* forward function declerations */
static void InitSignalHandlers();
static void SignalHandleReceivedLifeSign(int signum);
static void SignalHandleReceivedDNR(int signum);

static void *ThreadCommunicateWithWD(void *param);

static int SchedulerActionSendSignal(void *param);
static int SchedulerActionIncreaseCounter(void *param);
static int SchedulerActionReviveWD(void *param);

static int CreateCommunicationThread(void);
static int CreateWDProcess(void);




/* global static variables */
static atomic_int repetition_counter = ATOMIC_VAR_INIT(0);
static pid_t g_wd_pid = 0;
static atomic_int g_mmi_active = MMI_DISABLED;
static int g_is_wd_alive = WD_DEAD;		/* is this needed? because also using repetition_counter... */
static sem_t thread_ready_sem;
static sem_t *process_sem;
static scheduler_t *scheduler;
static char *user_exec_path;
static size_t max_repetitions;
static size_t interval;


/** thread function **/
static void *ThreadCommunicateWithWD(void *param)
{
	/* wait for the WD process to initialize */
	sem_wait(process_sem);
	
	/* init scheduler and load tasks */
	scheduler = SchedulerCreate();
	if (scheduler == NULL)
	{
		return NULL;	/* how do i know it failed? */
	}
	
	
/*	ilrd_uid_t SchedulerAddTask(scheduler_t *scheduler, scheduler_action_func_t, scheduler_clean_func_t, void *action_param, size_t time_interval);*/
	ilrd_uid_t task_signal_life_sign = SchedulerAddTask(scheduler, SchedulerActionSendSignal, NULL, NULL, interval);
	ilrd_uid_t task_watchdog_tick = SchedulerAddTask(scheduler, SchedulerActionIncreaseCounter, NULL, NULL, interval);
	
	
	/* update main thread that communication is ready */
	sem_post(&thread_ready_sem);
	
	/* run scheduler */
	SchedulerRun(scheduler);
	
	/* destroy scheduler - will only reach this part after receiving SIGUSR2 (DNR) */
	SchedulerDestroy(scheduler);

	(void)param;
	return NULL;
}




int MMI(size_t interval_in_seconds, size_t repetitions, char **argv)
{
	int return_status = 0;
	user_exec_path = argv[0];
	max_repetitions = repetitions;
	interval = interval_in_seconds;
	
	/* semaphore to sync between current process and WD process */
	process_sem = sem_open(SEMAPHORE_NAME, O_CREAT, SEM_PERMISSIONS, 0);
	
	InitSignalHandlers();
    
	atomic_store(&g_mmi_active, MMI_ACTIVE);
	sem_init(&thread_ready_sem, 0, 0);
	
	
	/* fork + exec WD process */
	return_status = CreateWDProcess();
	if (return_status != 0)
	{
		return return_status;
	}
	
	
	/* create communication thread */
	return_status = CreateCommunicationThread();
	if (return_status != 0)
	{
		return return_status;
	}
	

	return return_status;
}





static int CreateWDProcess(void)
{
	int return_status = 0;
	
	g_wd_pid = fork();
	
	if (g_wd_pid == 0)	/* in child (WD) process */
	{
		return_status = execl(WATCH_DOG_PATH, WATCH_DOG_PATH, user_exec_path, &interval, &max_repetitions, NULL);
		printf("Exec failed. return status = %d\n", return_status);
	}
	else				/* in parent process */
	{
		return return_status;
	}
}


static int CreateCommunicationThread(void)
{
	pthread_t communication_thread;
	pthread_attr_t attr;
	int return_status = 0;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	return_status = pthread_create(&communication_thread, &attr, &ThreadCommunicateWithWD, NULL);
	
	if (return_status != 0)
	{
	    fprintf(stderr, "failed to create thread\n");
	    return return_status;
	}
	
	/* wait for thread to set up, then return and exit function */
	sem_wait(&thread_ready_sem);
	
	return return_status;
}



/** signal handlers **/

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
	if (signum == SIGUSR1)
	{
/*		g_is_wd_alive = WD_ALIVE;*/
		atomic_store(&repetition_counter, 0);
	}
}


static void SignalHandleReceivedDNR(int signum)
{
	if (signum == SIGUSR2)
	{
		atomic_store(&g_mmi_active, MMI_DISABLED);
		atomic_store(&repetition_counter, 0);
		SchedulerStop(scheduler);
	}
}




/** scheduler action functions **/
static int SchedulerActionReviveWD(void *param)
{
	return CreateWDProcess();
}


static int SchedulerActionSendSignal(void *param)
{
	kill(g_wd_pid, SIGUSR1);
	return 0;
}


static int SchedulerActionIncreaseCounter(void *param)
{
	int current_count = 0;
	
	atomic_fetch_add(&repetition_counter, 1);
	current_count = atomic_load(&repetition_counter);
	
    if ((size_t)current_count == max_repetitions)		/* make this thread safe */
    {
        printf("Repetition counter reached max!\n");
        SchedulerAddTask(scheduler, SchedulerActionReviveWD, NULL, NULL, 0);		/* finish of this part of code */
    }
    
	return 0;
}




