#include "wsarrays.h"
#include <stdio.h>
#include <string.h>	/*strlen, strdup*/
#include <stdlib.h>	/*malloc*/
#include <stddef.h>
#include <ctype.h>	/*tolower*/
#include <assert.h>	/*assert*/



static int countmoves; /* counts runtime complexity of josephus, can be deleted */


int main(int argc, char *argv[], char *envp[])
{
	int i = 0;
	char **envstring;
	envstring = CopyEnv(envp, envstring);
	
	
	while ((envstring[i]) != NULL)
	{
		envstring[i] = ToLowerString(envstring[i]);
		printf("%s\n",envstring[i]);
		i++;
	}
	printf("\n\n\n");
	
	i = 0;
	while ((envstring[i]) != NULL)
	{	
		printf("%s\n",envstring[i]);
		free(envstring[i]);
		i++;
	}
	
	free(envstring);
	
	/*
	**** testing the 2D array exercise ****
	int size = 2;
	int i;
	int mat[2][2] = {{1,2},{3,4}};
	int arr[2];
	int *res = arr;
	res = MatrixSum(size, mat, res);
	
	for (i = 0; i < size; i++)
	{
		printf("%d\n", res[i]);
	}
	*/
	
	
	
	/*
	**** testing for josephus ****
	int n, i;
	printf("Enter n: \n");
	scanf("%d", &n);
	int *arr = (int *)malloc(sizeof(int) * n);
	for (i = 0; i < n; i++)
	{
		arr[i] = 1;
	}
	
	printf("last soldier is: %d\n", josephus(arr, n));
	printf("num of moves: %d\n",  countmoves);
	
	free(arr);
	*/
	
	
	return 0;
}

char *ToLowerString(char *str)
{
	int i;
	for(i = 0; str[i] != '\0'; i++)
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
	
	
	envstring = (char **)malloc(sizeof(char *) * count_strings + 1);
	assert(envstring);
	
	i = 0;
	while ((envp[i]) != NULL)
	{
		envstring[i] = (char *)malloc(sizeof(char) * strlen(envp[i]) + 1);
		assert(envstring[i]);
		
		strcpy(envstring[i], envp[i]);
		/* envstring[i] = envp[i]; */
		/* envstring[i] = strdup(envp[i]); */
		
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
	int i = 0, result, j;
	
	while(1)
	{
		if (arr[(i % size)] != 0)
		{
			for (j = 0; j < size; j++)
				printf("[%d] ", arr[j]);
			printf("\n");
			printf("\tcurrent soldier with sword: %d\n", i % size);
			printf("\tgoing to kill soldier: %d\n\n", NextAliveIndex(arr, size, i % size));
		}
		
		
		if (arr[i % size] == 0)
		{
			;
			countmoves++;
		}
		
		else
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
		countmoves++;
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



