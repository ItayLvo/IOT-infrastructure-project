/*
name: itay lvovich
reviewer: vova
status: added fixes after cr
*/

#include <stdio.h>	/*printf*/
#include <string.h>	/*strlen, strdup*/
#include <stdlib.h>	/*malloc*/
#include <stddef.h>	/*size_t*/
#include <ctype.h>	/*tolower*/
#include <assert.h>	/*assert*/

#include "ws3.h"


static int NextAliveIndex(int arr[], int size, int index);

static char *ToLowerString(char *str)
{
	int i = 0;
	
	for (i = 0; str[i] != '\0'; ++i)
	{
		str[i] = tolower(str[i]);
	}
	
	return str;
}

void CopyAndToLowerEnvp(char *envp[])
{
	int i = 0;
	char **env_string = NULL;
	
	env_string = CopyEnv(envp, env_string);
	
	while ((NULL != env_string[i]))
	{
		env_string[i] = ToLowerString(env_string[i]);
		printf("%s\n",env_string[i]);
		++i;
	}
	printf("\n\n");
	
	i = 0;
	while ((NULL != env_string[i]))
	{	
		free(env_string[i]);
		++i;
	}
	free(env_string);
}


char **CopyEnv(char **envp, char **env_string)
{
	int i = 0;
	char **ptrenv = envp;
	int count_strings = 0;
	
	while ((ptrenv[i]) != NULL)
	{
		++count_strings;
		++i;
	}
	
	env_string = (char **)malloc(sizeof(char *) * (count_strings + 1));
	
	if (NULL == env_string)
	{
		return NULL;
	}
	
	i = 0;
	while ((envp[i]) != NULL)
	{
		env_string[i] = (char *)malloc(sizeof(char) * (strlen(envp[i]) + 1));
		if(NULL == env_string[i])
		{
			int index_mallocd = 0;
			for (index_mallocd = 0; index_mallocd < i; index_mallocd++)
			{
				free(env_string[index_mallocd]);
			}
			return NULL;
		}
		
		strcpy(env_string[i], envp[i]);
		++i;
	}
	env_string[i] = NULL;
	
	return env_string;
}



int *MatrixSum(int **mat, int rows, int cols, int *res)
{
	int i = 0, j = 0, count_row = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			count_row += mat[i][j];
		}
		res[i]=count_row;
		count_row = 0;
	}
	
	return res;
}



int Josephus(int arr[], int size)
{
	int i = 0;
	int result = 0;
	int next_alive_index = 0;
	
	while(-1 != result)
	{
		if (arr[i % size] != 0)
		{
			next_alive_index = NextAliveIndex(arr, size, i % size);
			
			if (-1 == next_alive_index)
			{
				return i % size;
			}
			else
			{
				arr[next_alive_index] = 0;
			}
		}
		i++;	
	}
	
	return i % size;
}

static int NextAliveIndex(int arr[], int size, int index)
{
	int i = 0;
	int count_steps = 0;
	
	while (arr[(index + i + 1) % size] != 1 && count_steps < size)
	{
		++count_steps;
		++i;
	}
	
	if (count_steps == (size - 1))
	{
		return -1;
	}
	
	return ((index + i + 1) % size);
}



