/*
name: itay
status: implemented all functions and tests
reviewer:
*/

#include <stdio.h>	/* printf */
#include <limits.h>	/* CHAR_BIT */
#include <assert.h>	/* assert */

#include "../include/bitarray.h"

#define ALL_BITS_SET_64 0xFFFFFFFFFFFFFFFF
#define SIZE_T_BITS 64

static size_t nibble_mirror_lut[16] = {0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE,
				0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF};
				
static size_t nibble_count_on[] = {0, 1, 1, 2, 1, 2, 2, 3,
			   1, 2, 2, 3, 2, 3, 3, 4};	

bit_array_t SetAll(bit_array_t bit_arr)
{
	bit_arr |= ALL_BITS_SET_64;
	
	return bit_arr;
}

bit_array_t ResetAll(bit_array_t bit_arr)
{
	(void)bit_arr;
	return (bit_array_t)0;
	
}

bit_array_t SetOn(bit_array_t bit_arr, size_t index)
{
	assert (index <= SIZE_T_BITS);
	bit_arr = SetBit(bit_arr, index, 1);
	
	return bit_arr;
}

bit_array_t SetOff(bit_array_t bit_arr, size_t index)
{
	assert (index <= SIZE_T_BITS);
	bit_arr = SetBit(bit_arr, index, 0);
	
	return bit_arr;
}

bit_array_t SetBit(bit_array_t bit_arr, size_t index, int bit_value)
{
	size_t bit_value_64_bit_cast = (size_t)bit_value;
	size_t mask = 1 << index;
	size_t reversed_bit_mask = ~mask;
	assert (index <= SIZE_T_BITS);

	bit_arr = bit_arr & reversed_bit_mask;
	bit_arr = bit_arr | (bit_value_64_bit_cast << index);

	return bit_arr;
}


int GetVal(bit_array_t bit_arr, size_t index)
{
	size_t mask = (1 << index);
	
	return !!(bit_arr & mask);
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
	return ~bit_arr;
}

/* mirror LUT is defined at the top top of the file */
static size_t MirrorNibbles(char byte)
{
	return (nibble_mirror_lut[byte & 0xF]) | (nibble_mirror_lut[(byte >> 4) & 0xF]) << 4;
}

bit_array_t MirrorLut(bit_array_t bit_arr)
{
	bit_array_t mirrored_bit_arr = 0;

	mirrored_bit_arr |= (MirrorNibbles((bit_arr >> 0) & 0xFF) << 56);
	mirrored_bit_arr |= (MirrorNibbles((bit_arr >> 8) & 0xFF) << 48);
	mirrored_bit_arr |= (MirrorNibbles((bit_arr >> 16) & 0xFF) << 40);
	mirrored_bit_arr |= (MirrorNibbles((bit_arr >> 24) & 0xFF) << 32);
	mirrored_bit_arr |= (MirrorNibbles((bit_arr >> 32) & 0xFF) << 24);
	mirrored_bit_arr |= (MirrorNibbles((bit_arr >> 40) & 0xFF) << 16);
	mirrored_bit_arr |= (MirrorNibbles((bit_arr >> 48) & 0xFF) << 8);
	mirrored_bit_arr |= (MirrorNibbles((bit_arr >> 56) & 0xFF) << 0);

	return mirrored_bit_arr;
}


bit_array_t Mirror(bit_array_t bit_arr)
{
	bit_arr = ((bit_arr >> 1) & 0x5555555555555555) |
		((bit_arr & 0x5555555555555555) << 1);
		/*010101...*/
	bit_arr = ((bit_arr >> 2) & 0x3333333333333333) |
		((bit_arr & 0x3333333333333333) << 2);
		/*00110011...*/
	bit_arr = ((bit_arr >> 4) & 0x0F0F0F0F0F0F0F0F) |
		((bit_arr & 0x0F0F0F0F0F0F0F0F) << 4);
		/*0000111100001111...*/
	bit_arr = ((bit_arr >> 8) & 0x00FF00FF00FF00FF) |
		((bit_arr & 0x00FF00FF00FF00FF) << 8);
		/*0000000011111111...*/
	bit_arr = ((bit_arr >> 16) & 0x0000FFFF0000FFFF) |
		((bit_arr & 0x0000FFFF0000FFFF) << 16);
		/*16 bits off, 16 bits on*/
	bit_arr = (bit_arr >> 32) | (bit_arr << 32);
		/*swaps left 32 bits with right 32 bits*/	

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
	n %= SIZE_T_BITS;
	return (bit_arr >> n) | (bit_arr << (SIZE_T_BITS - n));
}


bit_array_t RotateLeft(bit_array_t bit_arr,size_t n)
{
	n %= SIZE_T_BITS;
	return (bit_arr << n) | (bit_arr >> (SIZE_T_BITS - n));
}


static size_t CountSetBitsInByte(char byte)
{
	return nibble_count_on[byte & 0xF] + nibble_count_on[(byte >> 4) & 0xF];
}

size_t CountOnLut(bit_array_t bit_arr)
{
	return CountSetBitsInByte(bit_arr & 0xFF) +
		CountSetBitsInByte((bit_arr >> 8) & 0xFF) +
		CountSetBitsInByte((bit_arr >> 16) & 0xFF) +
		CountSetBitsInByte((bit_arr >> 24) & 0xFF) +
		CountSetBitsInByte((bit_arr >> 32) & 0xFF) +
		CountSetBitsInByte((bit_arr >> 40) & 0xFF) +
		CountSetBitsInByte((bit_arr >> 48) & 0xFF) +
		CountSetBitsInByte((bit_arr >> 56) & 0xFF);
}

/* O(n) implementation of CountOn*/

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
	assert(buffer);
	
	for (i = size - 1; i >= 0; --i)
	{
		*runner = ((bit_arr >> i) & 1) + '0';
		++runner;
	}
	*runner = '\0';
	
	return buffer;
}
