#include <stdio.h>

int IsRotation(int *s1, int *s2, int size)
{
	int i = 0, j = 0;
	
	while (*s1 != *(s2+j) && j < size)
	{
		++j;
	}
	
	if (j == size)
	{
		return 0;
	}
	
	for (i = 0; i < size; ++i)
	{
		if (*(s1+i) != *(s2+(j+i) % size))
		{
			return 0;
		}
	}
	
	return 1;
}

int main()
{
	int arr1[] = {1,2,3,4,5};
	int arr2[] = {4,5,1,2,3};
	int size = sizeof(arr1) / sizeof(arr1[0]);
	printf("%d\n", IsRotation(arr1, arr2, size));
	return 0;
}
