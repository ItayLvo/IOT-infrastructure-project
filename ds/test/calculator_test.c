#include <stdio.h>	/* printf */

#include "calculator.h"


int main()
{
	char *st = "  ( 5 +(2-3   ) )";
	double result = 1.1;
	e_status_t status = Calculate(st, &result);
	
	if (status == CALC_SUCCESS)
	{
		printf("result = %f\n", result);
	}
	else
	{
		printf("failed: %d\n", status);
	}
	
	return 0;
}
