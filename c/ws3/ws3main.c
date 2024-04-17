#include <stdlib.h>	/*malloc*/
#include <stdio.h>	/*printf, scanf*/

#include "ws3.h"


int main(int argc, char *argv[], char *envp[])
{
	/* Testing the *envp[] exercise 
	TestEnvp(envp); */
	
	/* Testing the 2D array exercise 
	TestMatrix();*/
	
	/* Testing for Josephus 
	TestJosephus();*/
	
	return 0;
}


void TestEnvp(char *envp[])
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


void TestMatrix()
{
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
}

void TestJosephus()
{
	int n = 0, i = 0;
	int *arr = NULL;
	
	printf("Enter n: \n");
	scanf("%d", &n);
	arr = (int *)malloc(sizeof(int) * n);
	for (i = 0; i < n; i++)
	{
		arr[i] = 1;
	}
	
	printf("last soldier is: %d\n", josephus(arr, n));
	
	free(arr);
}
