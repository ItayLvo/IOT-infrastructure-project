#include <stdio.h>	/* printf */

#include "calculator.h"


int main()
{
	char *st = "((1/8))";
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



/*
	tests:
    	char str[100] = "7+8"; 				  Result = 15 status = CALC_SUCCESS 
    	char str2[100] = "8+8*3+-2^5";		  Result =  0 status = CALC_SUCCESS	  
    	char str3[100] = "8+8*3-2^";		  Result =  0 status = CALC_SYNTAX_ERROR  
    	char str4[100] = "2/0";				  Result =  0 status = CALC_MATH_ERROR    
    	char str5[100] = "8++8*((3-2)*5)";	  Result = 48 status = CALC_SUCCESS 	   
    	char str6[100] = "3-2)*5";			  Result =  0 status = CALC_SYNTAX_ERROR  
    	char str7[100] = "(3-2)*5+ 5*(4+4+4"; Result =  0 status = CALC_SYNTAX_ERROR 
    	char str8[100] = "4-3-2-1\n";		  Result =  -2 status = CALC_SUCCESS  
    	char str9[100] = "4^3^2^1\n";		  Result =  4096 status = CALC_SUCCESS 
*/
