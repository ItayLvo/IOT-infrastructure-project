#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/vector.h"

/* printf("%ld\n", ) */
int main()
{
	vector_t *vector;
	int x = 5, y = 0;
	int i = 0;
	int *int_ptr = &x, *int_ptr2 = &y;
	vector = VectorCreate(2,  sizeof(int));
	
	for (i = 0; i < 10; ++i)
	{
		printf("element count = %ld\n", VectorElementCount(vector));	
		printf("capacity = %ld\n", VectorCapacity(vector));
		
		VectorPushBack(vector, &x);
		
		int_ptr = VectorAccessVal(vector, i);
		printf("accessval = %d\n", *int_ptr);
	}
	printf("\n\n");
	
	*int_ptr2 = 999;
	*(int *)VectorAccessVal(vector, VectorElementCount(vector) - 1) = *int_ptr2;
	printf("accessval = %d\n", *(int *)VectorAccessVal(vector, VectorElementCount(vector) - 1));
		
	printf("shrink:\n");
	VectorShrink(vector);
	printf("element count = %ld\n", VectorElementCount(vector));	
	printf("capacity = %ld\n", VectorCapacity(vector));
	
	VectorPushBack(vector, &x);
	printf("element count = %ld\n", VectorElementCount(vector));	
	printf("capacity = %ld\n", VectorCapacity(vector));
	printf("\n");
	
	printf("reserve:\n");
	VectorReserve(vector, 60);
	printf("element count = %ld\n", VectorElementCount(vector));	
	printf("capacity = %ld\n", VectorCapacity(vector));
	printf("\n");
	VectorPushBack(vector, &x);
	printf("element count = %ld\n", VectorElementCount(vector));	
	printf("capacity = %ld\n", VectorCapacity(vector));
	printf("\n");
	
	printf("shrink:\n");
	VectorShrink(vector);
	printf("element count = %ld\n", VectorElementCount(vector));	
	printf("capacity = %ld\n", VectorCapacity(vector));
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

