#define _XOPEN_SOURCE 700		/* sigaction struct */

/*
Date: 
Status: 
Reviwer: 
*/


#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */
#include <stdio.h>	/* printf */
#include <sys/types.h>	/* pid_t */
#include <unistd.h>		/* waitpid, fork, write */
#include <sys/wait.h>	/* waitpid */
#include <signal.h>		/* sigaction */

#include "watch_dog.h"

#define WATCH_DOG_PATH 

static void HandlePing(int num);

static pid_t g_wd_pid = 0;




int MMI(time_t interval_in_seconds, const char *executable_pathname, char **argv)
{
	int return_status = 0;
	char *wd_args[] = {WATCH_DOG_PATH, NULL};
	struct sigaction sa1 = {0};
	sa1.sa_handler = HandleReceivedLifeSign;
    sigaction(SIGUSR1, &sa1, 0);
	
	g_wd_pid = fork();
	
	if (g_child_pid != 0)		/* in parent process */
	{
		
	}
	
	
	else					/* in child process */
	{
		return_status = execvp(wd_args[0], wd_args);
		printf("Exec failed. return status = %d\n", return_status);
	}
	
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



static void HandlePing(int signum)
{
	if (signum == SIGUSR1)
	{
		printf("Ping\n");
		sleep(1);
		kill(g_child_pid, SIGUSR2);
	}
}

