#include <stdio.h>	/*printf*/
#include "ws4.h"


int main()
{
	char ch = 0;
	
	printf("1: LUT implementation\n2: Switch-case implementation\n3:If-else implementation\n");
	scanf("%c", &ch);
	switch (ch)
	{
		case '1':
			LUTImplementation();
			break;
		case '2':
			SwitchImplementation();
			break;
		case '3':
			IfImplementation();
			break;
		default: break;
	}
	
	return 0;
}

