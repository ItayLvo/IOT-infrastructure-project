#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "fixed_size_allocator.h"	/* fixed_size_allocator_t and FSA functions */

int main()
{
	size_t block_size = 8, block_count = 10;
	size_t suggested_size = FSASuggestSize(block_size, block_count);
	int *pool = malloc(suggested_size);
	int *test_ptr_1 = NULL;
	
	fixed_size_allocator_t * allocator = FSAInitialize(pool, block_size, suggested_size);
	
	printf("suggested = %ld\n", suggested_size);
	printf("countfree = %ld\n", FSACountFree(allocator));
	
	test_ptr_1 = FSAAlloc(allocator);
	*test_ptr_1 = 5;
	printf("%d\n", *test_ptr_1);
	printf("countfree = %ld\n", FSACountFree(allocator));
	
	FSAFree(allocator, test_ptr_1);
	printf("countfree = %ld\n", FSACountFree(allocator));
	
	free(pool);
	
	return 0;
}


