#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <assert.h>	/* assert */
#include <unistd.h>	/* sleep */

#include "uid.h"		/* uid_t functions */
#include "task.h"		/* task_t functions */
#include "priority_queue.h"	/* pq_t functions */
#include "scheduler.h"		/* scheduler_t functions */

enum scheduler_status_t {SCHEDULER_OFF, SCHEDULER_ON};
enum action_func_status {ACTION_FUNC_SUCCESS, ACTION_FUNC_FAILURE, ACTION_FUNC_REMOVE_ME};
enum run_function_status {RUN_FUNC_ENDED = 0, RUN_FUNC_STOPPED = 1, RUN_FUNC_ERROR = 2};

struct scheduler
{
    pq_t *tasks_priority_queue;
    enum scheduler_status_t is_scheduler_on;
};

static int CompareTaskPriority(const void *item, const void *data_to_compare);
static int MatchTask(const void *item, const void *data_to_compare);


scheduler_t *SchedulerCreate(void)
{
	scheduler_t *scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == scheduler)
	{
		return NULL;
	}
	
	scheduler->tasks_priority_queue = PQCreate(CompareTaskPriority);
	if (NULL == scheduler->tasks_priority_queue)
	{
		free(scheduler);
		return NULL;
	}
	
	return scheduler;
}


void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(scheduler);
	
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
	assert(scheduler);
	
	task = CreateTask(action_func, clean_func,
				action_param, time_interval);
				
	if (NULL == task)
	{
		return UIDGetBad();
	}
	
	uid = TaskGetUid(task);
	if (UIDIsEqual(UIDGetBad(), uid))
	{
		return UIDGetBad();
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
	task_t *erased_task = NULL;
	assert(scheduler);
	
	erased_task = PQErase(scheduler->tasks_priority_queue, MatchTask, &task_uid);
	if (NULL == erased_task)
	{
		return 1;
	}
	
	TaskExecuteCleanFunc(erased_task);
	
	return 0;
}



int SchedulerRun(scheduler_t *scheduler)
{
	task_t *task = NULL;
	size_t time_until_task = 0;
	int action_func_status = 0;
	size_t current_time = 0;
	scheduler->is_scheduler_on = SCHEDULER_ON;
	assert(scheduler);
	
	while (scheduler->is_scheduler_on == SCHEDULER_ON && !SchedulerIsEmpty(scheduler))
	{
		task = PQDequeue(scheduler->tasks_priority_queue);
		current_time = time(NULL);
		if (-1 == current_time)
		{
			return RUN_FUNC_ERROR;
		}
		
		time_until_task = TaskGetTimeToStart(task) - current_time;
		
		/* sleep(time_until_task) can end before time_until_task seconds! */
		while (time_until_task > 0)
		{
			sleep(time_until_task);
			current_time = time(NULL);
			if (-1 == current_time)
			{
				return RUN_FUNC_ERROR;
			}
			time_until_task = TaskGetTimeToStart(task) - current_time;
		}
		
		action_func_status = TaskExecuteActionFunc(task);

		if (action_func_status == ACTION_FUNC_REMOVE_ME)
		{
			TaskExecuteCleanFunc(task);
			DestroyTask(task);
		}
		else
		{
			TaskSetTimeToStart(task, current_time + TaskGetInterval(task));
			if (NULL == PQEnqueue(scheduler->tasks_priority_queue, task))
			{
				return RUN_FUNC_ERROR;
			}
		}
	}
	
	if (scheduler->is_scheduler_on == SCHEDULER_OFF)
	{
		return RUN_FUNC_STOPPED;
	}
	
	return RUN_FUNC_ENDED;
}


void SchedulerStop(scheduler_t *scheduler)
{
	assert(scheduler);	
	scheduler->is_scheduler_on = SCHEDULER_OFF;
}



void SchedulerClear(scheduler_t *scheduler)
{
	task_t *task = NULL;
	assert(scheduler);
	
	while (!SchedulerIsEmpty(scheduler))
	{
		task = PQDequeue(scheduler->tasks_priority_queue);
		TaskExecuteCleanFunc(task);
		DestroyTask(task);
	}
}



int SchedulerIsEmpty(const scheduler_t *scheduler)
{
	assert(scheduler);
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


