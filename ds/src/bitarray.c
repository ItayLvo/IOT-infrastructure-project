#include <stdio.h>	/* printf */
#include <limits.h>	/* CHAR_BIT */

#include "../include/bitarray.h"

#define ALL_BITS_SET_64 0xFFFFFFFFFFFFFFFF
#define SIZE_T_BITS 64

bit_array_t SetAll(bit_array_t bit_arr)
{
	bit_arr |= ALL_BITS_SET_64;
	
	return bit_arr;
}

bit_array_t ResetAll(bit_array_t bit_arr)
{
	bit_arr = 0;
	
	return bit_arr;
}

bit_array_t SetOn(bit_array_t bit_arr, size_t index)
{
	bit_arr = SetBit(bit_arr, index, 1);
	
	return bit_arr;
}

bit_array_t SetOff(bit_array_t bit_arr, size_t index)
{
	bit_arr = SetBit(bit_arr, index, 0);
	
	return bit_arr;
}


bit_array_t SetBit(bit_array_t bit_arr, size_t index, int bool_value)
{
	size_t mask = 1;
	size_t not_mask;
	
	mask <<= index;
	not_mask  = ~mask;

	bit_arr = bit_arr & not_mask;
	bit_arr = bit_arr | (bool_value << index);

	return bit_arr;
}

int GetVal(bit_array_t bit_arr, size_t index)
{
	size_t mask = 1;
	
	bit_arr >>= index;
	
	return (bit_arr & mask);
}

bit_array_t FlipBit(bit_array_t bit_arr, size_t index)
{
	size_t mask = 1;
	
	mask <<= index;
	bit_arr = bit_arr ^ mask;
	
	return bit_arr;
}


bit_array_t FlipAll(bit_array_t bit_arr)
{
	bit_arr = ~(bit_arr);
	
	return bit_arr;
}


bit_array_t Mirror(bit_array_t bit_arr)
{
	bit_arr = ((bit_arr >> 1) & 0x5555555555555555) | ((bit_arr & 0x5555555555555555) << 1);
	bit_arr = ((bit_arr >> 2) & 0x3333333333333333) | ((bit_arr & 0x3333333333333333) << 2);
	bit_arr = ((bit_arr >> 4) & 0x0F0F0F0F0F0F0F0F) | ((bit_arr & 0x0F0F0F0F0F0F0F0F) << 4);
	bit_arr = ((bit_arr >> 8) & 0x00FF00FF00FF00FF) | ((bit_arr & 0x00FF00FF00FF00FF) << 8);
	bit_arr = ((bit_arr >> 16) & 0x0000FFFF0000FFFF) | ((bit_arr & 0x0000FFFF0000FFFF) << 16);
	bit_arr = (bit_arr >> 32) | (bit_arr << 32);

	return bit_arr;
}


/* O(n) implementation of Mirror*/
/* 
bit_array_t Mirror(bit_array_t bit_arr)
{
	size_t reversed_num = 0;
	size_t n = bit_arr;
	int i = 0;
	
	for (i = 0; i < sizeof(reversed_num) * CHAR_BIT; ++i)
	{
		reversed_num = reversed_num << 1;
		reversed_num = reversed_num | (n & 1);
		n = n >> 1;
	}
	bit_arr = reversed_num;
	
	return bit_arr;
}
*/


bit_array_t RotateRight(bit_array_t bit_arr,size_t n)
{
	return (bit_arr >> n) | (bit_arr << (SIZE_T_BITS - n));
}


bit_array_t RotateLeft(bit_array_t bit_arr,size_t n)
{
	return (bit_arr << n) | (bit_arr >> (SIZE_T_BITS - n));
}



static size_t nibble_count_on[] = {0, 1, 1, 2, 1, 2, 2, 3,
				   1, 2, 2, 3, 2, 3, 3, 4};

static size_t count_set_bits_in_byte(char byte)
{
	return nibble_count_on[byte & 0xF] + nibble_count_on[(byte >> 4) & 0xF];
}

size_t CountOn(bit_array_t bit_arr)
{
	return count_set_bits_in_byte(bit_arr & 0xFF) +
		count_set_bits_in_byte((bit_arr >> 8) & 0xFF) +
		count_set_bits_in_byte((bit_arr >> 16) & 0xFF) +
		count_set_bits_in_byte((bit_arr >> 24) & 0xFF) +
		count_set_bits_in_byte((bit_arr >> 32) & 0xFF) +
		count_set_bits_in_byte((bit_arr >> 40) & 0xFF) +
		count_set_bits_in_byte((bit_arr >> 48) & 0xFF) +
		count_set_bits_in_byte((bit_arr >> 56) & 0xFF);
}

/* O(n) implementation of CountOn*/
/*
size_t CountOn(bit_array_t bit_arr)
{
	size_t count = 0;
	unsigned long n = bit_arr;
	
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
*/

size_t CountOff(bit_array_t bit_arr)
{	
	size_t count = 0;
	
	count = (sizeof(bit_arr) * CHAR_BIT) - (CountOn(bit_arr));

	return count;
}


char *ToString(bit_array_t bit_arr, char *buffer)
{
	int i = 0;
	size_t size = sizeof(size_t) * CHAR_BIT;
	char *runner = buffer;
	
	for (i = size - 1; i >= 0; --i)
	{
		*runner = ((bit_arr >> i) & 1) + '0';
		++runner;
	}
	*runner = '\0';
	return buffer;
}
