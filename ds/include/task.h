#ifndef __TASK_H__
#define __TASK_H__

#include <stddef.h> 	/* size_t */

#include "uid.h"	/* uid functions */
#include "scheduler.h"		/* ilrd_scheduler_t functions */

typedef struct task task_t;

task_t *CreateTask(scheduler_action_func_t scheduler_action_func,
			scheduler_clean_func_t scheduler_clean_func,
			void *param,
			size_t interval);
void DestroyTask(task_t *task);
int TaskExecuteActionFunc(task_t *task);
int TaskExecuteCleanFunc(task_t *task);
size_t TaskGetTimeToStart(task_t *task);
void TaskSetTimeToStart(task_t *task, size_t time);
size_t TaskGetTimeUntilTask(task_t *task);
ilrd_uid_t TaskGetUid(task_t *task);
size_t TaskGetInterval(task_t *task);

#endif /* __TASK_H__ */
