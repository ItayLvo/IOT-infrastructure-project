#define _XOPEN_SOURCE 700
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


static void HandlePong(int signum);


int main()	/* in child process */
{
	struct sigaction sa2 = {0};
    sa2.sa_handler = HandlePong;
    sigaction(SIGUSR2, &sa2, 0);
    
	while(1)
	{
		kill(getppid(), SIGUSR1);
		pause();
	}
	
	return 0;
}



static void HandlePong(int signum)
{
	if (signum == SIGUSR2)
	{
		printf("Pong\n");
		sleep(1);
		kill(getppid(), SIGUSR1);
	}
}

