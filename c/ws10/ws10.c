#include <stdint.h>	/* uintptr_t */
#include <stddef.h>	/* size_t */
#include "ws10.h"


#define WORD_SIZE sizeof(int)


static unsigned int CreateWordSizedChunk(char c);


void *Memset(void *dest, int c, size_t n)
{
	size_t i = 0;
	char *ptr = dest;
	size_t word_count = 0;
	/* size_t word_remainder = 0; */
	unsigned int word = 0;
	unsigned int *word_ptr = NULL;
	
	
	
	if (NULL == dest)
	{
		return NULL;
	}
	
	
	/* handle un-aligned bytes one by one until aligning to word size */
	while ((((unsigned long)ptr % WORD_SIZE) != 0) && n > 0)
	{
		*ptr = (unsigned char)c;
		++ptr;
		--n;
	}
	
	
	word_count = n / WORD_SIZE;
	/* word_remainder = n % WORD_SIZE; */
	
	word = CreateWordSizedChunk(c);
	
	/* handle word chunks */
	word_ptr = (unsigned int *)ptr;		/* convering ptr from (char *) to (unsigned int *) */
	for (i = 0; i < word_count; ++i)
	{
		*word_ptr = word;
		++word_ptr;
		n = n - WORD_SIZE;
	}
	
	
	/* dealing with left-over bytes, that are smaller than word size */
	ptr = (char *)word_ptr;		/* converting ptr back to (char *) */
	while (0 < n)
	{
		*ptr = c;
		++ptr;
		--n;
	}
	
	return dest;
}

static unsigned int CreateWordSizedChunk(char c)
{
	unsigned int word = 0;
	size_t i = 0;
	
	for (i = 0; i < WORD_SIZE; ++i)
	{
		word = word | (c << (i * 8));
	}
	
	return word;
}





