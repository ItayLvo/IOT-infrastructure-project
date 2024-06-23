#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */
#include <math.h>	/* pow */
#include <stdlib.h>	/* strtod */

#include "calculator.h"		/* calculator functions */
#include "stack.h"		/* stack data structure and functions */


e_status_t Calculate(const char *input, double *result)
{
	char *runner = (char *)input;
	char *end_of_current_num = NULL;
	char *str_remainder = NULL;
	double operand = 0.0;
	
	stack_t *operator_stack = StackCreate(100, sizeof(double));
	stack_t *operand_stack = StackCreate(100, sizeof(char));
	
	/*
	while ('\0' != *runner)
	{
	*/
		operand = strtod(runner, &str_remainder);
	/*
	}
	*/
	
	printf("%f\n", operand);
	printf("%s\n", str_remainder);
	
	operand = strtod(str_remainder, &str_remainder);
	printf("%f\n", operand);
	printf("%s\n", str_remainder);
	
	StackDestroy(operator_stack);
	StackDestroy(operand_stack);
	
	return CALC_SUCCESS;
}

