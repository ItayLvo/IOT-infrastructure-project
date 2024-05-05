#include <stdio.h>	/* printf */
#include <limits.h>	/* CHAR_BIT */
#include <stdlib.h>	/* malloc */

#include "../include/bitarray.h"

#define ALL_BITS_SET_64 0xFFFFFFFFFFFFFFFF

int TestSetAll();
int TestResetAllAndCount();
int TestSetOffAndOn();
int TestGetVal();
int TestFlip();
int TestMirror();
int TestRotate();

int main()
{
	int program_exit_status = 0;
	
	program_exit_status = TestSetAll();
	program_exit_status = TestResetAllAndCount();
	program_exit_status = TestSetOffAndOn();
	program_exit_status = TestGetVal();
	program_exit_status = TestFlip();
	program_exit_status = TestRotate();
	program_exit_status = TestMirror();
	
	return program_exit_status;
}

int TestSetAll()
{
	bit_array_t bitarr = 0;

	char *buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	printf("bitarr = 0, before SetAll():\n");
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	bitarr = SetAll(bitarr);
	buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	printf("after SetAll():\n");
	buffer = ToString(bitarr, buffer);
	printf("%s\n\n", buffer);
	free(buffer);
	
	return 0;
}

int TestResetAllAndCount()
{
	bit_array_t bitarr = ALL_BITS_SET_64;
	
	char *buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	printf("before ResetAll: CountOn(bitarr) = %ld\n", CountOn(bitarr));
	printf("before ResetAll: CountOff(bitarr) = %ld\n", CountOff(bitarr));
	
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
	printf("after ResetAll: CountOn(bitarr) = %ld\n", CountOn(bitarr));
	printf("after ResetAll: CountOff(bitarr) = %ld\n\n", CountOff(bitarr));
	
	return 0;
}



int TestSetOffAndOn()
{
	bit_array_t bitarr = ALL_BITS_SET_64;

	char *buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	printf("Testing SetOff and SetOn:\n");
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	
	bitarr = SetOff(bitarr, 0);
	bitarr = SetOff(bitarr, 1);
	bitarr = SetOff(bitarr, 2);
	bitarr = SetOff(bitarr, 3);
	bitarr = SetOff(bitarr, 4);
	bitarr = SetOff(bitarr, 5);
	bitarr = SetOff(bitarr, 6);
	bitarr = SetOff(bitarr, 33);
	bitarr = SetOff(bitarr, 63);
	
	buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	
	
	buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	bitarr = SetOff(bitarr, 0);
	bitarr = SetOff(bitarr, 2);
	bitarr = SetOff(bitarr, 4);
	bitarr = SetOff(bitarr, 6);
	bitarr = SetOn(bitarr, 1);
	bitarr = SetOn(bitarr, 3);
	bitarr = SetOn(bitarr, 5);
	bitarr = SetOn(bitarr, 63);
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n\n", buffer);
	free(buffer);
	
	
	return 0;
}

int TestGetVal()
{
	bit_array_t bitarr = 2;
	printf("Testing GetVal: \n");
	printf("for bitarr = 0b10, bit[0] = %d, bit[1] = %d\n\n", GetVal(bitarr, 0), GetVal(bitarr, 1));
	
	return 0;
}

int TestFlip()
{
	bit_array_t bitarr = 0;

	char *buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	printf("Testing FlipAll: \n");
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	
	bitarr = FlipAll(bitarr);
	buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	printf("Testing FlipBit(bitarr, 2): \n");
	bitarr = FlipBit(bitarr, 2);
	buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n\n", buffer);
	free(buffer);
	
	return 0;
}


int TestMirror()
{
	bit_array_t bitarr = 0xFFFF5555FFFF5555;
	
	char *buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	printf("Testing Mirror: \n");
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	
	bitarr = Mirror(bitarr);
	buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	
	printf("Testing MirrorLUT: \n");
	bitarr = MirrorLut(bitarr);
	buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n\n", buffer);
	free(buffer);
	
	
	return 0;
}


int TestRotate()
{
	bit_array_t bitarr = 0xFFFFFFFF00000000;
	
	char *buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	printf("Testing Rotate: \n");
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	
	bitarr = RotateLeft(bitarr, 5);
	buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n", buffer);
	free(buffer);
	
	bitarr = RotateRight(bitarr, 10);
	buffer = (char *)malloc(sizeof(char) * sizeof(size_t) * CHAR_BIT + 1);
	if (NULL == buffer)
	{
		return 1;
	}
	
	buffer = ToString(bitarr, buffer);
	printf("%s\n\n", buffer);
	free(buffer);
	
	
	return 0;
}
