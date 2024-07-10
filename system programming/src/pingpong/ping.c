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

#include "pingpong.h"


static void HandlePing(int num);

static pid_t g_child_pid = 0;




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

