#ifndef __PRODUCERS_CONSUMERS_H__
#define __PRODUCERS_CONSUMERS_H__

typedef enum status	/* todo: adjust */
{
	SUCCESS = 0,
	ERROR = 1
} prod_cons_status;

int RunConditionalVariable(void);
int RunThreadSafeFSQ(void);
int RunTwoSemaphores(void);
int RunSemaphore(void);
int RunMutex(void);
int RunSpinLock(void);


#endif /* __PRODUCERS_CONSUMERS_H__ */
