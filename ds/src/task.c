#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "uid.h"		/* uid functions */
#include "priority_queue.h"	/* pq_t functions */ /* needed ????????????? */



struct task
{
	ilrd_uid_t uid;
	void *data;
	size_t interval;
	size_t time_to_start;
	scheduler_action_func_t scheduler_action_func;
	scheduler_clean_func_t scheduler_clean_func;
};


task_t *CreateTask(ilrd_uid_t uid,
			scheduler_action_func_t scheduler_action_func,
			scheduler_clean_func_t scheduler_clean_func,
			void *param,
			size_t interval)
{
	task_t *task;
	
	task->uid = uid;
	task->scheduler_action_func = scheduler_action_func;
	task->scheduler_clean_func = scheduler_clean_func;
	task->data = param;
	task->interval = interval;
	task->time_to_start = 0;

	return task;
}
