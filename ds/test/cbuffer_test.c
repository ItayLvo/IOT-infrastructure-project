/*
date: 16/05/24 
status: waiting for CR
reviewer name:
*/

#include <stdio.h>	/* printf, size_t */
#include <stdlib.h>	/* malloc */

#include "../include/cbuffer.h"

#define BUFFER_SIZE 10

int CBufferTestIsEmpty(void);
int CBufferTestCreateDestroyBufsiz(void);
int CBufferTestReadWriteAvaliableSpace(void);

int main()
{
	CBufferTestIsEmpty();
	CBufferTestCreateDestroyBufsiz();
	CBufferTestReadWriteAvaliableSpace();
	printf("All tests passed\n");
	return 0;
}


int CBufferTestCreateDestroyBufsiz(void)
{
	cbuffer_t *cbuffer = CBufferCreate(BUFFER_SIZE);
	size_t cbuffer_size = BUFFER_SIZE;
	
	if (NULL == cbuffer)
	{
		printf("CBufferCreate failed\n");
		return 1;
	}
	
	if (cbuffer_size != CBufferBufsiz(cbuffer))
	{
		printf("CBuffersiz failed\n");
		printf("Expected CBufferBufsiz = %ld,received value=  %ld\n",
				 cbuffer_size, CBufferBufsiz(cbuffer));
		return 1;
	}
	
	
	CBufferDestroy(cbuffer);
	return 0;
}


int CBufferTestIsEmpty(void)
{
	cbuffer_t *cbuffer = CBufferCreate(BUFFER_SIZE);
	int is_empty_true = 1, is_empty_false = 0;
	char *src = malloc(2);
	src[0] = 0;
	src[1] = 1;
	
	if (is_empty_true != CBufferIsEmpty(cbuffer))
	{
		printf("CBufferIsEmpty failed\n");
		printf("Expected CBufferIsEmpty = %d,received value= %d\n",
				 is_empty_true, CBufferIsEmpty(cbuffer));
		return 1;
	}
	
	CBufferWrite(cbuffer, src, 2);
	if (is_empty_false != CBufferIsEmpty(cbuffer))
	{
		printf("CBufferIsEmpty failed\n");
		printf("Expected CBufferIsEmpty = %d,received value= %d\n",
				 is_empty_false, CBufferIsEmpty(cbuffer));
		return 1;
	}
	
	free(src);
	CBufferDestroy(cbuffer);
	return 0;
}


int CBufferTestReadWriteAvaliableSpace(void)
{
	cbuffer_t *cbuffer = CBufferCreate(BUFFER_SIZE);
	char *src = calloc(10, 1);
	char *tmp_buffer = calloc(10, 1);
	ssize_t count_written = 0;
	ssize_t count_read = 0;
	int i = 0;
	
	for (i = 0; i < 10; ++i)
	{
		src[i] = i + '0';
	}
	
	
	count_written = CBufferWrite(cbuffer, src, BUFFER_SIZE / 2);
	if (count_written != BUFFER_SIZE / 2)
	{
		printf("CBufferWrite failed\n");
		printf("Expected CBufferWrite = %ld,received value=  %ld\n",
				 BUFFER_SIZE / 2, count_written);
		return 1;
	}
	
	
	if (BUFFER_SIZE / 2 != CBufferAvailableSpace(cbuffer))
	{
		printf("CBufferAvailableSpace failed\n");
		printf("Expected AvailableSpace = %ld,received value=%ld\n",
			 BUFFER_SIZE / 2, CBufferAvailableSpace(cbuffer));
		return 1;
	}
	
	count_read = CBufferRead(cbuffer, tmp_buffer, 2);
	if (count_read != 2)
	{
		printf("CBufferRead failed\n");
		printf("Expected CBufferRead = %ld,received value=  %ld\n",
				 BUFFER_SIZE / 2, count_read);
		return 1;
	}
	
	free(src);
	free(tmp_buffer);
	CBufferDestroy(cbuffer);
	return 0;
}
