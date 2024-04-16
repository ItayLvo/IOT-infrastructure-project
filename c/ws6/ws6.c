#include <stdio.h> /*printf*/
#include "ws6.h"





int main()
{
	
	/* testing Pow2: x = 2, y = 3, should return 2*(2^3) = 16 */
	/* printf("%ld\n", Pow2(2, 3)); */
	
	
	/* testing for IsPowerOfTwo */
	/*
	for (i = 0; i < 17; i++)
	{
	printf("i = %d\t", i);
	printf("%ld\n", IsPowerOfTwo(i));
	}
	*/
	
	
	/* testing for PrintThreeBitsOn */
	/*
	unsigned int arr[] = {1,2,3,7,4,15,11,1,13,0,4};
	PrintThreeBitsOn(arr, sizeof(arr));
	*/
	
	
	/* testing for MirrorBits */
	/*
	HelperPrintInBinary(14);
	HelperPrintInBinary(MirrorBits(14));
	printf("%d\n", MirrorBits(14));
	*/
	
	
	/* testing for SwapThirdAndFifth */
	/*
	SwapThirdAndFifth((unsigned char)'r');
	*/
	
	/* testing for Closest */
	/*
	Closest(153);
	*/
	
	
	/* testing for CountBitsOn */
	/*
	printf("%d\n", CountBitsOnLoop(54));
	*/
	
	
	/* testing for PrintFloat */
	/*
	float f = 2.5;
	PrintFloat(2.5);
	*/
	
	/* testing for Q1 */
	/*
	unsigned int x = 5;
	printf("%u %u %u\n", x, x << 2, x >> 2);
	*/
	
	
	
	return 0;
	
}


/*
void PrintFloat(float f)
{
	int i;
	for (i = 31; i >= 0; --i)
	{
		printf("%d", (f >> i) & 1);	
		
		if (i % 4 == 0)
		{
			printf(" ");
		}
	}
	printf("\n");
}
*/


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

int Closest(unsigned int n)
{
	printf("%d\n", n & 65520);
	return (n & 65520);
}


int CheckBothSecondSixth(unsigned char ch)
{
	if (((ch >> 1) & 1) && ((ch << 1) & 8))
	{
		return 1;
	}
	
	return 0;
}

int CheckSecondOrSixth(unsigned char ch)
{
	if (((ch >> 1) & 1) || ((ch << 1) & 8))
	{
		return 1;
	}
	
	return 0;
}

int SwapThirdAndFifth(unsigned char ch)
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
		printf("3rd and 5th are the same bit (not replacing)\n");
	}
	else
	{
		printf("replacing\n");
		if (third_bit == 0)
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
	
	return 0;
}


void HelperPrintInBinary(int num)
{
	int i;
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

int MirrorBits(int n)
{
	int reversed_num = 0;
	int i = 0;
	
	for (i = 0; i < 32; i++)
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

	while (x & tmp) 
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
	
	while (n != 0)
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


