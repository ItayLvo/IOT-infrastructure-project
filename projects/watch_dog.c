#define _XOPEN_SOURCE 700		/* sigaction struct */

/*
Date: 
Status: 
Reviwer: 
*/


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
static void HandleReceivedLifeSign(int signum);
static void *ThreadCommunicateWithWD(void *param);

static int ActionSendSignal(void *scheduler);
static int ActionIncreaseCounter(void *scheduler);



/* global static variables */
static atomic_int repitition_counter = ATOMIC_VAR_INIT(0);
static pid_t g_wd_pid = 0;
static atomic_int g_mmi_active = MMI_DISABLED;
static int g_is_wd_alive = WD_DEAD;		/* is this needed? because also using repitition_counter... */
static sem_t thread_ready_sem;
static sem_t *process_sem;
static scheduler_t *scheduler = NULL;


/** signal handlers **/
static void HandleReceivedLifeSign(int signum)
{
	if (signum == SIGUSR1)
	{
/*		g_is_wd_alive = WD_ALIVE;*/
		atomic_store(&repitition_counter, 0);
	}
}


static void HandleReceivedDNR(int signum)
{
	if (signum == SIGUSR2)
	{
		atomic_store(&g_mmi_active, MMI_DISABLED);
		atomic_store(&repitition_counter, 0);
		SchedulerStop(scheduler);
	}
}


/** thread function **/
static void *ThreadCommunicateWithWD(void *param)
{
	/* wait for the WD process to initialize */
	sem_wait(&process_sem);
	
	/* init scheduler and load tasks */
	scheduler = SchedulerCreate();
	if (scheduler == NULL)
	{
		return NULL;
	}
	
	uid_signal = SchedulerAddTask(scheduler, ActionStopSchedulerFunc, GenericCleanFunc, scheduler, interval_in_seconds + 20);
	
	/* update main thread that communication is ready */
	sem_post(&thread_ready_sem);
	
	/* run scheduler */
	SchedulerRun(scheduler);
	
	/* destroy scheduler - will only reach this part after receiving SIGUSR2 (DNR) */
	SchedulerDestroy(scheduler);
	
/*	while (g_mmi_active == MMI_ACTIVE && )*/

	
	(void)param;
	return NULL;
}




int MMI(size_t interval_in_seconds, size_t repititions, char **argv)
{
	int return_status = 0;
	char *user_exec_path = argv[0];	/* send this to watchdog process somehow? */
	char *wd_args[] = {WATCH_DOG_PATH, NULL};
	
	process_sem = sem_open(SEMAPHORE_NAME, O_CREAT, SEM_PERMISSIONS, 0);
	pthread_t communication_thread;
	
	/* init signal handlers */	
	struct sigaction sa1 = {0};
	struct sigaction sa2 = {0};
	sa1.sa_handler = HandleReceivedLifeSign;
    sigaction(SIGUSR1, &sa1, 0);
    sa2.sa_handler = HandleReceivedDNR;
    sigaction(SIGUSR2, &sa2, 0);
    
	atomic_store(&g_mmi_active, MMI_ACTIVE);
	sem_init(&thread_ready_sem, 0, 0);
	
	/* fork + exec WD process */
	g_wd_pid = fork();
	
	if (g_wd_pid != 0)		/* in parent (client) process */
	{	
		/* create communication thread */
		return_status = pthread_create(&communication_thread, NULL, &ThreadCommunicateWithWD, NULL);
		if (return_status != 0)
		{
		    fprintf(stderr, "failed to create thread\n");
		    return return_status;
		}
		
		/* wait for thread to set up, then return and exit function */
		sem_wait(&thread_ready_sem);
		return 0;
	}
	
	
	else					/* in child (WD) process */
	{
		return_status = execvp(wd_args[0], wd_args);
		printf("Exec failed. return status = %d\n", return_status);
	}
	
	
	return 0;
}




/** scheduler action functions **/
static int ActionSendSignal(void *scheduler)
{
	kill(g_wd_pid, SIGUSR1);
	return 0;
}

static int ActionIncreaseCounter(void *scheduler)
{
	atomic_fetch_add(&repitition_counter, 1);
	return 0;
}




