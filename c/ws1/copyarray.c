#include "copyarray.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	int arr[5] = {0,1,2,3,4};
	int *newarr = CopyArray(arr, 5);
	printf("%d \n", newarr[3]);
	
	free(newarr);
	
	return 0;
}

int *CopyArray(int *arr, int size)
{
	int i;
	int *newarr = malloc(size * sizeof(arr[0]));
	
	if (newarr == NULL)
		return NULL;
		
	for (i = 0; i < size; i++)
	{
		newarr[i] = arr[i];
	}
	
	return newarr;
}
