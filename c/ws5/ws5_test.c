/*
name: itay
status: done
reviewed by evelin
*/

#include <string.h>	/*strcmp*/
#include <stdio.h>	/*printf, stdin*/
#include "ws5.h"

int main(int argc, char *argv[])
{
	char user_input[100] = {0};
	
	while (strcmp(user_input, "-exit") != 0)
	{
		printf("\n");
		
		fgets(user_input, sizeof(user_input), stdin);
		user_input[strcspn(user_input, "\n")] = '\0';	/* replaces enter with null terminator */
		CompareAndOperate(user_input, argv);
	}


	printf("enter file location: \n");

	
	return 0;
}

