/*
name: itay
status: fixed after CR
reviewed by evelin & vova
*/

#include <stdlib.h>	/*malloc*/
#include <string.h>	/*strcmp*/
#include <stdio.h>	/*printf, size_t*/

#include "ws5.h"

#define EQUAL 1


STATUS CompareAndOperate(char *user_input, char *file_name);
STATUS OperationCount(char *filename, char *input);
STATUS OperationPrepend(char *filename, char *input);
STATUS OperationRemove(char *filename, char *input);
STATUS OperationExit(char *filename, char *input);
STATUS AppendToFile(char *filename, char *input);

int ComparePrepend(char *input);
int CompareExit(char *input);
int CompareCount(char *input);
int CompareRemove(char *input);
int CompareDefaultAppend(char *input);

typedef struct special_input_t
{
    char *inputString;
    int (*CompareFunc)(char *);
    STATUS (*OperationFunc)(char *, char *);
} special_input_t;


special_input_t flags_handlers_arr[] =
{
	{"-remove", CompareRemove, OperationRemove}, 
	{"-count", CompareCount, OperationCount}, 
	{"-exit", CompareExit, OperationExit},
	{"<", ComparePrepend, OperationPrepend},
	{" ", CompareDefaultAppend, AppendToFile}
};

STATUS Logger(void)
{
	STATUS program_status = SUCCESS;
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
		program_status = CompareAndOperate(user_input, file_name);
	}
	
	return program_status;
}


STATUS CompareAndOperate(char *user_input, char *file_name)
{
	int i = 0;
	STATUS program_status = SUCCESS;
	
	if (NULL == user_input)
	{
		return 1;
	}
	
	for (i = 0; i < 5; ++i)		/* loop through 5 possible options of flags_handlers_arr[]
						last option (5th) is the default option */
	{
	 	if (flags_handlers_arr[i].CompareFunc(user_input) == EQUAL)
	 	{
	 		program_status = flags_handlers_arr[i].OperationFunc(file_name, user_input);
			if (program_status == SUCCESS)
			{
	        		if (strcmp(user_input, "-exit") == 0)
	        		{
	        			return 0;
	        		}
	        		break;	/* special input proccessed, stops comparing */
	        	}
	        	else	/* operation function returned failure */
	        	{
	        		return program_status;
	        	}
		}
	}
	
	return program_status;
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


int ComparePrepend(char *input)
{
	if ('<' == input[0])
	{
		return EQUAL;
	}
	
	return 0;
}


int CompareDefaultAppend(char *input)
{
	return EQUAL;
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
        	return FILE_FALIURE;
        }
}


STATUS OperationPrepend(char *filename, char *input)
{
	FILE *file = NULL;
	size_t file_size = 0;
	char *file_content = NULL;
	
	if (NULL == filename) 
    	{
        	return FILE_FALIURE;
    	}
    	
    	file = fopen(filename, "r+");
    	if (NULL == file)
    	{
    		return FILE_FALIURE;
    	}
	
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	file_content = (char *)malloc(file_size + 1);
	if (NULL == file_content)
	{
		return MALLOC_FALIURE;
	}
	rewind(file);
	
	fread(file_content, 1, file_size, file);
	file_content[file_size] = '\0';
	rewind(file);

	fprintf(file, "%s\n%s", input + 1, file_content);

	free(file_content);
	fclose(file);

	printf("String %s pre-pended to the start of file %s \n", input + 1, filename);
	
	return SUCCESS;
}



STATUS OperationCount(char *filename, char *input)
{
	FILE *file = NULL;
	int count = 0;
	char line[100];
	
	if (NULL == filename)
	{
		return FILE_FALIURE;
	}
	
	file = fopen(filename, "r");
	if (NULL == file) 
	{
		return FILE_FALIURE;
	}
    
	while (fgets(line, sizeof(line), file) != NULL)
	{
		++count;
	}
    
	fclose(file);
	printf("number of lines in file %s: %d\n", filename, count);
    
	return SUCCESS;
}


STATUS AppendToFile(char *filename, char *str)
{
	FILE *file = NULL;
	if (NULL == filename)
	{
		return FILE_FALIURE;
	}
    
	file = fopen(filename, "a");
	if (NULL == file)
	{
		return FILE_FALIURE;
	}
    
	fprintf(file, "%s\n", str);
	fclose(file);
	
	return SUCCESS;
}




void Print(int num);

struct print_me
{
   	int data;
   	void (*Print)(int);
};

void Print(int num)
{
	printf("%d\n", num);
}

