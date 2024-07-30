#ifndef __PRIVATE_WATCHDOG_UTILS_H__
#define __PRIVATE_WATCHDOG_UTILS_H__

/****************************************************
	private_watchdog_utils is used for
	defines, global variables and functions used
	in both the client process (watch_dog.c) and
	the WD process (wd_process.c)
****************************************************/

#include <stdatomic.h>	/* atomic_int */
#include <semaphore.h>	/* sem_t */
#include "scheduler.h"	/* scheduler_t */


/* defines */
#define SEMAPHORE_NAME "/wd_sem"
#define UNUSED(var) (void)(var)
#define MAX_ARGV_SIZE 64



/* function forward declerations */
int InitSignalHandlers(void);
int InitScheduler(void);
int CreatePartnerProcess(void);		/* implemented separately in each file */


/* additional static helper functions, implemented in private_watchdog_utils.c : */
/* void SignalHandleReceivedLifeSign(int signum);*/
/* void SignalHandleReceivedDNR(int signum);*/
/* int SchedulerActionSendSignal(void *param);*/
/* int SchedulerActionIncreaseCounter(void *param);*/
/* int Revive(void);*/



/* static global vars */
extern volatile atomic_int repetition_counter;
extern pid_t g_partner_pid;
extern sem_t *process_sem;
extern scheduler_t *scheduler;
extern char *user_exec_path;
extern size_t max_repetitions;
extern size_t interval;
extern struct sigaction old_sig_action1;
extern struct sigaction old_sig_action2;





#endif /*__PRIVATE_WATCHDOG_UTILS_H__*/
