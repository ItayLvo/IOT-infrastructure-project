/*
Date: 
Status: 
Reviwer: 
*/


#include <stdio.h>	/* printf */
#include <stdlib.h>	/* exit */
#include <string.h> /* strstr, atoi, strcmp */
#include <fcntl.h> /* O_* constants (O_CREAT)*/
#include <semaphore.h>	/* POSIX semaphore functions and definitions */
#include <sys/stat.h>	/* permissions macros */

#include "sem_manipulation.h"

#define USER_INPUT_MAX_SIZE 64
#define INITIAL_SEM_VALUE 1
#define SEM_PERMISSIONS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) /* 0644 in octal */
	/*S_IRUSR: Read permission for the owner*/
	/*S_IWUSR: Write permission for the owner*/
	/*S_IRGRP: Read permission for the group*/
	/*S_IROTH: Read permission for others*/


/* forward declerations */
static void SemInit(const char *sem_name);
static int SemDecrement(char *user_input);
static int SemIncrement(char *user_input);
static int SemValue(void);
static int SemExit(void);


/* global semaphore primitive */
sem_t *g_sem;
static int sem_action_counter = 0;


int SemRun(const char *sem_name)
{
	char user_input[USER_INPUT_MAX_SIZE] = {0};
	int return_status = SUCCESS;
	
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
/*		else if (strcmp(user_input, "V") == 0)*/
		else if (user_input[0] == 'V')
		{
			return_status = SemValue();
		}
/*		else if (strcmp(user_input, "X") == 0)*/
		else if (user_input[0] == 'X')
		{
			return_status = SemExit();
		}
		else
		{
			printf("\nOptions:\nD number [undo]\nI number [undo]\nV\nX\n");
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
		if (sem_trywait(g_sem) != 0)
		{
			return SEM_FAIL;
		}
		--sem_value;
		--sem_action_counter;
	}
	
	sem_close(g_sem);
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
		if (sem_trywait(g_sem) != 0)
		{
			return SEM_FAIL;
		}
		--num_to_change;
	}

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
	g_sem = sem_open(sem_name, O_CREAT, SEM_PERMISSIONS, INITIAL_SEM_VALUE);
	if (g_sem == SEM_FAILED)
	{
		printf("semaphore init failed\n");
		exit(1);
	}
}
