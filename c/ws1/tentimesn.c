#include <stdio.h>
#include "tentimesn.h"



int PowerTen(int n)
{
	int result = 1;
	int i;
	
	for (i = 0; i < n; i++)
	{
		result = result * 10;
	}
	
	return result;
}
