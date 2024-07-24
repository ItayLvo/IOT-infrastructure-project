#define _XOPEN_SOURCE 700		/* sigaction struct */


#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */
#include <stdio.h>	/* printf */
#include <sys/types.h>	/* pid_t */
#include <unistd.h>		/* waitpid, fork, write */
#include <sys/wait.h>	/* waitpid */
#include <signal.h>		/* sigaction */

#include "watch_dog.h"


#define SEMAPHORE_NAME "/wd_sem"

/* global static variables */
static atomic_int repetition_counter = ATOMIC_VAR_INIT(0);
static pid_t g_user_pid = 0;
static atomic_int g_mmi_active = MMI_DISABLED;
static int g_is_wd_alive = WD_DEAD;		/* is this needed? because also using repetition_counter... */
static sem_t *process_sem;
static scheduler_t *scheduler;
static char *user_exec_path;
static size_t max_repetitions;
static size_t interval;


/* forward function declerations */
static void InitSignalHandlers(void);
static void SignalHandleReceivedLifeSign(int signum);
static void SignalHandleReceivedDNR(int signum);





int main(int argc, char *argv[])
{
	user_exec_path = argv[1];
	interval = *(size_t *)argv[2];
	max_repetitions = *(size_t *)argv[3];
	
	InitSignalHandlers();
	
	if (InitScheduler() != 0)
	{
		return 1;
	}
	
	
	/* open the existing IPC semaphore and post to it, notifying parent process that WD is ready */
    process_sem = sem_open(SEMAPHORE_NAME, 0);
    if (process_sem == SEM_FAILED)
    {
        perror("sem_open failed\n");
		exit(EXIT_FAILURE);
    }
	sem_post(process_sem);
	
	/* run scheduler */
	SchedulerRun(scheduler);
	
	/* destroy scheduler - will only reach this part after receiving SIGUSR2 (DNR) */
	SchedulerDestroy(scheduler);
	
}



static int InitScheduler(void)
{
	scheduler = SchedulerCreate();
	ilrd_uid_t task_signal_life_sign = {0};
	ilrd_uid_t task_watchdog_tick = {0};
	
	if (scheduler == NULL)
	{
		return 1;
	}
	
	task_signal_life_sign = SchedulerAddTask(scheduler, SchedulerActionSendSignal, NULL, NULL, interval);
	task_watchdog_tick = SchedulerAddTask(scheduler, SchedulerActionIncreaseCounter, NULL, NULL, interval);
	
	return 0;
}


static void InitSignalHandlers()
{
	struct sigaction sa1 = {0};
	struct sigaction sa2 = {0};
	
	sa1.sa_handler = SignalHandleReceivedLifeSign;
    sigaction(SIGUSR1, &sa1, 0);
    sa2.sa_handler = SignalHandleReceivedDNR;
    sigaction(SIGUSR2, &sa2, 0);
}


static void SignalHandleReceivedLifeSign(int signum)		/* maybe make this extern? */
{
	if (signum == SIGUSR1)
	{
/*		g_is_wd_alive = WD_ALIVE;*/
		atomic_store(&repetition_counter, 0);
	}
}


static void SignalHandleReceivedDNR(int signum)				/* maybe make this extern? */
{
	if (signum == SIGUSR2)
	{
		atomic_store(&g_mmi_active, MMI_DISABLED);
		atomic_store(&repetition_counter, 0);
		SchedulerStop(scheduler);
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

