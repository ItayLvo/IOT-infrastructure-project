#include <stdio.h>	/* printf */

#include "calculator.h"


int main()
{
	char *st = "5-3+1-4";
	char tmp[200] = {0};
	double result = 1.1;
	Calculate(st, &result);
	
	printf("result = %f\n", result);
	
	return 0;
}
