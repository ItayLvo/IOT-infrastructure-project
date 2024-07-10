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


static void HandlePing(int sig, siginfo_t *info, void *context);

int main()
{
	
	struct sigaction sa1 = {0};
	sa1.sa_sigaction = HandlePing;
	sa1.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa1, 0);
	
	printf("Ping process running, waiting for serve.\tPID =  %d\n", getpid());
	while(1)
	{
/*			sigaction(SIGUSR1, &sa1, 0);*/
			pause();
	}
}



static void HandlePing(int signum, siginfo_t *info, void *context)
{
	pid_t sender_pid = info->si_pid; /* get the sender's PID */
/*	printf("Received signal %d from process %d\n", signum, sender_pid);*/

	if (signum == SIGUSR1)
	{
		printf("Ping\n");
		sleep(1);
		kill(sender_pid, SIGUSR2);
	}
	
	(void)context;
}

