#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "variable_size_allocator.h"	/* fixed_size_allocator_t and FSA functions */



int main()
{
	/*
	int *px = malloc(128);
	int *p_test = NULL;
	
	vsa_t *allocator;
	allocator = VSAInitialize(px, 128);
	printf("%ld\n" ,VSALargestChunkAvailable(allocator));
	p_test = VSAAlloc(allocator, 80);
	printf("%ld\n" ,VSALargestChunkAvailable(allocator));
	VSAFree(p_test);
	printf("%ld\n" ,VSALargestChunkAvailable(allocator));
	*/
	return 0;
}
