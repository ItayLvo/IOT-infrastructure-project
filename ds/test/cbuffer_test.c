#include <stdio.h>	/* printf, size_t */
#include <stdlib.h>	/* malloc */

#include "../include/cbuffer.h"

int main()
{
	cbuffer_t *cbuffer = CBufferCreate(10);
	char *src = calloc(10, 1);
	int i = 0;
	char *buffer = calloc(10, 1);
	ssize_t count_written = 0;
	ssize_t count_read = 0;
	
	
	for (i = 0; i < 10; ++i)
	{
		src[i] = i + '0';
	}
	
	
	printf("CBufferBufsiz = %ld\n", CBufferBufsiz(cbuffer));
	printf("CBufferIsEmpty = %d\n", CBufferIsEmpty(cbuffer));
	count_written = CBufferWrite(cbuffer, src, 15);
	printf("after tring to write 15 bytes, count_written = %ld\n", count_written);
	printf("CBufferAvailableSpace = %ld\n", CBufferAvailableSpace(cbuffer));
	printf("CBufferIsEmpty = %d\n\n", CBufferIsEmpty(cbuffer));
	
	count_read = CBufferRead(cbuffer, buffer, 2);
	printf("after trying to read 2 bytes, count_read = %ld\n", count_read);
	
	count_written = CBufferWrite(cbuffer, src, 15);
	printf("after tring to write 15 bytes again, count_written = %ld\n", count_written);
	
	count_read = CBufferRead(cbuffer, buffer, 5);
	printf("after trying to read 5 bytes, count_read = %ld\n", count_read);
	
	for (i = 0; i < 10; ++i)
	{
		printf("%c ", buffer[i]);
	}
	printf("\n");
	
	free(src);
	free(buffer);
	CBufferDestroy(cbuffer);
	
	return 0;
}


