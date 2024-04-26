#include <stdint.h>	/* uintptr_t */
#include <stddef.h>	/* size_t */
#include <stdio.h>	/* printf */
#include "ws10.h"


#define WORD_SIZE sizeof(int)


static unsigned int CreateWordSizedChunk(char c);
static void *MemmoveOverlap(void *dest, const void * src, size_t n);


void *Memset(void *dest, int c, size_t n)
{
	size_t i = 0;
	char *ptr = dest;
	size_t word_count = 0;
	unsigned int word = 0;
	int *word_ptr = NULL;
	
	if (NULL == dest)
	{
		return NULL;
	}
	
	/* write un-aligned bytes one by one until aligning to word size */
	while ((((int)(long)ptr % WORD_SIZE) != 0) && n > 0)
	{
		*ptr = (char)c;
		++ptr;
		--n;
	}
	
	word_count = n / WORD_SIZE;
	word = CreateWordSizedChunk(c);
	
	/* write word-size chunks */
	word_ptr = (int *)ptr;		/* convering ptr from (char *) to (unsigned int *) */
	for (i = 0; i < word_count; ++i)
	{
		*word_ptr = word;
		++word_ptr;
		n = n - WORD_SIZE;
	}
	
	/* dealing with left-over bytes (smaller than word size) one by one */
	ptr = (char *)word_ptr;		/* converting ptr back to (char *) */
	while (0 < n)
	{
		*ptr = c;
		++ptr;
		--n;
	}
	
	return dest;
}


void *Memcpy(void *dest, const void * src, size_t n)
{
	char *ptr_dest = dest;
	const char *ptr_src = src;
	size_t word_count = 0;
	int *word_ptr_dest = NULL;
	int *word_ptr_src = NULL;
	size_t i = 0;

	if (NULL == dest || NULL == src)
	{
		return dest;
	}
	
	while ((((long)ptr_dest % WORD_SIZE) != 0) && n > 0)
	{
		*ptr_dest = *ptr_src;
		++ptr_dest;
		--n;
		++ptr_src;
	}
	
	word_count = n / WORD_SIZE;
	
	word_ptr_dest = ((int *)ptr_dest);	/* convering ptr from (char *) to (unsigned int *) */
	word_ptr_src = ((int *)ptr_src);
	for (i = 0; i < word_count; ++i)
	{
		*word_ptr_dest = *word_ptr_src;
		++word_ptr_dest;
		n = n - WORD_SIZE;
		++word_ptr_src;
	}

	
	ptr_dest = (char *)word_ptr_dest;		/* converting ptr back to (char *) */
	ptr_src = (char *)word_ptr_src;
	while (0 < n)
	{
		*ptr_dest = *ptr_src;
		++ptr_dest;
		--n;
		++ptr_src;
	}
	

	return dest;
}



void *Memmove(void *dest, const void * src, size_t n)
{
	char *ptr_dest = dest;
	const char *ptr_src = src;

	if (NULL == dest || NULL == src)
	{
		return dest;
	}
	
	if (ptr_dest > ptr_src && ptr_dest < (ptr_src + n))	/*overlapping memory addresses, dest > src */
	{
		dest = MemmoveOverlap(dest, src, n);
	}
	else	/* no memory overlap */
	{
		dest = Memcpy(dest, src, n);
	}
	
	return dest;
}


static void *MemmoveOverlap(void *dest, const void * src, size_t n)
{
	char *ptr_dest = (char *)dest + n - 1;
	const char *ptr_src = (const char *)src + n - 1;
	size_t word_count = 0;
	int *word_ptr_dest = NULL;
	int *word_ptr_src = NULL;
	size_t i = 0;
	
	while ((((long)ptr_dest % WORD_SIZE) != 0) && n > 0)
	{
		*ptr_dest = *ptr_src;
		--ptr_dest;
		--n;
		--ptr_src;
	}
	
	word_count = n / WORD_SIZE;
	
	word_ptr_dest = ((int *)ptr_dest);	/* convering ptr from (char *) to (unsigned int *) */
	word_ptr_src = ((int *)ptr_src);
	for (i = 0; i < word_count; ++i)
	{
		*word_ptr_dest = *word_ptr_src;
		--word_ptr_dest;
		n = n - WORD_SIZE;
		--word_ptr_src;
	}

	
	ptr_dest = (char *)word_ptr_dest;		/* converting ptr back to (char *) */
	ptr_src = (char *)word_ptr_src;
	while (0 < n)
	{
		*ptr_dest = *ptr_src;
		--ptr_dest;
		--n;
		--ptr_src;
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





