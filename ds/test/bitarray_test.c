#include <stdio.h>	/* printf */
#include <limits.h>	/* CHAR_BIT */
#include <stdlib.h>	/* malloc */
#include "../include/bitarray.h"


int main()
{
	bit_array_t bitarr = 0;

	char *buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT);
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	
	bitarr = SetAll(bitarr);
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	return 0;
}
