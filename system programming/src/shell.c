/*
Date: 
Status: 
Reviwer: 
*/

#include <stddef.h>	/* size_t */	/*??*/
#include <stdlib.h>	/* malloc */	/*??*/
#include <assert.h>	/* assert */
#include <string.h>	/* strstr, strcspn */
#include <stdio.h>	/* printf */
#include <sys/types.h>	/* pid_t */
#include <unistd.h>		/* waitpid, fork */
#include <sys/wait.h>	/* waitpid */

#include "shell.h"

#define USER_INPUT_MAX_SIZE 128
#define NUM_MAX_ARGS 32
#define SYSTEM "system"
#define FORK "fork"


typedef enum status
{
	SUCCESS = 0,
	EXIT = 1
} shell_status_t;

static int RunSystem(char *user_input);
static int RunFork(char *user_input);


int Run(void)
{
	char user_input[USER_INPUT_MAX_SIZE] = {0};
	int return_status = 0;
	
	while (return_status == SUCCESS)
	{
		printf("$>");
		fgets(user_input, USER_INPUT_MAX_SIZE, stdin);
		
		if (strstr(user_input, SYSTEM) != NULL)
		{
			return_status = RunSystem(user_input);
		}
		else if (strstr(user_input, FORK) != NULL)
		{	
			return_status = RunFork(user_input);
		}
		else if (strstr(user_input, "exit") != NULL)
		{
			return 0;
		}
		else
		{
			printf("Use 'fork' or 'system' to run command.");
		}
		printf("\n");
	}
	
	return return_status;
}



static int RunSystem(char *user_input)
{
	int return_value = 0;
	
	*(strstr(user_input, " system")) = '\0';
	
	return_value = system(user_input);

	return return_value;
}


static int RunFork(char *user_input)
{
	pid_t child_pid = 0;
	char *arg_list[NUM_MAX_ARGS] = {0};	/* array of strings */
	char **runner = arg_list;
	char space[2] = " ";
	char *token = NULL;
	int child_return_status = 0;
	
	/* remove "fork" from user input string and null terminate it */
	*(strstr(user_input, " fork")) = '\0';
	
	/* initialize argument list for exec() */
	token = strtok(user_input, space);
	while(token != NULL)
	{
		*runner = token;
		++runner;
		token = strtok(NULL, space);
	}
	*runner = NULL;

	child_pid = fork();
	
	if (child_pid != 0)		/* in parent process */
	{
		printf("in parent process. child pid = %d\n", child_pid);
		waitpid(child_pid, &child_return_status, 0);
	}
	else					/* in child process */
	{
		printf("in child process. child pid = %d\n", child_pid);
		child_return_status = execvp(arg_list[0], arg_list);
		printf("%s failed. Error code: %d\n",arg_list[0], WEXITSTATUS(child_return_status));
		return WEXITSTATUS(child_return_status);
	}

	return child_return_status;
}


