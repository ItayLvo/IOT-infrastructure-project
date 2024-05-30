#ifndef __TASK_H__
#define __TASK_H__

#include <stddef.h> 	/* size_t */

#include "uid.h"	/* uid functions */

typedef struct task task_t;

task_t *CreateTask(ilrd_uid_t uid,
			scheduler_action_func_t scheduler_action_func,
			scheduler_clean_func_t scheduler_clean_func,
			void *param,
			size_t interval);

#endif /* __TASK_H__ */
