#include <stdio.h>	/*printf*/


int FindNum(int *array, int size, int num)
{
	int test = 1, i = 0;
	
	for (i = 0; i < size; ++i)
	{
		test *= (array[i]-num);
	}
	
	return (test == 0);
}


int main()
{
	int array[] = {6, 12, 3, 5, 1, 4, 9, 2};
	int size = sizeof(array) / sizeof(array[0]);
	
	printf("%d\n", FindNum(array,size, 3));

	return 0;
}



