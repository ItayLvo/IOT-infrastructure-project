#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */
#include<unistd.h>	/* sleep */

#include "uid.h"		/* uid_t functions */
#include "task.h"		/* task_t functions */
#include "priority_queue.h"	/* pq_t functions */
#include "scheduler.h"		/* scheduler_t functions */

struct scheduler
{
    pq_t *tasks_priority_queue;
};

static int CompareItemPriority(const void *item, const void *data_to_compare);
static int MatchTask(const void *item, const void *data_to_compare);

enum on_status_t {OFF, ON};
static int is_scheduler_on = OFF;

scheduler_t *SchedulerCreate(void)
{
	scheduler_t *scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == scheduler)
	{
		return NULL;
	}
	
	scheduler->tasks_priority_queue = PQCreate(CompareItemPriority);
	
	return scheduler;
}


void SchedulerDestroy(scheduler_t *scheduler)
{
	SchedulerClear(scheduler);
	PQDestroy(scheduler->tasks_priority_queue);
	free(scheduler);
}


ilrd_uid_t SchedulerAddTask(scheduler_t *scheduler,
			scheduler_action_func_t action_func,
			scheduler_clean_func_t clean_func,
			void *action_param,
			size_t time_interval)
{
	int enqueue_status = 0;
	task_t *task = NULL;
	ilrd_uid_t uid = UIDGetBad();
	
	
	task = CreateTask(action_func, clean_func,
				action_param, time_interval);
	uid = TaskGetUid(task);
	
	if (UIDIsEqual(UIDGetBad(), uid))
	{
		return uid;
	}
	
	enqueue_status = PQEnqueue(scheduler->tasks_priority_queue, task);
	if (0 != enqueue_status)
	{
		return UIDGetBad();
	}
	
	return uid;
}



int SchedulerRemove(scheduler_t *scheduler, ilrd_uid_t task_uid)
{
	task_t *erased_task = PQErase(scheduler->tasks_priority_queue, MatchTask, &task_uid);
	
	TaskExecuteCleanFunc(erased_task);
	
	/* if NULL == erase_result, returns 1 --> SchedulerRemovefailed */
	return (NULL == erased_task);
}



int SchedulerRun(scheduler_t *scheduler)
{
	task_t *task;
	size_t time_until_task = 0;
	int action_func_status = 0;
	size_t current_time = 0;
	is_scheduler_on = ON;
	
	while (is_scheduler_on == ON && !SchedulerIsEmpty(scheduler))
	{
		task = PQPeek(scheduler->tasks_priority_queue);
		
		current_time = time(NULL);
		time_until_task = TaskGetTimeToStart(task) - current_time;
		while (time_until_task > 0)
		{
			sleep(1);
			current_time = time(NULL);
			time_until_task = TaskGetTimeToStart(task) - current_time;
		}
		
		action_func_status = TaskExecuteActionFunc(task);
		TaskSetTimeToStart(task, current_time + TaskGetInterval(task));
		
		
		/* if after executing the action function the peek() returns a different task, the task removed itself from the queue */
		if (!UIDIsEqual(TaskGetUid(task), TaskGetUid(PQPeek(scheduler->tasks_priority_queue))))
		{
			TaskExecuteCleanFunc(task);
			DestroyTask(task);
		}
		
		else
		{
			task = PQDequeue(scheduler->tasks_priority_queue);
			
			/* action function return status -1 means function removes itself from the queue */
			if (action_func_status == -1)
			{
				TaskExecuteCleanFunc(task);
				DestroyTask(task);
			}
			else
			{			
				PQEnqueue(scheduler->tasks_priority_queue, task);
			}
		}
	}
	
	
	return 0;
}


void SchedulerStop(scheduler_t *scheduler)
{
	(void)(scheduler);
	is_scheduler_on = OFF;
}



void SchedulerClear(scheduler_t *scheduler)
{
	task_t *task = NULL;
	while (!SchedulerIsEmpty(scheduler))
	{
		task = PQDequeue(scheduler->tasks_priority_queue);
		TaskExecuteCleanFunc(task);
		DestroyTask(task);
	}
}



int SchedulerIsEmpty(const scheduler_t *scheduler)
{
	return PQIsEmpty(scheduler->tasks_priority_queue);
}


static int CompareItemPriority(const void *item, const void *data_to_compare)
{
	task_t *task1 = (task_t *)item;
	task_t *task2 = (task_t *)data_to_compare;
	
	size_t time_of_task1 = TaskGetTimeToStart(task1);
	size_t time_of_task2 = TaskGetTimeToStart(task2);
	
	
	return (time_of_task2 - time_of_task1);
}



static int MatchTask(const void *item, const void *data_to_compare)
{
 	task_t *task = (task_t *)item;
	ilrd_uid_t uid1 = TaskGetUid(task);
	
	ilrd_uid_t uid2 = *(ilrd_uid_t *)data_to_compare;
	
	
	return UIDIsEqual(uid1, uid2);
}




/* previous run() version: */

/*
int SchedulerRun(scheduler_t *scheduler)
{
	task_t *task;
	size_t time_until_task = 0;
	int action_func_status = 0;
	size_t current_time = 0;
	is_scheduler_on = ON;
	
	while (is_scheduler_on == ON && !SchedulerIsEmpty(scheduler))
	{
		task = PQDequeue(scheduler->tasks_priority_queue);
		current_time = time(NULL);
		time_until_task = TaskGetTimeToStart(task) - current_time;
		while (time_until_task > 0)
		{
			sleep(1);
			current_time = time(NULL);
			time_until_task = TaskGetTimeToStart(task) - current_time;
		}
		
		action_func_status = TaskExecuteActionFunc(task);
		TaskSetTimeToStart(task, current_time + TaskGetInterval(task));
		
		if (action_func_status == -1)
		{
			TaskExecuteCleanFunc(task);
			DestroyTask(task);
		}
		
		else
		{			
			PQEnqueue(scheduler->tasks_priority_queue, task);
		}
		
		
	}
	
	
	return 0;
}
*/
