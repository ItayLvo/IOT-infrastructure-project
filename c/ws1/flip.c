#include <stdio.h>
#include "flip.h"

int flip(int n)
{
	int result = 0;
	
	while (n != 0)
	{
		result = result * 10;
		result += n % 10;
		n = n / 10;
	}
	
	return result;
}
