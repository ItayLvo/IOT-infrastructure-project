#define _XOPEN_SOURCE 700	/* sigaction, snprintf */

#include <stddef.h>	/* size_t */
#include <stdio.h>	/* printf */
#include <sys/types.h>	/* pid_t */
#include <unistd.h>	/* waitpid, fork, write */
#include <sys/wait.h>	/* waitpid */
#include <signal.h>	/* sigaction, sigset_t */
#include <fcntl.h>	/* O_* constants (O_CREAT) */
#include <semaphore.h>	/* POSIX semaphore functions and definitions */
#include <pthread.h>	/* POSIX pthreads */
#include <stdatomic.h>	/* atomic types */
#include <string.h>	/* snprintf */
#include <stdlib.h>	/* getenv, atoi */

#include "watch_dog.h"			/* MMI, DNR declerations */
#include "private_watchdog_utils.h"
/*	private watchdog utils:	*/
/*
functions:
	InitSignalHandlers(),
	InitScheduler(),
	CreatePartnerProcess()
global variables:
	repetition_counter,
	process_sem,
	scheduler,
	interval,
	user_exec_path,
	old_sig_action
*/
												

#define WATCH_DOG_PATH "/home/itay/git/projects/watchdog_exec.out"
#define SEM_PERMISSIONS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define ENVIRONMENT_VAR "WD_RUNNING"


/* forward function declerations */
static int BlockSignals(void);
static int UnblockSignals(void);
static int CreateCommunicationThread(void);
static void *ThreadCommunicateWithWD(void *param);


/* global variables */
static sem_t thread_ready_sem;
static pthread_t communication_thread;
static char **user_argv;


/* API Functions */
int MMI(size_t interval_in_seconds, size_t repetitions, char **argv)
{
	int return_status = 0;
	char *env_wd_running = NULL;
	user_exec_path = argv[0];
	max_repetitions = repetitions;
	interval = interval_in_seconds;
	user_argv = argv;

	/* semaphore to sync between current process and WD process */
	process_sem = sem_open(SEMAPHORE_NAME, O_CREAT, SEM_PERMISSIONS, 0);
	if (process_sem == SEM_FAILED)
	{
		perror("sem_open failed\n");
	}
	
	sem_init(&thread_ready_sem, 0, 0);

	printf("Client\t MMI func\t before fork+exec\n");
	printf("Client\t MMI func\t Main thread ID = %lu\n", pthread_self());

	/* if envirnent variable exists - WD process is already running. doesn't create new one */
	env_wd_running = getenv(ENVIRONMENT_VAR);
	if (env_wd_running != NULL)
	{
		printf("Client\t MMI func\t WD process alive. environment var is %s\n", env_wd_running);
		/* extract WD PID from envirnent var */
		g_partner_pid = atoi(env_wd_running);
		
		/* notify WD process that client process is ready */
		sem_post(process_sem);
	}

	/* else - WD process does not exist yet, create WD process */
	else
	{
		printf("Client\t MMI func\t environment var not found. Creating WD process\n");
		/* forks + execs WD process */
		return_status = CreatePartnerProcess();
		if (return_status != 0)
		{
			return return_status;
		}
		
		/* wait for the WD process to initialize */
		sem_wait(process_sem);
		
		printf("Client\t MMI func\t WD process ready and posted to sem\n");
	}

	/* mask SIGUSR1 and SIGUSR2 on main thread */
	return_status = BlockSignals();
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



void DNR(void)
{
	int wait_status = 0;
	
	/* signal WD process to stop and cleanup */
	kill(g_partner_pid, SIGUSR2);
	
	/* stop client scheduler */
	kill(getpid(), SIGUSR2);
	
	/* cleanup semaphores */
	sem_destroy(&thread_ready_sem);
	sem_close(process_sem);
	sem_unlink(SEMAPHORE_NAME);
	
	/* cleanup WD environment var */
	unsetenv(ENVIRONMENT_VAR);
	
	/* reset global repetition counter */
	atomic_store(&repetition_counter, 0);
	
	/* wait for communication thread to cleanup */
	pthread_join(communication_thread, NULL);
	
	/* un-mask SIGUSR1 and SIGUSR2 in main thread */
	UnblockSignals();
	
	SchedulerDestroy(scheduler);
	
	/* wait for WD process to cleanup */
	waitpid(g_partner_pid, &wait_status, WNOHANG);
}



/* thread function */
static void *ThreadCommunicateWithWD(void *param)
{
	printf("Client\t Thread func\t starting thread func\n");
	printf("Client\t Thread func\t  Comms Thread ID = %lu\n", pthread_self());
	
	/* initialize signal handlers for current thread */
	InitSignalHandlers();
	
	/* init scheduler and load tasks */
	InitScheduler();
	
	/* update main thread that current thread is ready */
	sem_post(&thread_ready_sem);
	printf("Client\t Thread func\t After posting to main thread. starting scheduler\n");
	
	/* run scheduler */
	SchedulerRun(scheduler);
	
	/* will only reach this part after receiving DNR */
	UNUSED(param);
	return NULL;
}



int CreatePartnerProcess(void)
{
	int return_status = 0;
	char *wd_argv[MAX_ARGV_SIZE] = {0};
	char interval_str[16];
	char max_repetitions_str[16];
	char **dest_runner = wd_argv + 4;
	char **src_runner = user_argv + 1;

	/* convert integers to strings for exec argv argument */
	snprintf(interval_str, sizeof(interval_str), "%lu", interval);
	snprintf(max_repetitions_str, sizeof(max_repetitions_str), "%lu", max_repetitions);

	wd_argv[0] = WATCH_DOG_PATH;
	wd_argv[1] = user_exec_path;
	wd_argv[2] = interval_str;
	wd_argv[3] = max_repetitions_str;

	while (*src_runner != NULL)
	{
		*dest_runner = *src_runner;
		++src_runner;
		++dest_runner;
	}

	*dest_runner = NULL;

	g_partner_pid = fork();

	if (g_partner_pid == 0)	/* in child (WD) process */
	{
		return_status = execv(wd_argv[0], wd_argv);
		printf("Exec failed. return status = %d\n", return_status);
	}
	else if (g_partner_pid < 0)		/* if fork() failed */		
	{
		return_status = g_partner_pid;
	}
	
	/* after fork(), in parent process */
	return return_status;
}




static int CreateCommunicationThread(void)
{
	int return_status = 0;
	
	return_status = pthread_create(&communication_thread, NULL, &ThreadCommunicateWithWD, NULL);
	
	if (return_status != 0)
	{
	    perror("pthread_create\n");
	    return return_status;
	}
	
	/* wait for thread to set up, then return and exit function */
	sem_wait(&thread_ready_sem);
	
	return return_status;
}



static int BlockSignals(void)
{
	sigset_t set;

	/* mask SIGUSR1 and SIGUSR2 in the main thread */
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	
	if (pthread_sigmask(SIG_BLOCK, &set, NULL) != 0)
	{
		perror("pthread_sigmask\n");
		return 1;
	}
	
	return 0;
}


static int UnblockSignals(void)
{
	/* restore SIGUSR1 and SIGUSR2 to previous behavior (before MMI) */
	if (sigaction(SIGUSR1, &old_sig_action1, NULL) == -1)
	{
		perror("sigaction\n");
		return 1;
	}
	
	if (sigaction(SIGUSR2, &old_sig_action2, NULL) == -1)
	{
		perror("sigaction\n");
		return 1;
	}
	
	return 0;
}



