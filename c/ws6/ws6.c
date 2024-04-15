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
	
	
}


int AddOne(int x) 
{ 
	int tmp = 1; 
     
	/* flip all the bits until we find a 0 */

	while (x & tmp) 
	{ 
		x = x ^ tmp; 
		tmp  = tmp << 1; 
	} 
	/* flip the rightmost 0 bit */

	x = x ^ m; 
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


