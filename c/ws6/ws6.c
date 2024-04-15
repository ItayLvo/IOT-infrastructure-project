#include <stdio.h> /*printf*/
#include "ws6.h"

int main()
{
	long i = 0;
	
	
	/* testing Pow2: x = 2, y = 3, should return 2*(2^3) = 16 */
	/* printf("%ld\n", Pow2(2, 3)); */
	
	
	
	
	/* testing for IsPowerOfTwo */
	/*
	for (i = 0; i < 17; i++)
	{
	printf("i = %d\t", i);
	printf("%ld\n", IsPowerOfTwo(i));
	}
	
	return 0;
	*/
	
	unsigned int arr[] = {1,2,3,7,4,15,11,1,13,0,4};
	PrintThreeBitsOn(arr, sizeof(arr));
	
	return 0;
	
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
			printf("%ld has 3 bits on exactly\n", arr[i]); 
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


