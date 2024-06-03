#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "fixed_size_allocator.h"		/* FSA functions, typedefs */

#define WORD_SIZE 8

struct fixed_size_allocator
{
	void *free_list;
};

static size_t AlignBlockSize(size_t block_size);


fixed_size_allocator_t *FSAInitialize(void *memory_pool, size_t block_size, size_t pool_size)
{
	fixed_size_allocator_t *allocator = (fixed_size_allocator_t *)memory_pool;
	size_t index = 0;
	char *start_of_blocks = NULL;
	char *current_block = NULL;
	size_t total_blocks = 0;
	
	assert(memory_pool);
	
	block_size = AlignBlockSize(block_size);
	
	total_blocks = (pool_size - sizeof(fixed_size_allocator_t)) / block_size;
	
	start_of_blocks = (char *)allocator + sizeof(fixed_size_allocator_t);
	allocator->free_list = (void *)start_of_blocks;
	
	current_block = start_of_blocks;
	for (index = 0; index < total_blocks - 1; ++index)
	{
		*(void **)current_block = current_block + block_size;
		current_block += block_size;
	}
	*(void **)current_block = NULL;
	
	return allocator;
}


void *FSAAlloc(fixed_size_allocator_t *allocator)
{
	void *allocated_block = NULL;
	
	assert(allocator);
	if (allocator->free_list == NULL)
	{
		return NULL;
	}
	
	allocated_block = allocator->free_list;
	allocator->free_list = *(void **)(allocator->free_list);

	return allocated_block;
}

void FSAFree(fixed_size_allocator_t *allocator, void *mem_to_free_ptr)
{
	*(void **)mem_to_free_ptr = allocator->free_list;
	
	allocator->free_list = mem_to_free_ptr;
}


size_t FSACountFree(fixed_size_allocator_t *allocator)
{
	void *runner = allocator->free_list;
	size_t count = 0;
	
	while (NULL != runner)
	{
		runner = *(void **)runner;
		++count;
	}
	
	return count;
}

size_t FSASuggestSize(size_t block_size, size_t block_count)
{
	block_size = AlignBlockSize(block_size);
	
	return (sizeof(fixed_size_allocator_t) + (block_count * block_size));
}




static size_t AlignBlockSize(size_t block_size)
{
	if (block_size < sizeof(void *))
	{
			block_size = sizeof(void *);
			return block_size;
	}
	
	if (block_size % WORD_SIZE == 0)
	{
		return block_size;
	}
	
	block_size = (((block_size / WORD_SIZE) + 1) * WORD_SIZE);
	
	return block_size;
}
