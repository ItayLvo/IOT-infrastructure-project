/*
name: itay
status: done
reviewed by evelin
*/

#include <string.h>	/*strcmp*/
#include <stdio.h>	/*printf, stdin*/
#include "ws5.h"

int main()
{
	char user_input[100] = {0};
	char file_name[50] = {0};
	
	printf("enter file path: \n");
	fgets(file_name, 50, stdin);
	file_name[strcspn(file_name, "\n")] = '\0'; /* replaces \n with null terminator */
	
	while (strcmp(user_input, "-exit") != 0)
	{
		printf("\n");
		
		fgets(user_input, sizeof(user_input), stdin);
		user_input[strcspn(user_input, "\n")] = '\0';	/* replaces \n with null terminator */
		CompareAndOperate(user_input, file_name);
	}


	printf("enter file location: \n");

	
	return 0;
}

