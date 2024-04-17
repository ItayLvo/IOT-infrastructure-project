#include <stdio.h>	/*printf*/
#include <string.h>	/*strlen, strdup*/
#include <stdlib.h>	/*malloc*/
#include <stddef.h>	/*size_t*/
#include <ctype.h>	/*tolower*/
#include <assert.h>	/*assert*/

#include "ws3.h"



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
		count_strings++;
		i++;
	}
	
	
	envstring = (char **)malloc(sizeof(char *) * (count_strings + 1));
	if (envstring == NULL)
	{
		return NULL;
	}
	
	i = 0;
	while ((envp[i]) != NULL)
	{
		envstring[i] = (char *)malloc(sizeof(char) * (strlen(envp[i]) + 1));
		if(envstring[i] == NULL)
		{
			return NULL;
		}
		
		strcpy(envstring[i], envp[i]);
		
		i++;
	}
	envstring[i] = NULL;
	
	return envstring;
}



int *MatrixSum(int size, int mat[2][2], int *res)
{
	int i, j, countrow = 0;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			countrow += mat[i][j];
		}
		res[i]=countrow;
		countrow = 0;
	}
	return res;
}




int josephus(int arr[], int size)
{
	int i = 0, j = 0;
	int result;
	
	while(1)
	{
		if (arr[(i % size)] != 0)	/* GUI that prints status every iteration, can be deleted */
		{
			for (j = 0; j < size; j++)
				printf("[%d] ", arr[j]);
			printf("\n");
			printf("\tcurrent soldier with sword: %d\n", i % size);
			printf("\tgoing to kill soldier: %d\n\n", NextAliveIndex(arr, size, i % size));
		}
		
		if (arr[i % size] != 0)
		{
			arr[NextAliveIndex(arr, size, i % size)] = 0;
			result = NextAliveIndex(arr, size, i % size);
			if (result == -1)
			{
				return i % size;
			}
		}
		i++;	
	}
}

int NextAliveIndex(int arr[], int size, int index)
{
	int i = 0;
	int countsteps = 0;
	
	while (arr[(index + i + 1) % size] != 1 && countsteps < size)
	{
		countsteps++;
		i++;
	}
	
	if (countsteps == (size - 1))
	{
		return -1;
	}
	else
	{
		return ((index + i + 1) % size);
	}
}



