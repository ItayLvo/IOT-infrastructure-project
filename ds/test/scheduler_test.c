#include <stdio.h>	/* printf */

#include "scheduler.h"		/* scheduler_t functions */
#include "uid.h"		/* uid_t functions */

int PrintAndIncrementNumActionFunc(void *num);
int PrintAndIncrementCharActionFunc(void *ch);
int GenericCleanFunc(void);
int ActionStopSchedulerFunc(void *scheduler);

static scheduler_t *scheduler = NULL;

/*
gd ./ds/src/priority_qeueu.c ./ds/test/scheduler_test.c ./ds/src/sorted_list.c ./ds/src/dllist.c ./ds/src/scheduler.c ./ds/src/task.c ./ds/src/uid.c -I ./ds/include
*/

int main()
{
	
	char ch1 = 'a';
	size_t interval_in_seconds = 1;
	ilrd_uid_t uid2;
	ilrd_uid_t uid3;
	
	scheduler = SchedulerCreate();
	
	uid3 = SchedulerAddTask(scheduler,
				ActionStopSchedulerFunc,
				GenericCleanFunc,
				scheduler,
				interval_in_seconds + 20);
				
	
				
	uid2 = SchedulerAddTask(scheduler,
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
	
	
	if (*(int *)num == 10)
	{
		return 2;
	}
	
	
	return 0;
}

int PrintAndIncrementCharActionFunc(void *ch)
{
	static int x1 = 1;
	size_t interval_in_seconds = 1;
	
	printf("%c\n", *(char *)ch);
	++*(char *)ch;
	
	if (*(char *)ch == 'c')
	{
		ilrd_uid_t uid1 = SchedulerAddTask(scheduler,
				PrintAndIncrementNumActionFunc,
				GenericCleanFunc,
				&x1,
				interval_in_seconds);
	}

	
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
