#include "wsarrays.h"
#include <stdio.h>
#include <string.h> /*strlen*/


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
	int arr[6] = {1,1,1,1,1,1};
	printf("last soldier is: %d\n", josephus(arr, 6));
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
	int i = 0, result;
	
	while(1)
	{
		if (arr[(i % size)] != 0)
		{
			printf("[%d] [%d] [%d] [%d] [%d] [%d]\n", arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
			printf("\tcurrent soldier with sword: %d\n", i % size);
			printf("\tgoing to kill soldier: %d\n\n", NextAliveIndex(arr, size, i % size));
		}
		if (arr[i % size] == 0)
		{
			;
		}
		
		else
		{
			/* printf("(before killing) next alive soldier is in index: %d\n", NextAliveIndex(arr, size, (i % size))); */
			arr[NextAliveIndex(arr, size, i % size)] = 0;
			/* printf("after killing: [%d] [%d] [%d] [%d] [%d]\n", arr[0], arr[1], arr[2], arr[3], arr[4]);*/
			/* printf("(after killing) next alive soldier is in index: %d\n", NextAliveIndex(arr, size, (i % size)));*/
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



