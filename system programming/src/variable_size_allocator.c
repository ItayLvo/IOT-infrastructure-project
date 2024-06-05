#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "variable_size_allocator.h"		/* FSA functions, typedefs */

#define WORD_SIZE sizeof(void *)
#define BLOCK_HEADER_SIZE sizeof(size_t *)

struct variable_size_allocator
{
	size_t pool_size;
};

static void *AlignMemoryPoolStart(void *memory_pool);
static size_t AlignPoolSize(size_t pool_size);
static size_t AlignBlockSize(size_t block_size);
static long *MergeFreeBlocks(long *prev, long *next);
static long GetAbsValue(long num);


vsa_t *VSAInitialize(void *memory_pool, size_t pool_size)
{
	vsa_t *allocator = NULL;	
	long *first_block = NULL;
	size_t pool_aligned_penalty = 0;
	assert(memory_pool);
	
	allocator = (vsa_t *)(AlignMemoryPoolStart(memory_pool));
	pool_aligned_penalty = (char *)allocator - (char *)memory_pool;
	
	allocator->pool_size = AlignPoolSize(pool_size - pool_aligned_penalty);
	
	first_block = (long *)((char *)memory_pool + sizeof(vsa_t));
	*first_block = pool_size - sizeof(vsa_t) - BLOCK_HEADER_SIZE;
	
	return allocator;
}


void *VSAAlloc(vsa_t *allocator, size_t block_size)
{
	long *current_block = (long *)((char *)allocator + sizeof(vsa_t));
	long current_block_size = 0;
	long consecutive_block_sum = 0;
	long *block_runner = NULL;
	size_t current_offset = (char *)current_block - (char *)allocator;
	
	block_size = AlignBlockSize(block_size);
	
	while (current_offset < (allocator->pool_size - BLOCK_HEADER_SIZE))
	{
		current_block_size =  *current_block;
		
		/* if current block size is negative => the current block is already alloc'd. move pointer to next block */
		if (current_block_size < 0)
		{
			current_block = (long *)((char *)current_block + ((current_block_size * -1) + BLOCK_HEADER_SIZE));
		}
		/* else, current block size is positive => check if there's enough room to fit request */
		else
		{
			if (current_block_size < (block_size + BLOCK_HEADER_SIZE)) /* current block is avaliable but too small for requested block size */
			{
				block_runner = current_block;
				while (current_block_size > 0 &&
					current_offset < (allocator->pool_size - BLOCK_HEADER_SIZE) &&
					consecutive_block_sum < (block_size + BLOCK_HEADER_SIZE))
				{
					consecutive_block_sum += (*block_runner + BLOCK_HEADER_SIZE);
					block_runner = (long *)((char *)block_runner + (current_block_size + BLOCK_HEADER_SIZE));
					current_offset = (char *)block_runner - (char *)allocator;
					
					if (current_offset < (allocator->pool_size - BLOCK_HEADER_SIZE))	/* to prevent dereferening outside of pool */
					{
						current_block_size = *block_runner;	
					}
				}
				
				if (consecutive_block_sum == (block_size + BLOCK_HEADER_SIZE))	/* size fits exactly: allocate without creating next header */
				{
					*current_block = (consecutive_block_sum * -1);
					return ((char *)current_block + BLOCK_HEADER_SIZE);
				}
				else if (consecutive_block_sum > (block_size + BLOCK_HEADER_SIZE))	/* size fits with extra room: allocate and create next header*/
				{
					long *new_header = NULL;
					size_t space_remainder = consecutive_block_sum - (block_size + BLOCK_HEADER_SIZE);
					
					*current_block = (block_size * -1);
					new_header = (long *)((char *)current_block + block_size + BLOCK_HEADER_SIZE);
					*new_header = space_remainder;
					
					return ((void *)((char *)current_block + BLOCK_HEADER_SIZE));
				}
				else if (current_offset >= (allocator->pool_size - BLOCK_HEADER_SIZE)) /* reached end of pool, no fitting block found (current_offset >= allocator->pool_size) */
				{
					return NULL;
				}
				else if (current_block_size < 0) 	/* current consecutive series of blocks is not sufficient */
				{
					current_block = block_runner;
					consecutive_block_sum = 0;
					current_block = (long *)((char *)current_block + ((current_block_size * -1) + BLOCK_HEADER_SIZE));
				}
			}
			else	/* current block is avaliable and is sufficient in size */
			{
				if (current_block_size == (block_size + BLOCK_HEADER_SIZE))	/* size fits exactly: allocate without creating next header */
				{
					*current_block = (current_block_size * -1);
					
					return ((void *)((char *)current_block + BLOCK_HEADER_SIZE));
				}
				else	/* size fits with extra room: allocate and create next header*/
				{
					long *new_header = NULL;
					size_t space_remainder = current_block_size - (block_size + BLOCK_HEADER_SIZE);
					
					*current_block = (block_size * -1);
					new_header = (long *)((char *)current_block + block_size + BLOCK_HEADER_SIZE);
					*new_header = space_remainder;
					
					return ((void *)((char *)current_block + BLOCK_HEADER_SIZE));
				}
			}
			
			current_block = (long *)((char *)current_block + current_block_size + BLOCK_HEADER_SIZE);
		}
		
		current_offset = (char *)current_block - (char *)allocator;
		consecutive_block_sum = 0;
	}
	
	return NULL;
}



void VSAFree(void *mem_to_free_ptr)
{
	*(long *)((long *)mem_to_free_ptr - 1) *= -1;
}



size_t VSALargestChunkAvailable(vsa_t *allocator)
{
	long *start_block = (long *)((char *)allocator + sizeof(vsa_t));
	long current_block_size = 0;
	long consecutive_block_sum = 0;
	long *block_runner = start_block;
	size_t current_offset = (char *)start_block - (char *)allocator;
	size_t max_chunk = 0;
	
	while (current_offset < (allocator->pool_size - BLOCK_HEADER_SIZE))
	{
		current_block_size = *block_runner;
		
		if (current_block_size > 0)
		{
			consecutive_block_sum += (*block_runner + BLOCK_HEADER_SIZE);
			block_runner = (long *)((char *)block_runner + (current_block_size + BLOCK_HEADER_SIZE));
			current_offset = (char *)block_runner - (char *)allocator;
			
			if (current_offset < (allocator->pool_size - BLOCK_HEADER_SIZE))	/* to prevent dereferening outside of pool */
			{
				current_block_size = *block_runner;	
			}
		}
		else
		{
			if (consecutive_block_sum > max_chunk)
			{
				max_chunk = consecutive_block_sum;
			}
			
			start_block = (long *)((char *)start_block + ((current_block_size * -1) + BLOCK_HEADER_SIZE));
			block_runner = start_block;
			current_offset = (char *)start_block - (char *)allocator;
		}
		
		if (consecutive_block_sum > max_chunk)
		{
			max_chunk = consecutive_block_sum;
		}
		
		consecutive_block_sum = 0;
	}
	
	return max_chunk;
}





/***************** static helper functions *****************/

static void *AlignMemoryPoolStart(void *pool)
{
	char *runner = NULL;
	assert(pool);
	
	runner = (char *)pool;
	while ((size_t)runner % WORD_SIZE != 0)
	{
		++runner;
	}
	
	return (void *)runner;
}


static size_t AlignPoolSize(size_t pool_size)
{
	while (pool_size % WORD_SIZE != 0)
	{
		--pool_size;
	}
	
	return pool_size;
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
