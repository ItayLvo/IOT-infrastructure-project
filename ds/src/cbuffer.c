#include <stddef.h>	/* size_t */
#include <string.h>	/* memcpy */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "../include/cbuffer.h"

struct cbuffer
{
  size_t capacity;              
  size_t current_size;              
  size_t read_index;              
  char buffer[1];           
} cbuffer_t ;   


cbuffer_t *CBufferCreate(size_t buffer_size)
{
	cbuffer_t *cbuffer = (cbuffer_t *)malloc(sizeof(cbuffer_t + buffer_size - 1));
	
	if (NULL == cbuffer)
	{
		return NULL;
	}
	
	cbuffer->capacity = buffer_size;
	cbuffer->current_size = 0;
	cbuffer->read_index = 0;
	
	return cbuffer;
}


void CBufferDestroy(cbuffer_t *cbuffer);
{
	free(cbuffer);
}


	assert(stack);
	return (stack->current_size == 0);
}


size_t CBufferAvailableSpace(const cbuffer_t *cbuffer)
{
	return ((cbuffer->capacity) - (cbuffer->current_size));
}


size_t CBufferBufsiz(const cbuffer_t *cbuffer)
{
	return cbuffer->capacity;
}


ssize_t CBufferRead(const cbuffer_t *cbuffer, void *dest, size_t count)
{
	ssize_t count_read_to_return = 0;
	size_t tmp_read_index = 0;
	
	while ((cbuffer->current_size > 0) && count > 0)
	{
		tmp_read_index = cbuffer->read_index % cbuffer->capacity;
		*(char *)dest = *((cbuffer->buffer) + tmp_read_index);
		++(cbuffer->read_index);
		--(cbuffer->current_size);
		++count_read_to_return;
		++(char *)dest;
		--count;
	}
	
	return count_read_to_return;
}



ssize_t CBufferWrite(cbuffer_t *cbuffer, const void *src, size_t count)
{
	size_t write_index = 0;
	
	while (((cbuffer->current_size) < (cbuffer->capacity)) &&
					count > 0 && NULL != src)
	{
		write_index = ((cbuffer->current_size) +
				(cbuffer->read_index)) % cbuffer->capacity;
	
		*(cbuffer->buffer + write_index) = *(char *)src;
		++(cbuffer->current_size);
		++(char *)src;
		--count;
	}
}


int CBufferIsEmpty(const cbuffer_t *cbuffer)
{
	return (cbuffer->current_size == 0);
}




