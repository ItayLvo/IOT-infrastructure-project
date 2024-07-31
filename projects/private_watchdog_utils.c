#define _XOPEN_SOURCE 700	/* sigaction */

#include <stddef.h>	/* size_t */
#include <stdio.h>	/* printf */
#include <sys/types.h>	/* pid_t */
#include <unistd.h>	/* waitpid, fork, write */
#include <sys/wait.h>	/* waitpid */
#include <signal.h>	/* sigaction */
#include <semaphore.h>	/* POSIX semaphore functions and definitions */
#include <pthread.h>	/* POSIX pthread functions */
#include <stdatomic.h>	/* atomic types */

#include "private_watchdog_utils.h"
#include "scheduler.h"


/* static helper functions */
static int Revive(void);
static int SchedulerActionIncreaseCounter(void *param);
static int SchedulerActionSendSignal(void *param);
static void SignalHandleReceivedDNR(int signum);
static void SignalHandleReceivedLifeSign(int signum);



/* global static variables */
volatile atomic_int repetition_counter = ATOMIC_VAR_INIT(0);
pid_t g_partner_pid = 0;
sem_t *process_sem;
scheduler_t *scheduler;
char *user_exec_path;
size_t max_repetitions;
size_t interval;
struct sigaction old_sig_action1;
struct sigaction old_sig_action2;



int InitScheduler(void)
{
	scheduler = SchedulerCreate();
	if (scheduler == NULL)
	{
		perror("SchedulerCreate\n");
		return 1;	
	}
	
	if (UIDIsEqual(SchedulerAddTask(scheduler, SchedulerActionSendSignal, NULL, NULL, interval), UIDGetBad()))
	{
		perror("SchedulerAddTask\n");
		return 1;
	}
	
	if (UIDIsEqual(SchedulerAddTask(scheduler, SchedulerActionIncreaseCounter, NULL, NULL, interval), UIDGetBad()))
	{
		perror("SchedulerAddTask\n");
		return 1;
	}
	
	return 0;
}



int InitSignalHandlers(void)
{
	struct sigaction sa1 = {0};
	struct sigaction sa2 = {0};
	sigset_t set;

	/* unblock SIGUSR1 and SIGUSR2 in current thread */
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	pthread_sigmask(SIG_UNBLOCK, &set, NULL);

	/* set signal handler for SIGUSR1 and SIGUSR2, and save old handlers */
	sa1.sa_handler = SignalHandleReceivedLifeSign;
	sa2.sa_handler = SignalHandleReceivedDNR;

	if (sigaction(SIGUSR1, &sa1, &old_sig_action1) == -1) 
	{
		perror("sigaction sa1\n");
		return 1;
	}

	if (sigaction(SIGUSR2, &sa2, &old_sig_action2) == -1) 
	{
		perror("sigaction sa2\n");
		return 1;
	}

	return 0;
}



static void SignalHandleReceivedLifeSign(int signum)
{
	printf("%d : Received life sign. Reseting counter\n", getpid());
	atomic_store(&repetition_counter, 0);
	UNUSED(signum);
}



static void SignalHandleReceivedDNR(int signum)
{
	printf("%d : Received DNR signal\n", getpid());
	SchedulerStop(scheduler);
	UNUSED(signum);
}



static int SchedulerActionSendSignal(void *param)
{
	printf("%d : Sending SIGUSR1 to %d\n", getpid(), g_partner_pid);
	kill(g_partner_pid, SIGUSR1);
	
	UNUSED(param);
	return 0;
}



static int SchedulerActionIncreaseCounter(void *param)
{
	int current_count = 0;

	atomic_fetch_add(&repetition_counter, 1);
	
	current_count = atomic_load(&repetition_counter);	/* redundant, just for test printing */
	printf("%d : Increasing counter to %d\n", getpid(), current_count);

	/* if number of repetition reached limit - revive the partner process */
	if ((size_t)(atomic_load(&repetition_counter)) == max_repetitions)
	{
		printf("%d : Reached max repetitions! Calling Revive()\n", getpid());
		Revive();
	}

	UNUSED(param);
	return 0;
}



static int Revive(void)
{
	/* reset counter and scheduler */
	SchedulerStop(scheduler);
	atomic_store(&repetition_counter, 0);
	
	/* create partner process with fork+exec */
	CreatePartnerProcess();
	
	/* wait for the partner process to initialize */
	sem_wait(process_sem);
	printf("%d : Revived partner process, and waiting for semaphore. Resuming scheduler\n", getpid());
	/* resume scheduler */
	SchedulerAddTask(scheduler, SchedulerActionIncreaseCounter, NULL, NULL, interval);
	SchedulerRun(scheduler);
	
	return 0;
}

