/*
itay
reviewed by shahar
*/

#include <stdio.h> /*printf*/
#include "ws6.h"


static void HelperPrintInBinary(int num)
{
	int i = 0;
	for (i = 31; i >= 0; --i)
	{
		printf("%d", (num >> i) & 1);	
		
		if (i % 4 == 0)
		{
			printf(" ");
		}
	}
	printf("\n");
}



void PrintFloat(float f)
{
	unsigned int *int_ptr;
	
	int_ptr = (unsigned int *)&f;
	HelperPrintInBinary(*int_ptr);
}


int CountBitsOnLoop(unsigned int n)
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

void Swap(int *a, int *b)
{
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}

int ClosestToSixteenNoRemains(unsigned int n)
{
	printf("%d\n", n & 0xFFFFFFF0);
	return (n & 0xFFFFFFF0);
}


int CheckBothSecondSixth(unsigned char ch)
{
	return ((ch & 2) && (ch & 64));
}

int CheckSecondOrSixth(unsigned char ch)
{
	return ((ch & 2) || (ch & 64));
}


int CountBitsOnNoLoop(unsigned int n)
{
	/* LUT for each nibble (4 bits) */
	const unsigned char bit_count_table[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};

	/* sum the number of set bits in each nibble */
	return	bit_count_table[n & 0xF] + bit_count_table[(n >> 4) & 0xF] +
		bit_count_table[(n >> 8) & 0xF] + bit_count_table[(n >> 12) & 0xF] +
		bit_count_table[(n >> 16) & 0xF] + bit_count_table[(n >> 20) & 0xF] +
		bit_count_table[(n >> 24) & 0xF] + bit_count_table[(n >> 28) & 0xF];
}


unsigned char SwapThirdAndFifth(unsigned char ch)
{
	char third_bit = 0;
	char fifth_bit = 0;
	
	printf("current char: \n");
	HelperPrintInBinary(ch);
	
	
	third_bit = third_bit | (ch & 4);
	fifth_bit = fifth_bit | (ch & 16);
	
	printf("third bit: \n");
	HelperPrintInBinary(third_bit);
	printf("fifth bit: \n");
	HelperPrintInBinary(fifth_bit);
	
	if ((third_bit << 2) == fifth_bit)
	{
		return ch; /* 3rd and 5th are the same bit (not replacing) */
	}
	else
	{
		printf("replacing\n");
		if (0 == third_bit)
		{
			ch = ch ^ fifth_bit;
			ch = ch | 4;
		}
		else	/* fifth_bit == 0 */
		{
			ch = ch ^ third_bit;
			ch = ch | 16;
		}
	}
	
	printf("afte swap: \n");
	HelperPrintInBinary(ch);
	
	return ch;
}


int MirrorBits(int n)
{
	int reversed_num = 0;
	int i = 0;
	
	for (i = 0; i < 32; ++i)
	{
		reversed_num = reversed_num << 1;
		reversed_num = reversed_num | (n & 1); /* adds the right-most digit of n to ans. if it was 1, it remains 1 */
		n = n >> 1;
	}
	
	return reversed_num;
}


void PrintThreeBitsOn(unsigned int arr[], size_t size)
{
	size_t i = 0;
	int temp = 0;
	int count = 0;
	
	for (i = 0; i < size / sizeof(arr[0]); ++i)
	{
		temp = arr[i];
		while (temp)
		{
			if (temp & 1)
			{
				++count;
				
			}
			temp = temp >> 1;	
		}
		
		if (3 == count)
		{
			printf("%d has 3 bits on exactly\n", arr[i]); 
		}
		count = 0;
	}
}


int AddOne(int x) 
{ 
	int tmp = 1; 
	
	while ((x & tmp) != 0) 
	{
		x = x ^ tmp; 
		tmp  = tmp << 1; 
	} 
	x = x ^ tmp; 
	
	return x; 
} 

long Pow2(unsigned int x, unsigned int y)
{
	return (x << y);
}

int IsPowerOfTwoLoop(unsigned int n)
{
	int count = 0;
	
	if (0 == n)
	{
		return 0;
	}
	
	while (0 != n)
	{
		if (n & 1)
		{
			++count;
		}
		if (count > 1)
		{
			return 0;
		}
		
		n = (n >> 1);
	}
	
	return 1;
}

int IsPowerOfTwo(unsigned int n)
{
	int x = 0;
	
	if (0 == n)
	{
		return 0;
	}
	
	x = (n & (n - 1));	/* x is 0 if there are no overlaping bits in (n) and (n-1). if any bits overlap, x is 1 */
	x = !x;			/* if x was 0, x will now be 1 */
	
	return x;
}


