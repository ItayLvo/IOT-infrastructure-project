/*
Date: 
Status: 
Reviwer: 
*/


/*DESCRIPTION*/
/*       sem_open()  creates  a  new  POSIX  semaphore or opens an existing semaphore.  The semaphore is*/
/*       identified by name.  For details of the construction of name, see sem_overview(7).*/


#include <stdio.h>	/* printf */
#include <stdlib.h>	/* exit */
#include <string.h> /* strstr, atoi, strcmp */
#include <fcntl.h> /* O_* constants (O_CREAT)*/
#include <semaphore.h>	/* POSIX semaphore functions and definitions */

#define USER_INPUT_MAX_SIZE 64

/* forward declerations */
static void SemInit(const char *sem_name);
static int SemDecrement(char *user_input);
static int SemIncrement(char *user_input);
static int SemValue(void);
static int SemExit(void);


/* global semaphore primitive */
sem_t *g_sem;
static int sem_action_counter = 0;

typedef enum status
{
	SUCCESS = 0,
	USER_EXIT = 1,
	SEM_FAIL = 2
} sem_status_t;


int SemRun(const char *sem_name)
{
	char user_input[USER_INPUT_MAX_SIZE] = {0};
	int return_status = 0;
	
	SemInit(sem_name);
	
	while (return_status == SUCCESS)
	{
	fgets(user_input, USER_INPUT_MAX_SIZE, stdin);
		
		if (strstr(user_input, "D ") != NULL)
		{
			return_status = SemDecrement(user_input);
		}
		else if (strstr(user_input, "I ") != NULL)
		{	
			return_status = SemIncrement(user_input);
		}
		else if (strcmp(user_input, "V"))
		{
			return_status = SemValue();
		}
		else if (strcmp(user_input, "X"))
		{
			return_status = SemExit();
		}
		else
		{
			printf("D number [undo]\nI number [undo]\nV\nX");
		}
		printf("\n");
	}
	
	return return_status;
}

static int SemExit(void)
{
	int sem_value = -1;
	if (sem_getvalue(g_sem, &sem_value) != 0)
	{
		return SEM_FAIL;
	}
	
	while (sem_value > 0 && sem_action_counter > 0)
	{
		if (sem_wait(g_sem) != 0)
		{
			return SEM_FAIL;
		}
		--sem_value;
		--sem_action_counter;
	}
	
	
	return USER_EXIT;
}

static int SemDecrement(char *user_input)
{
	int num_to_change = atoi(user_input + 2);
	
	if (strstr(user_input, " undo") != NULL)
	{
		sem_action_counter -= num_to_change;
	}
	
	
	while (num_to_change > 0)
	{
		if (sem_wait(g_sem) != 0)
		{
			return SEM_FAIL;
		}
		--num_to_change;
	}
	
/*	printf("num is %d, sem_action_counter = %d\n", num_to_change, sem_action_counter);*/

	return 0;
}




static int SemIncrement(char *user_input)
{
	int num_to_change = atoi(user_input + 2);
	
	if (strstr(user_input, " undo") != NULL)
	{
		sem_action_counter += num_to_change;
	}
	
	
	while (num_to_change > 0)
	{
		if (sem_post(g_sem) != 0)
		{
			return SEM_FAIL;
		}
		--num_to_change;
	}
	
	
	
/*	printf("num is %d, sem_action_counter = %d\n", num_to_change, sem_action_counter);*/

	return 0;
}



static int SemValue(void)
{
	int sem_value = -1;
	if (sem_getvalue(g_sem, &sem_value) != 0)
	{
		return SEM_FAIL;
	}
	
	printf("Semaphore value is: %d\n", sem_value);
	
	return 0;
}


static void SemInit(const char *sem_name)
{
	/* 0644 = read and write permissions to owner, and read-only to others */
	/* 1 is semaphore value */
	g_sem = sem_open(sem_name, O_CREAT, 0644, 1);
	if (g_sem == SEM_FAILED)
	{
		printf("semaphore init failed\n");
		exit(1);
	}
}
