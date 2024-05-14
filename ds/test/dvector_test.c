#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/dvector.h"

int main()
{
	vector_t *vector;
	int x = 5, y = 0;
	int i = 0;
	int *int_ptr2 = &y;
	vector = VectorCreate(2,  sizeof(int));
	
	printf("element count = %ld\n", VectorElementCount(vector));	
	printf("capacity = %ld\n", VectorCapacity(vector));
	
	for (i = 0; i < 10; ++i)
	{
		VectorPushBack(vector, &x);
		printf("element count = %ld\n", VectorElementCount(vector));	
		printf("capacity = %ld\n", VectorCapacity(vector));
	}
	printf("\n\n");
	
	printf("AccessVal test:\n");
	printf("accessval = %d\n\n", *(int *)VectorAccessVal(vector, VectorElementCount(vector) - 1));
	
	printf("AccessVal test: setting vector[element_count] to 999\n");
	*int_ptr2 = 999;
	*(int *)VectorAccessVal(vector, VectorElementCount(vector) - 1) = *int_ptr2;
	printf("accessval = %d\n\n", *(int *)VectorAccessVal(vector, VectorElementCount(vector) - 1));
		
	printf("shrink test:\n");
	VectorShrink(vector);
	printf("element count = %ld\n", VectorElementCount(vector));	
	printf("capacity = %ld\n\n", VectorCapacity(vector));
	
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

