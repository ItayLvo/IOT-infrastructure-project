#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "fixed_size_allocator.h"		/* FSA functions, typedefs */


struct fixed_size_allocator
{
	allocator_block_t *free_list_head;
	size_t block_size;
	size_t current_free_blocks;
	size_t pool_size; 
	void *free_list;
};

typedef struct 
{
    void *next;
} allocator_block_t;

fixed_size_allocator_t *FSAInitialize(void *memory_pool, size_t block_size, size_t pool_size)
{
	fixed_size_allocator_t *allocator = ( fixed_size_allocator_t *)memory_pool;
	size_t index = 0;
	char *block = NULL;
	assert(memory_pool);
	
	allocator->block_size = block_size;
	allocator->pool_size = pool_size;
	allocator->current_free_blocks = (pool_size - sizeof(fixed_size_allocator_t)) / block_size;
	allocator->free_list = (char *)allocator + sizeof(fixed_size_allocator_t);
	
	block = allocator->free_list;
	for (index = 0; index < allocator->current_free_blocks; ++index)
	{
		*((void **)block) = block + block_size;		/******* not sure at all... ********/
		block += block_size;
	}
	*((void **)block) = NULL;
	
	
	return allocator;
}



size_t FSASuggestSize(size_t block_size, size_t block_count)
{
	return (sizeof(fixed_size_allocator_t) + (block_count * block_size));
}
