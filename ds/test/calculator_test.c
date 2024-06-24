#include <stdio.h>	/* printf */

#include "calculator.h"


int main()
{
	char *st = "-(3-2*5-2)+(3)-5*(2-3*4)";
	double result = 1.1;
	Calculate(st, &result);
	
	printf("result = %f\n", result);
	
	return 0;
}


/*
p *(double *)StackPeek(operand_stack)
p *(char *)StackPeek(operator_stack)
*/
