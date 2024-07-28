#define _XOPEN_SOURCE 700		/* sigaction struct */

#include <stddef.h>		/* size_t */
#include <stdio.h>		/* printf */
#include <sys/types.h>	/* pid_t */
#include <unistd.h>		/* waitpid, fork, write */
#include <sys/wait.h>	/* waitpid */
#include <signal.h>		/* sigaction, sigset_t */
#include <fcntl.h>		/* O_* constants (O_CREAT) */
#include <semaphore.h>	/* POSIX semaphore functions and definitions */
#include <pthread.h>	/* POSIX pthreads */
#include <stdatomic.h>	/* atomic types */
#include <string.h>		/* snprintf */
#include <stdlib.h>		/* getenv, atoi */

#include "watch_dog.h"
#include "scheduler.h"
#include "uid.h"

#define WATCH_DOG_PATH "/home/itay/git/projects/watchdog_exec.out"
#define SEMAPHORE_NAME "/wd_sem"
#define SEM_PERMISSIONS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define ENVIRONMENT_VAR "WD_RUNNING"
#define UNUSED(var) (void)(var)
#define MAX_ARGV_SIZE 32

/* forward function declerations (grouped by category) */
static int InitSignalHandlers(void);
static int BlockSignals(void);
static int UnblockSignals(void);

static void SignalHandleReceivedLifeSign(int signum);
static void SignalHandleReceivedDNR(int signum);

static int InitScheduler(void);
static int SchedulerActionSendSignal(void *param);
static int SchedulerActionIncreaseCounter(void *param);

static int ReviveWD(void);
static int CreateWDProcess(void);

static int CreateCommunicationThread(void);
static void *ThreadCommunicateWithWD(void *param);



/* global variables */
static volatile atomic_int repetition_counter = ATOMIC_VAR_INIT(0);
static pid_t g_wd_pid = 0;
static sem_t thread_ready_sem;
static sem_t *process_sem;
static scheduler_t *scheduler;
static char *user_exec_path;
static size_t max_repetitions;
static size_t interval;
struct sigaction old_sig_action1;
struct sigaction old_sig_action2;
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
	sem_init(&thread_ready_sem, 0, 0);
	
	printf("client process, MMI func, before fork+exec\n");
	printf("MAIN thread = %lu\n", pthread_self());
	
	/* if envirnent variable exists - WD process is already running. doesn't create new one */
	env_wd_running = getenv(ENVIRONMENT_VAR);
	if (env_wd_running != NULL)
	{
		printf("****WD process alive. env var is %s\n", env_wd_running);
		/* extract WD PID from envirnent var */
		g_wd_pid = atoi(env_wd_running);
		
		/* notify WD process that client process is ready */
		sem_post(process_sem);
	}
	
	/* else - WD process does not exist yet, create WD process */
	else
	{
		printf("****environ var not found, creating WD process\n");
		/* forks + execs WD process */
		return_status = CreateWDProcess();
		if (return_status != 0)
		{
			return return_status;
		}
		
		/* wait for the WD process to initialize */
		sem_wait(process_sem);
		
		printf("client process, MMI func, after fork+exec (after WD proc initialized), before thread_create\n");
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
	/* signal WD process to stop and cleanup */
	kill(g_wd_pid, SIGUSR2);
	
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
	waitpid(g_wd_pid, NULL, 0);
}



/* thread function */
static void *ThreadCommunicateWithWD(void *param)
{
	printf("client process, thread func start\n");
	printf("COMMS thread = %lu\n", pthread_self());
	
	/* initialize signal handlers for current thread */
	InitSignalHandlers();
	
	/* init scheduler and load tasks */
	InitScheduler();
	
	/* update main thread that current thread is ready */
	sem_post(&thread_ready_sem);
	printf("client process, thread func, after posting to main thread, starting scheduler\n");
	
	/* run scheduler */
	SchedulerRun(scheduler);
	
	/* will only reach this part after receiving DNR */
	UNUSED(param);
	return NULL;
}



/* static helper functions */
static int CreateWDProcess(void)
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
    
	g_wd_pid = fork();
	
	if (g_wd_pid == 0)	/* in child (WD) process */
	{
		return_status = execv(wd_argv[0], wd_argv);
		printf("Exec failed. return status = %d\n", return_status);
	}
	else				/* in parent process */
	{
		return return_status;
	}
	
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



static int ReviveWD(void)
{
	/* reset counter and pause scheduler */
	SchedulerStop(scheduler);
	atomic_store(&repetition_counter, 0);
	
	CreateWDProcess();
	
	/* wait for the WD process to initialize */
	sem_wait(process_sem);
	printf("\tClient process\t received post after WD re-created. resuming scheduler\n");
	
	/* resume scheduler */
	SchedulerAddTask(scheduler, SchedulerActionIncreaseCounter, NULL, NULL, interval);
	SchedulerRun(scheduler);
	
	return 0;
}



/* signal handling functions */
static int InitSignalHandlers(void)
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
		perror("sigaction\n");
		return 1;
	}
	
	if (sigaction(SIGUSR2, &sa2, &old_sig_action2) == -1) 
	{
		perror("sigaction\n");
		return 1;
	}
	
	return 0;
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



static void SignalHandleReceivedLifeSign(int signum)
{
	printf("Client process\t Signal Handler\t received life sign from WD. zeroing counter\n");
	atomic_store(&repetition_counter, 0);
	
	UNUSED(signum);
}


static void SignalHandleReceivedDNR(int signum)
{
	SchedulerStop(scheduler);
	printf("Thread %lu received SIGUSR2\n", pthread_self());
	UNUSED(signum);
}



/* scheduler init and action functions */
static int SchedulerActionSendSignal(void *param)
{
	printf("Client process\t Action Function\tsending SIGUSR1 to (WD = %d) process\n", g_wd_pid);
	kill(g_wd_pid, SIGUSR1);
	
	UNUSED(param);
	return 0;
}


static int SchedulerActionIncreaseCounter(void *param)
{
	int current_count = 0;
	
	atomic_fetch_add(&repetition_counter, 1);
	current_count = atomic_load(&repetition_counter);
	printf("Client process\t Action Function\tincrease counter func. current count = %d\n", current_count);
    if ((size_t)current_count == max_repetitions)		/* make this thread safe */
    {
        printf("****Client process\t Action Function\t repetition counter reached max! = %d, creating new WD process\n", current_count);

		ReviveWD();
    }
    
    UNUSED(param);
	return 0;
}


static int InitScheduler(void)
{
	scheduler = SchedulerCreate();
	if (scheduler == NULL)
	{
		return 1;	
	}
	
	if (UIDIsEqual(SchedulerAddTask(scheduler, SchedulerActionSendSignal, NULL, NULL, interval), UIDGetBad()))
	{
		return 1;
	}
	
	if (UIDIsEqual(SchedulerAddTask(scheduler, SchedulerActionIncreaseCounter, NULL, NULL, interval), UIDGetBad()))
	{
		return 1;
	}
	
	return 0;
}


