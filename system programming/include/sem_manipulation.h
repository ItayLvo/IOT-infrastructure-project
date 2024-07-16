#ifndef __SEM_MANIPULATION_H__
#define __SEM_MANIPULATION_H__

typedef enum status
{
	SUCCESS = 0,
	USER_EXIT = 1,
	SEM_FAIL = 2
} sem_status_t;


int SemRun(const char *sem_name);

#endif /* __SEM_MANIPULATION_H__ */
