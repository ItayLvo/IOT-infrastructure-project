#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "uid.h"		/* uid functions */
#include "task.h"		/* task_t functions */
#include "scheduler.h"		/* ilrd_scheduler_t functions */



struct task
{
	ilrd_uid_t uid;
	void *data;
	size_t interval;
	size_t time_to_start;
	scheduler_action_func_t scheduler_action_func;
	scheduler_clean_func_t scheduler_clean_func;
};


task_t *CreateTask(scheduler_action_func_t scheduler_action_func,
			scheduler_clean_func_t scheduler_clean_func,
			void *param,
			size_t interval)
{
	task_t *task = (task_t *)malloc(sizeof(task_t));
	if (NULL == task)
	{
		return NULL;
	}
	task->uid = UIDCreate();
	task->scheduler_action_func = scheduler_action_func;
	task->scheduler_clean_func = scheduler_clean_func;
	task->data = param;
	task->interval = interval;
	task->time_to_start = task->uid.timestamp + task->interval;

	return task;
}


void DestroyTask(task_t *task)
{
	free(task);
}


size_t TaskGetTimeToStart(task_t *task)
{
	return task->time_to_start;
}


void TaskSetTimeToStart(task_t *task, size_t time)
{
	task->time_to_start = time;
}

size_t TaskGetInterval(task_t *task)
{
	return task->interval;
}

ilrd_uid_t TaskGetUid(task_t *task)
{
	return task->uid;
}

size_t TaskGetTimeUntilTask(task_t *task)
{
	return task->time_to_start;
}

int TaskExecuteActionFunc(task_t *task)
{
	int status = task->scheduler_action_func(task->data);
	return status;
}

int TaskExecuteCleanFunc(task_t *task)
{
	int status = task->scheduler_clean_func();
	return status;
}


