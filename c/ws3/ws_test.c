#include <stdlib.h>	/*malloc*/
#include <stdio.h>	/*printf, scanf*/

#include "ws3.h"


static void TestJosephus();
static void TestMatrix();

int main(int argc, char *argv[], char *envp[])
{
	/* Testing the *envp[] exercise */
	CopyAndToLowerEnvp(envp);
	
	/* Testing the 2D array exercise */
	TestMatrix();
	
	/* Testing for Josephus */
	TestJosephus();
	
	return 0;
} 


static void TestMatrix()
{
	int rows = 2, cols = 2;
	int i = 0;
	int arr[2] = {0};
	int *res = arr;
	int **mat = (int **)malloc(rows * sizeof(int *));
	if (NULL == mat)
	{
		return;
	}
	
	for (i = 0; i < rows; i++)
	{
		mat[i] = (int *)malloc(cols * sizeof(int));
		if (NULL == mat[i])
		{
			int j = 0;
			for (j = 0; j < i; ++j)
			{
				free(mat[i]);
			}
			free(mat);
			return;
		}
	}
	
	mat[0][0] = 1;
	mat[0][1] = 2;
	mat[1][0] = 3;
	mat[1][1] = 4;

	res = MatrixSum(mat, rows, cols, res);

	for (i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i)
	{
		printf("%d\n", res[i]);
	}	
	
	for (i = 0; i < rows; ++i)
	{
		free(mat[i]);
	}
	
	free(mat);
}


static void TestJosephus()
{
	int n = 0, i = 0;
	int *arr = NULL;

	printf("Enter n: \n");
	scanf("%d", &n);
	arr = (int *)malloc(sizeof(int) * n);
	if (arr == NULL)
	{
		return;
	}
	
	for (i = 0; i < n; ++i)
	{
		arr[i] = 1;
	}
	
	printf("last soldier is: %d\n", Josephus(arr, n));
	
	free(arr);
}
