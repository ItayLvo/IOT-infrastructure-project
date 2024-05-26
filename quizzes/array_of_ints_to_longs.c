#include <stdio.h>

long *ArrayOfIntsToLongs(int ints[], size_t numOfElements);


int main(void)
{
	int ints[] = {1, 6, 456, -3, 8, 12};
	long *longs = NULL;
	longs = ArrayOfIntsToLongs(ints, 6);
	
	printf("%ld, %ld, %ld\n", longs[0], longs[1], longs[2]);
	
	return 0;
}


long *ArrayOfIntsToLongs(int ints[], size_t numOfElements)
{
	size_t i = 0, j = 0;
	long tmp_sum = 0;
	long *longs = (long *)ints;
	
	for(i = 0; i < numOfElements; ++i)
	{
		tmp_sum += ints[i];
		printf("tmpsum = %ld\n", tmp_sum);
		if (i % 2 == 1)
		{
			printf("i = %d\n", i);
			longs[j] = tmp_sum;
			++j;
			tmp_sum = 0;
		}
	}
	
	return longs;
}

