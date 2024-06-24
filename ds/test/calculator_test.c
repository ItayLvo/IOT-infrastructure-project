#include <stdio.h>	/* printf */

#include "calculator.h"


int main()
{
	char *st = "   2  +  8  + 2  -   5  - 7 - 2  ";
	char tmp[200] = {0};
	Calculate(st, NULL);
	
	return 0;
}
