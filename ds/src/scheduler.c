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

static int CompareTaskPriority(const void *item, const void *data_to_compare);
static int MatchTask(const void *item, const void *data_to_compare);

enum scheduler_status_t {SCHEDULER_OFF, SCHEDULER_ON};
/* static variable for monitoring status of scheduler on/off */
/* if there is more than 1 instance of scheduler, they are all on/off according to this single static var */ 
static int is_scheduler_on = SCHEDULER_OFF;


scheduler_t *SchedulerCreate(void)
{
	scheduler_t *scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == scheduler)
	{
		return NULL;
	}
	
	scheduler->tasks_priority_queue = PQCreate(CompareTaskPriority);
	
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
	if (NULL == erased_task)
	{
		return 1;
	}
	
	TaskExecuteCleanFunc(erased_task);
	
	return 0;
}


/* a-synchronic stop pseudo-code: */
/* fopen (which file? if sent by user, need to change API of Run/Create) before outer while loop */	
/* fread before entering inner while loop */
/* if stop signal exists in file => stop the scheduler */
/* fread after each sleep(1) iteration */
/* if stop signal exists in file => stop the scheduler */
/* fclose before return statements */
int SchedulerRun(scheduler_t *scheduler)
{
	task_t *task = NULL;
	size_t time_until_task = 0;
	int action_func_status = 0;
	size_t current_time = 0;
	is_scheduler_on = SCHEDULER_ON;
	
	while (is_scheduler_on == SCHEDULER_ON && !SchedulerIsEmpty(scheduler))
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
		

		/* if after executing the action function the peek() returns a different task, that means the task removed itself from the queue */
		if (!UIDIsEqual(TaskGetUid(task), TaskGetUid(PQPeek(scheduler->tasks_priority_queue))))
		{
			TaskExecuteCleanFunc(task);
			DestroyTask(task);
		}
		else
		{
			task = PQDequeue(scheduler->tasks_priority_queue);
			
			/* if action function return status is -1, that means the function opts-out from the queue */
			if (action_func_status == -1)
			{
				TaskExecuteCleanFunc(task);
				DestroyTask(task);
			}
			/* if the function didn't remove itself from the queue and didnt opt-out, update function time for activation and re-enqueue */
			else
			{	
				TaskSetTimeToStart(task, current_time + TaskGetInterval(task));
				PQEnqueue(scheduler->tasks_priority_queue, task);
			}
		}
	}
	
	if (is_scheduler_on == SCHEDULER_OFF)
	{
		return 1;
	}
	
	return 0;
}


void SchedulerStop(scheduler_t *scheduler)
{
	(void)(scheduler);
	is_scheduler_on = SCHEDULER_OFF;
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


static int CompareTaskPriority(const void *item, const void *data_to_compare)
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


