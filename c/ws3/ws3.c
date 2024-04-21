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

char *ToLowerString(char *str)
{
	int i = 0;
	
	for (i = 0; str[i] != '\0'; ++i)
	{
		str[i] = tolower(str[i]);
	}
	
	return str;
}

char **CopyEnv(char **envp, char **envstring)
{
	int i = 0;
	char **ptrenv = envp;
	int count_strings = 0;
	
	while ((ptrenv[i]) != NULL)
	{
		++count_strings;
		++i;
	}
	
	envstring = (char **)malloc(sizeof(char *) * (count_strings + 1));
	
	if (NULL == envstring)
	{
		return NULL;
	}
	
	i = 0;
	while ((envp[i]) != NULL)
	{
		envstring[i] = (char *)malloc(sizeof(char) * (strlen(envp[i]) + 1));
		if(NULL == envstring[i])
		{
			int index_mallocd = 0;
			for (index_mallocd = 0; index_mallocd < i; index_mallocd++)
			{
				free(envstring[index_mallocd]);
			}
			return NULL;
		}
		
		strcpy(envstring[i], envp[i]);
		++i;
	}
	envstring[i] = NULL;
	
	return envstring;
}



int *MatrixSum(int **mat, int rows, int cols, int *res)
{
	int i = 0, j = 0, countrow = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			countrow += mat[i][j];
		}
		res[i]=countrow;
		countrow = 0;
	}
	
	return res;
}



int Josephus(int arr[], int size)
{
	int i = 0, j = 0;
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
	int countsteps = 0;
	
	while (arr[(index + i + 1) % size] != 1 && countsteps < size)
	{
		++countsteps;
		++i;
	}
	
	if (countsteps == (size - 1))
	{
		return -1;
	}
	
	return ((index + i + 1) % size);
}



