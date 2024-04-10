#include "wsarrays.h"
#include <stdio.h>
#include <string.h> /*strlen*/
#include <stdlib.h> /*malloc*/


int main()
{

	/*
	
	testing the 2D array exercise
	
	int size = 2;
	int i;
	int mat[2][2] = {{1,2},{3,4}};
	int arr[10];
	int *res = arr;
	res = MatrixSum(size, mat, res);
	
	for (i = 0; i < size; i++)
	{
		printf("%d\n", res[i]);
	}
	*/
	int n, i;
	printf("Enter n: \n");
	scanf("%d", &n);
	int *arr = (int *)malloc(sizeof(int) * n);
	for (i = 0; i < n; i++)
	{
		arr[i] = 1;
	}
	
	/* int arr[6] = {1,1,1,1,1,1}; */
	printf("last soldier is: %d\n", josephus(arr, n));
	
	free(arr);
	
	return 0;
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



