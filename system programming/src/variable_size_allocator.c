#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "variable_size_allocator.h"		/* FSA functions, typedefs */

#define WORD_SIZE sizeof(void *)


struct variable_size_allocator
{
	size_t free_list_offset;
	size_t pool_size;
};

static size_t AlignBlockSize(size_t block_size);


vsa_t *VSAInitialize(void *memory_pool, size_t pool_size)
{
	vsa_t *allocator = NULL;	
	size_t *first_block = NULL;
	assert(memory_pool);
	
	allocator = (vsa_t *)memory_pool;
	
	allocator->free_list_offset = sizeof(vsa_t);
	
	first_block = (size_t *)((char *)memory_pool + allocator->free_list_offset);
	*first_block = pool_size - sizeof(vsa_t);
	*(first_block + 1) = 0;
	
	return allocator;
}


void *VSAAlloc(vsa_t *allocator, size_t block_size)
{
	/* block_size = AlignBlockSize(block_size); */
	long *current_block = (long *)((char *)allocator + allocator->free_list_offset);
	long *prev = NULL;
	long *allocated_block = NULL;
	long current_size = 0;
	long tmp_size_sum = 0;
	long remaining_size = 0;
	long *block_runner = NULL;
	
	while (*current_block != 0)
	{
		current_size =  *current_block;
		/* if current block size is negative => the current block is taken. move pointer to next block */
		if (current_size < 0)
		{
			(char *)current_block += (current_size * -1);
			tmp_size_sum = 0;
		}
		else
		{
			if (current_size >= (block_size + WORD_SIZE)) /* size fits */
			{
				/* change sign of current_size to negative to mark it as taken */
				current_size *= -1;
				return ((char *)current_block + WORD_SIZE);
			}
			else	/* current block too small for requested block size */
			{
				block_runner = current_block;
				while (current_size > 0)
				{
					current_size = *block_runner;
					tmp_size_sum += current_size;
					(char *)block_runner += current_size;
			}
		}
	}
	
}


void VSAFree(void *mem_to_free_ptr)
{
	size_t *block_to_free = NULL;
	vsa_t *vsa = NULL;
	assert(mem_to_free_ptr);
	
	block_to_free = (size_t *)mem_to_free_ptr - 1;
	vsa = (vsa_t *)((char *)block_to_free - vsa->free_list_offset);

	*block_to_free = vsa->free_list_offset;
	vsa->free_list_offset = (char *)block_to_free - (char *)vsa;
}


size_t VSALargestChunkAvailable(vsa_t *vsa)
{
	size_t largest_chunk = 0;
	size_t *current = (size_t *)((char *)vsa + vsa->free_list_offset);

	while (*current != 0)
	{
		if (*current > largest_chunk)
		{
			largest_chunk = *current;
		}
		current = (size_t *)((char *)vsa + *(current + 1));
	}

    return largest_chunk;
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
