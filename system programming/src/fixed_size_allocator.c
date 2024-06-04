#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "fixed_size_allocator.h"		/* FSA functions, typedefs */

#define WORD_SIZE sizeof(void *)

struct fixed_size_allocator
{
	size_t free_list_offset;
};

static size_t AlignBlockSize(size_t block_size);


fixed_size_allocator_t *FSAInitialize(void *memory_pool, size_t block_size, size_t pool_size)
{
	fixed_size_allocator_t *allocator = (fixed_size_allocator_t *)memory_pool;
	size_t index = 0;
	size_t total_blocks = 0;
	size_t *current_block = NULL;
	size_t offset = 0;
	
	assert(memory_pool);
	
	block_size = AlignBlockSize(block_size);
	total_blocks = (pool_size - sizeof(fixed_size_allocator_t)) / block_size;
	
	allocator->free_list_offset = sizeof(fixed_size_allocator_t);
	
	offset = allocator->free_list_offset;
	current_block = (size_t *)((char *)allocator + offset);
	
	for (index = 0; index < total_blocks - 1; ++index)
	{
		offset += block_size;
		*current_block = offset;
		current_block = (size_t *)((char *)allocator + offset);
	}
	*current_block = 0;
	
	return allocator;
}


void *FSAAlloc(fixed_size_allocator_t *allocator)
{
	size_t *allocated_block = NULL;
	
	assert(allocator);
	if (0 == allocator->free_list_offset)
	{
		return NULL;
	}
	
	allocated_block = (void *)((char *)allocator + allocator->free_list_offset);
	allocator->free_list_offset = *allocated_block;

	return allocated_block;
}

void FSAFree(fixed_size_allocator_t *allocator, void *mem_to_free_ptr)
{
	size_t *ptr_to_free_addrs = (size_t *)mem_to_free_ptr;
	*ptr_to_free_addrs = allocator->free_list_offset;
	
	allocator->free_list_offset = (size_t)ptr_to_free_addrs - (size_t)allocator;
}


size_t FSACountFree(fixed_size_allocator_t *allocator)
{
	size_t current_offset = allocator->free_list_offset;
	size_t count = 0;
	
	while (0 != current_offset)
	{
		current_offset = *(size_t *)((char *)allocator + current_offset);
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
