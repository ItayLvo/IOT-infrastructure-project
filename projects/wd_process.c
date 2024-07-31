#define _XOPEN_SOURCE 700	/* sigaction, snprintf */

#include <stddef.h>	/* size_t */
#include <stdio.h>	/* printf */
#include <sys/types.h>	/* pid_t */
#include <unistd.h>	/* waitpid, fork, write */
#include <sys/wait.h>	/* waitpid */
#include <signal.h>	/* sigaction */
#include <fcntl.h>	/* O_* constants (O_CREAT)*/
#include <semaphore.h>	/* POSIX semaphore functions and definitions */
#include <pthread.h>	/* POSIX pthreads */
#include <stdatomic.h>	/* atomic types */
#include <stdlib.h>	/* exit, strtoul, setenv */
#include <string.h>	/* sprintf , snprintf */

#include "private_watchdog_utils.h"	/*	functions:
												InitSignalHandlers(),
												InitScheduler(),
												CreatePartnerProcess()
										global variables:
												repetition_counter,
												process_sem,
												scheduler,
												interval,
												user_exec_path */

#define DECIMAL_BASE 10


/* forward function declerations */
static int InitEnvVar(void);
static void InitClientArgv();


/* global static variables */
static char *user_argv[MAX_ARGV_SIZE];
static char **wd_argv;


int main(int argc, char *argv[])
{
	g_partner_pid = getppid();

	/* assign global vars based on client argv MMI args */
	wd_argv = argv;
	InitClientArgv();

	/* set up SIGUSR1 and SIGUSR2 to handle communication */
	InitSignalHandlers();

	/* set environment variable to indicate that WD process is running */
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
	printf("WD\t Main func\t Loading WD process.\n*interval = %lu\n*max rep = %lu\n*client pid = %u\n*user exec path = %s\n\n", interval, max_repetitions, g_partner_pid, user_exec_path);
	printf("WD\t Main func\t before posting to semaphore\n");
	sem_post(process_sem);
	printf("WD\t Main func\t after posting to semaphore. starting scheduler\n");
	/* run scheduler */
	SchedulerRun(scheduler);

	/* cleanup after receiving SIGUSR2 (DNR) */
	printf("WD\t Main func\t Stopped scheduler. cleaning up WD process and exiting\n");
	SchedulerDestroy(scheduler);
	atomic_store(&repetition_counter, 0);

	UNUSED(argc);
	return 0;
}



int CreatePartnerProcess(void)
{
	int return_status = 0;
    
	g_partner_pid = fork();
	
	if (g_partner_pid == 0)	/* in child (client) process */
	{
		return_status = execv(user_argv[0], user_argv);
		printf("WD: execv failed. return status = %d\n", return_status);
	}
	else				/* in parent process */
	{
		return return_status;
	}
	
	return return_status;
}



static void InitClientArgv()
{
	char **dest_runner = user_argv + 1;
	char **src_runner = wd_argv + 4;

	/* assign global variables based on MMI arguments */
	user_exec_path = wd_argv[1];
	/* convert argv arguments back to size_t */
	interval = strtoul(wd_argv[2], NULL, DECIMAL_BASE);  
	max_repetitions = strtoul(wd_argv[3], NULL, DECIMAL_BASE);


	/* re-construct argv for client exec */
	user_argv[0] = user_exec_path;
	while (*src_runner != NULL)
	{
		*dest_runner = *src_runner;
		++src_runner;
		++dest_runner;
	}
}



static int InitEnvVar(void)
{
	pid_t wd_pid = getpid();
	char string_pid[36];

	sprintf(string_pid, "%d", wd_pid);

	/* set "WD_RUNNING" environment variable as the WD PID */
	setenv("WD_RUNNING", string_pid, 1);

	return 0;
}


