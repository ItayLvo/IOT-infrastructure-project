#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "variable_size_allocator.h"	/* fixed_size_allocator_t and FSA functions */



int main()
{
	
	int *px = malloc(128);
	int *p_test1 = NULL;
	int *p_test2 = NULL;
	int *p_test3 = NULL;
	vsa_t *allocator;
	
	printf("px: %p\n", px);
	
	allocator = VSAInitialize(px, 128);
	printf("allocator: %p\n", allocator);
	
	p_test1 = VSAAlloc(allocator, 80);
	printf("p_test1: %p\n", p_test1);
	
	p_test2 = VSAAlloc(allocator, 5);
	printf("p_test2: %p\n", p_test2);
	
	VSAFree(p_test1);
	VSAFree(p_test2);
	
	p_test3 = VSAAlloc(allocator, 30);
	printf("p_test3: %p\n", p_test3);
	
	free(px);
	return 0;
}
