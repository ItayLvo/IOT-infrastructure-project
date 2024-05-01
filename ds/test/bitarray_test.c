#include <stdio.h>	/* printf */
#include <limits.h>	/* CHAR_BIT */
#include <stdlib.h>	/* malloc */
#include "../include/bitarray.h"

#define ALL_BITS_SET_64 0xFFFFFFFFFFFFFFFF

int TestSetAll();
int TestResetAllAndCountAll();

int main()
{
	TestSetAll();
	TestResetAllAndCountAll();
	
	
	return 0;
}

int TestSetAll()
{
	bit_array_t bitarr = 0;

	char *buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	bitarr = SetAll(bitarr);
	buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	
	return 0;
}

int TestResetAllAndCountAll()
{
	bit_array_t bitarr = ALL_BITS_SET_64;
	
	char *buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	printf("before ResetAll: CountAll(bitarr) = %ld\n", CountOn(bitarr));
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	
	bitarr = ResetAll(bitarr);
	buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	printf("before ResetAll: CountAll(bitarr) = %ld\n", CountOn(bitarr));
	
	return 0;
}
