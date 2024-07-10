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
#include <stdlib.h>		/* atoi */



static void HandlePong(int signum, siginfo_t *info, void *context);


int main(int argc, char *argv[])	/* in child process */
{

	pid_t g_exec_pid = (pid_t)atoi(argv[1]);
	
	struct sigaction sa2 = {0};
    sa2.sa_sigaction = HandlePong;
	sa2.sa_flags = 0;
	sa2.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR2, &sa2, 0);
    
    printf("Pong process running, sending SIGUSR1\n");
	kill(g_exec_pid, SIGUSR1);
    
	while(1)
	{
		pause();
	}
	
	(void)argc;
	return 0;
}


static void HandlePong(int signum, siginfo_t *info, void *context)
{
	pid_t sender_pid = info->si_pid;
	
	if (signum == SIGUSR2)
	{
		printf("Pong\n");
		sleep(1);
		kill(sender_pid, SIGUSR1);
	}
	
	(void)context;
}

