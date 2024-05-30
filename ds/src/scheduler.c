#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "uid.h"		/* uid_t functions */
#include "task.h"		/* task_t functions */
#include "priority_queue.h"	/* pq_t functions */



struct scheduler
{
    pq_t *tasks_priority_queue;
};

int CompareInt(const void *item, const void *data_to_compare);



ilrd_scheduler_t *SchedulerCreate(void)
{
	scheduler_t *scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == scheduler)
	{
		return NULL;
	}
	
	scheduler->tasks_priority_queue = PQCreate(CompareInt);
	
	return scheduler;
}


void SchedulerDestroy(scheduler_t *scheduler)
{
	PQDestroy(scheduler->tasks_priority_queue);
	free(scheduler);
}


ilrd_uid_t SchedulerAddTask(scheduler_t *scheduler,
			scheduler_action_func_t action_func,
			scheduler_clean_func_t clean_func,
			void *action_param,
			size_t time_interval)
{
	
	ilrd_uid_t uid = UIDCreate();
	if (UIDGetBad() == uid)
	{
		return uid;
	}
	
	task_t *task = CreateTask(uid, action_func, clean_func,
				action_param, time_interval);
	
	
	PQEnqueue(scheduler->tasks_priority_queue, task);
	
	return uid;
}



/* fix so that smallest item is top priority */
int CompareItemsPriority(const void *item, const void *data_to_compare)
{
 	return (*(int *)item - *(int *)data_to_compare);
}








