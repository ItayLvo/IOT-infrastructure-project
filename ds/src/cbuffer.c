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
};   


cbuffer_t *CBufferCreate(size_t buffer_size)
{
	cbuffer_t *cbuffer = (cbuffer_t *)malloc(sizeof(cbuffer_t) + buffer_size - 1);
	
	if (NULL == cbuffer)
	{
		return NULL;
	}
	
	cbuffer->capacity = buffer_size;
	cbuffer->current_size = 0;
	cbuffer->read_index = 0;
	
	return cbuffer;
}


void CBufferDestroy(cbuffer_t *cbuffer)
{
	free(cbuffer);
}


size_t CBufferAvailableSpace(const cbuffer_t *cbuffer)
{
	return ((cbuffer->capacity) - (cbuffer->current_size));
}


size_t CBufferBufsiz(const cbuffer_t *cbuffer)
{
	return cbuffer->capacity;
}


ssize_t CBufferRead(cbuffer_t *cbuffer, void *dest, size_t count)
{
	ssize_t count_read_to_return = 0;
	size_t tmp_read_index = 0;
	char *dest_to_char = NULL;
	assert(dest);
	dest_to_char = (char *)dest;
	
	while ((cbuffer->current_size > 0) && count > 0)
	{
		tmp_read_index = cbuffer->read_index % cbuffer->capacity;
		*dest_to_char = *((cbuffer->buffer) + tmp_read_index);
		++(cbuffer->read_index);
		--(cbuffer->current_size);
		++count_read_to_return;
		++dest_to_char;
		--count;
	}
	
	return count_read_to_return;
}



ssize_t CBufferWrite(cbuffer_t *cbuffer, const void *src, size_t count)
{
	size_t write_index = 0;
	char *src_to_char = NULL;
	ssize_t count_write_to_return = 0;
	assert(src);
	src_to_char = (char *)src;
	
	while (((cbuffer->current_size) < (cbuffer->capacity)) &&
					count > 0 && NULL != src_to_char)
	{
		write_index = ((cbuffer->current_size) +
				(cbuffer->read_index)) % cbuffer->capacity;
	
		*(cbuffer->buffer + write_index) = *src_to_char;
		++(cbuffer->current_size);
		++src_to_char;
		--count;
		++count_write_to_return;
	}
	
	return count_write_to_return;
}


int CBufferIsEmpty(const cbuffer_t *cbuffer)
{
	return (cbuffer->current_size == 0);
}



void PrintBuffer(cbuffer_t *cbuffer)
{
	size_t i = 0;
	
	for (i = 0; i < cbuffer->capacity; ++i)
	{
		printf("%c ", *(cbuffer->buffer + i));
	}
	printf("\n");
}
