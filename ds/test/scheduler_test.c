#include <stdio.h>	/* printf */

#include "scheduler.h"		/* scheduler_t functions */
#include "uid.h"		/* uid_t functions */

int PrintAndIncrementNumActionFunc(void *num);
int PrintAndIncrementCharActionFunc(void *ch);
int GenericCleanFunc(void);
int ActionStopSchedulerFunc(void *scheduler);

int main()
{
	scheduler_t *scheduler = SchedulerCreate();
	int x1 = 1;
	char ch1 = 'a';
	size_t interval_in_seconds = 1;
	
	ilrd_uid_t uid3 = SchedulerAddTask(scheduler,
				ActionStopSchedulerFunc,
				GenericCleanFunc,
				scheduler,
				interval_in_seconds+20);
				
				
	ilrd_uid_t uid1 = SchedulerAddTask(scheduler,
				PrintAndIncrementNumActionFunc,
				GenericCleanFunc,
				&x1,
				interval_in_seconds);
	
				
	ilrd_uid_t uid2 = SchedulerAddTask(scheduler,
				PrintAndIncrementCharActionFunc,
				GenericCleanFunc,
				&ch1,
				interval_in_seconds+1);
	
	
	
	
	SchedulerRun(scheduler);
		
	SchedulerDestroy(scheduler);
	
	return 0;
}



int PrintAndIncrementNumActionFunc(void *num)
{
	printf("%d\n", *(int *)num);
	++*(int *)num;
	
	/*
	if (*(int *)num == 5)
	{
		return -1;
	}
	*/
	
	return 0;
}

int PrintAndIncrementCharActionFunc(void *ch)
{
	printf("%c\n", *(char *)ch);
	++*(char *)ch;
	

	
	return 0;
}

int ActionStopSchedulerFunc(void *scheduler)
{
	SchedulerStop(scheduler);
	return 0;
}

int GenericCleanFunc(void)
{
	printf("cleaned\n");
	
	return 0;
}
