#include <stdio.h>	/* printf */

#include "calculator.h"


int main()
{
	char *st = "1+5/4    +1 - (  1+2)";
	char tmp[200] = {0};
	Calculate(st, NULL);
	
	return 0;
}
