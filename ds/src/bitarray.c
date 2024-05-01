#include <stdio.h>	/* printf */
#include <limits.h>	/* CHAR_BIT */

#include "bitarray.h"


void SetAll(bitarray *bitarr)
{
	bitarr->data = 0xFFFFFFFFFFFFFFFF;
}

void ResetAll(bitarray *bitarr)
{
	bitarr -> data = 0;
}

void SetBit(bitarray *bitarr, size_t index, bit_mode_t bit_mode);
{
	size_t mask = bit_mode;
	mask <<= index;
	bitarr -> data |= mask;
}

bit_mode_t GetVal(bitarray *bitarr, size_t index)
{
	size_t mask = 1;
	mask <<= index;
	return ((bitarr -> data) & mask);
}

void Flip(bitarray *bitarr)
{
	bitarr -> data = ~(bitarr -> data);
}

void Mirror(bitarray *bitarr)
{
	unsigned long reversed_num = 0;
	unsigned long n = bitarr -> data;
	int i = 0;
	
	for (i = 0; i < sizeof(reversed_num * CHAR_BIT; ++i)
	{
		reversed_num = reversed_num << 1;
		reversed_num = reversed_num | (n & 1); /* adds the right-most digit of n to ans. if it was 1, it remains 1 */
		n = n >> 1;
	}
	
	bitarr -> data = reversed_num;
}


void CountOn(bitarray *bitarr);
{
	int count = 0;
	
	while (n != 0)
	{
		if ((n & 1) == 1)
		{
			++count;
		}
		n = n >> 1;
	}
	
	return count;
}


void ToString(bitarray *bitarr)
{
	unsigned long data_copy = bitarr -> data;
	int i = 0;
	size_t size = sizeof(long) * CHAR_BIT;
	
	for (i = size - 1; i >= 0; --i)
	{
		printf("%ld", (data_copy >> i) & 1);	
		
		if (i % 4 == 0)
		{
			printf(" ");
		}
	}
	printf("\n");
}
