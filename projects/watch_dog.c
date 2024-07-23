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
#include <pthread.h>	/* threads */
#include <stdatomic.h>	/* atomic_ types */

#include "watch_dog.h"


#define WATCH_DOG_PATH /home/itay/git/projects/wd_process
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

/* global static variables */
static atomic_int counter = ATOMIC_VAR_INIT(0);
static pid_t g_wd_pid = 0;
static int g_is_wd_alive = WD_DEAD;
static int g_mmi_active = MMI_DISABLED;
static sem_t thread_ready_sem;
static sem_t *process_sem;



static void HandleReceivedLifeSign(int signum)
{
	if (signum == SIGUSR1)
	{
		g_is_wd_alive = WD_ALIVE;
	}
}


int MMI(time_t interval_in_seconds, const char *executable_pathname, char **argv)
{
	int return_status = 0;
	char *wd_args[] = {WATCH_DOG_PATH, NULL};
	
	process_sem = sem_open(SEMAPHORE_NAME, O_CREAT, SEM_PERMISSIONS, 0);
	pthread_t communication_thread;
	
	struct sigaction sa1 = {0};
	sa1.sa_handler = HandleReceivedLifeSign;
    sigaction(SIGUSR1, &sa1, 0);
    
	g_mmi_active = MMI_ACTIVE;
	sem_init(&thread_ready_sem, 0, 0);
	
	g_wd_pid = fork();
	
	if (g_wd_pid != 0)		/* in parent process */
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
	
	
	else					/* in child process */
	{
		return_status = execvp(wd_args[0], wd_args);
		printf("Exec failed. return status = %d\n", return_status);
	}
	
	
	return 0;
}


static void *ThreadCommunicateWithWD(void *param)
{
	/* wait for the WD process to initialize */
	sem_wait(&process_sem);
	
	/* update main thread that communication is ready */
	sem_post(&thread_ready_sem);
	while (g_mmi_active == MMI_ACTIVE && )
	{

		sleep(message * 1000000);
	}
	
	
	kill(g_child_pid, SIGUSR2);
	
	(void)param;
	return NULL;
}
		






int PingPong(void)
{
	char *args[] = {"/home/itay/git/system programming/src/pingpong/pong.out", NULL};
	int return_status = 0;
	
	struct sigaction sa1 = {0};
	sa1.sa_handler = HandlePing;
    sigaction(SIGUSR1, &sa1, 0);
	
	g_child_pid = fork();
	
	if (g_child_pid != 0)		/* in parent process */
	{
		while(1)
		{
			pause();
		}
	}
	
	
	else					/* in child process */
	{
		return_status = execvp(args[0], args);
		printf("Exec failed. return status = %d\n", return_status);
	}

	return return_status;
}





