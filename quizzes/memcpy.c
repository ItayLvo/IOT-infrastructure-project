#include <stdio.h>	/* printf, size_t */
#include <string.h>	/* memcpy for testing */

int main(void)
{
	
	char *str1 = "abcdefghij";
	char str2[30];


	if (((char *)Memcpy(str2, str1, 6)) == ((char *)memcpy(str2, str1, 6)))
	{
		printf("passed test\n");
	}
	

	return 0;
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
}`
