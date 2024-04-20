#include <stdlib.h>	/*malloc*/
#include <stdio.h>	/*printf, scanf*/

#include "ws3.h"


static void TestJosephus();
static void TestEnvp(char *envp[]);
static void TestMatrix();

int main(int argc, char *argv[], char *envp[])
{
	/* Testing the *envp[] exercise */
	TestEnvp(envp);
	
	/* Testing the 2D array exercise */
	TestMatrix();
	
	/* Testing for Josephus */
	TestJosephus();
	
	return 0;
}


static void TestEnvp(char *envp[])
{
	int i = 0;
	
	char **envstring = NULL;
	
	envstring = CopyEnv(envp, envstring);
	
	while ((NULL != envstring[i]))
	{
		envstring[i] = ToLowerString(envstring[i]);
		printf("%s\n",envstring[i]);
		i++;
	}
	printf("\n\n");
	
	i = 0;
	while ((NULL != envstring[i]))
	{	
		free(envstring[i]);
		i++;
	}
	
	free(envstring);
}


static void TestMatrix()
{
	int rows = 2, cols = 2;
	int i = 0;
	int arr[2] = {0};
	int *res = arr;
	int **mat = (int **)malloc(rows * sizeof(int *));
	
	for (i = 0; i < rows; i++)
	{
		mat[i] = (int *)malloc(cols * sizeof(int));
	}
	
	mat[0][0] = 1;
	mat[0][1] = 2;
	mat[1][0] = 3;
	mat[1][1] = 4;

	res = MatrixSum(mat, rows, cols, res);

	for (i = 0; i < sizeof(arr)/sizeof(arr[0]); i++)
	{
		printf("%d\n", res[i]);
	}	
	
	for (i = 0; i < rows; i++)
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
	
	for (i = 0; i < n; i++)
	{
		arr[i] = 1;
	}
	
	printf("last soldier is: %d\n", Josephus(arr, n));
	
	free(arr);
}
