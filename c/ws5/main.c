/*
name: itay
status: done
reviewed by evelin
*/

#include <string.h>	/*strcmp*/
#include <stdio.h>	/*printf, stdin*/
#include "ws5.h"

flag_t flags_handlers_arr[] =
{
	{"-remove", CompareRemove, OperationRemove}, 
	{"-count", CompareCount, OperationCount}, 
	{"-exit", CompareExit, OperationExit},
	{"<", CompareAppend, OperationAppend}
};


int main(int argc, char *argv[])
{
	char user_input[100] = {0};
	typedef enum flag_status_t {FLAG_OFF, FLAG_ON} flag_status_t;
	flag_status_t special_string_flag = FLAG_OFF;

	while (strcmp(user_input, "-exit") != 0)
	{
		int i = 0;
		printf("\n");
		
		fgets(user_input, sizeof(user_input), stdin);
		user_input[strcspn(user_input, "\n")] = '\0';	/* replaces enter with null terminator */
		
		for (i = 0; i < 4; ++i)			/* loop through 4 possible options of flags_handlers_arr[] */
		{
		 	if (flags_handlers_arr[i].CompareFunc(user_input) == EQUAL)
		 	{
		 		special_string_flag = FLAG_ON;
				if (flags_handlers_arr[i].OperationFunc(argv[1], user_input) == SUCCESS)
				{
		        		if (strcmp(user_input, "-exit") == 0)
		        		{
		        			return 0;
		        		}
		        	}
		        	else	/* operation function returned FALIURE */
		        	{
		        		return 1;
		        	}
			}
		}
		if (special_string_flag == FLAG_OFF)
		{
			AppendToFile(argv[1], user_input);
		}
		special_string_flag = 0;
	}

	return 0;
}

