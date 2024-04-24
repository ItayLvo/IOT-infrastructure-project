/*
name: itay
status: done
reviewed by evelin
*/

#include <stdlib.h>	/*malloc*/
#include <string.h>	/*strcmp*/
#include <stdio.h>	/*printf, size_t*/
#include "ws5.h"


flag_t flags_handlers_arr[] =
{
	{"-remove", CompareRemove, OperationRemove}, 
	{"-count", CompareCount, OperationCount}, 
	{"-exit", CompareExit, OperationExit},
	{"<", CompareAppend, OperationAppend}
};

int CompareAndOperate(char *user_input, char *argv[])
{
	int i = 0;
	typedef enum flag_status_t {FLAG_OFF, FLAG_ON} flag_status_t;
	flag_status_t special_string_flag = FLAG_OFF;
	
	if (NULL == user_input)
	{
		return 1;
	}
	
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
	
	return 0;
}


int CompareRemove(char *input)
{
	if (strcmp(input, "-remove") == 0)
	{
		return EQUAL;
	}

	return 0;
}


int CompareCount(char *input)
{
	if (strcmp(input, "-count") == 0)
	{
		return EQUAL;
	}
	
	return 0;
}


int CompareExit(char *input)
{
	if (strcmp(input, "-exit") == 0)
	{
		return EQUAL;
	}
	
	return 0;
}


int CompareAppend(char *input)
{
	if ('<' == input[0])
	{
		return EQUAL;
	}
	
	return 0;
}


STATUS OperationExit(char *filename, char *input)
{
    printf("closing program\n");
    return SUCCESS;
}



STATUS OperationRemove(char *filename, char *input)
{
 	if (0 == remove(filename))
 	{
		printf("file %s removed\n", filename);
		return SUCCESS;
	}
	else
	{
        	printf("failed to remove file %s\n", filename);
        	return FALIURE;
        }
}


STATUS OperationAppend(char *filename, char *input)
{
	FILE *file = NULL;
	size_t file_size = 0;
	char *file_content = NULL;
	
	if (NULL == filename) 
    	{
        	return FALIURE;
    	}
    	
    	file = fopen(filename, "r+");
    	if (NULL == file)
    	{
    		return FALIURE;
    	}
	
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	file_content = (char *)malloc(file_size + 1);
	if (NULL == file_content)
	{
		return FALIURE;
	}
	rewind(file);
	
	fread(file_content, 1, file_size, file);
	file_content[file_size] = '\0';
	rewind(file);

	fprintf(file, "%s\n%s", input + 1, file_content);

	free(file_content);
	fclose(file);

	printf("String %s appended to the start of file %s\n", input + 1, filename);
	
	return SUCCESS;
}



STATUS OperationCount(char *filename, char *input)
{
	FILE *file = NULL;
	int count = 0;
	char line[100];
	
	if (NULL == filename)
	{
		return FALIURE;
	}
	
	file = fopen(filename, "r");
	if (NULL == file) 
	{
		return FALIURE;
	}
    
	while (fgets(line, sizeof(line), file) != NULL)
	{
		++count;
	}
    
	fclose(file);
	printf("number of lines in file %s: %d\n", filename, count);
    
	return SUCCESS;
}


void AppendToFile(char *filename, char *str)
{
	FILE *file = NULL;
	if (NULL == filename)
	{
		return;
	}
    
	file = fopen(filename, "a");
	if (NULL == file)
	{
		return;
	}
    
	fprintf(file, "%s\n", str);
	fclose(file);
}

void Print(int num)
{
	printf("%d\n", num);
}

