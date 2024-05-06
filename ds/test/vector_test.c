#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/vector.h"

/* printf("%ld\n", ) */
int main()
{
	vector_t *vector;
	int x = 5;
	int i = 0;
	vector = VectorCreate(2,  sizeof(int));
	
	for (i = 0; i < 10; ++i)
	{
		printf("element count = %ld\n", VectorElementCount(vector));	
		printf("capacity = %ld\n", VectorCapacity(vector));
		VectorPushBack(vector, &x);
	}
	
	printf("\n");
	
	for (i = 0; i < 15; ++i)
	{
		printf("element count = %ld\n", VectorElementCount(vector));	
		printf("capacity = %ld\n", VectorCapacity(vector));
		VectorPopBack(vector);
	}
	
	VectorDestroy(vector);
	
	return 0;
}

