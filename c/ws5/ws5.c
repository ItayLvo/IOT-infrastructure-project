/*
itay
finished code, not reviewed yet
*/


#include <stdlib.h>	/*malloc*/
#include <string.h>	/*strcmp*/
#include <stdio.h>	/*printf, size_t*/

#include "ws5.h"




int main(int argc, char *argv[])
{
	char input[100] = {0};
	
	struct flag_t flags_handlers_arr[] = {
	{"-remove", CompareRemove, OperationRemove}, 
	{"-count", CompareCount, OperationCount}, 
	{"-exit", CompareExit, OperationExit},
	{"<", CompareAppend, OperationAppend}
	};

	while (strcmp(input, "-exit") != 0)
	{
		int i = 0;
		printf("\n");
		
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = '\0';	/* replace enter with null terminator to create valid string */

		
		for (i = 0; i < 4; ++i)			/* loop through 4 possible options of flags_handlers_arr[] */
		{
		 	if (flags_handlers_arr[i].CompareFunc(input) == 1)
		 	{
				if (flags_handlers_arr[i].OperationFunc(argv[1], input) == TRUE)
				{
		        		if (strcmp(input, "-exit") == 0)
		        		{
		        			return 0;
		        		}
		        	}
		        	else	/* operation function returned FALSE */
		        	{
		        		return 1;
		        	}
			}
		}
		AppendToFile(argv[1], input);
	}

	return 0;
}


int CompareRemove(char *input)
{
	if (strcmp(input, "-remove") == 0)
	{
		return 1;
	}

	return 0;
}


int CompareCount(char *input)
{
    if (strcmp(input, "-count") == 0)
	{
		return 1;
	}
	
	return 0;
}


int CompareExit(char *input)
{
	if (strcmp(input, "-exit") == 0)
	{
		return 1;
	}
	
	return 0;
}


int CompareAppend(char *input)
{
	if (input[0] == '<')
	{
		return 1;
	}
	
	return 0;
}


enum STATUS OperationExit(char *filename, char *input)
{
    printf("closing program\n");
    return TRUE;
}




enum STATUS OperationRemove(char *filename, char *input)
{
 	if (remove(filename) == 0)
 	{
		printf("file %s removed\n", filename);
		return TRUE;
	}
	else
	{
        	printf("failed to remove file %s\n", filename);
        	return FALSE;
        }
}


enum STATUS OperationAppend(char *filename, char *input)
{
	FILE *file;
	size_t file_size = 0;
	char *file_content;
	
	if (NULL == filename) 
    	{
        	return FALSE;
    	}
    	
    	file = fopen(filename, "r+");
    	if (NULL == file)
    	{
    		return FALSE;
    	}
	
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	file_content = (char *)malloc(file_size + 1);
	if (file_content == NULL)
	{
		return FALSE;
	}
	rewind(file);
	
	fread(file_content, 1, file_size, file);
	file_content[file_size] = '\0';
	rewind(file);

	fprintf(file, "%s\n%s", input + 1, file_content);

	free(file_content);
	fclose(file);

	printf("String %s appended to the start of file %s\n", input + 1, filename);
	
	return TRUE;
}



enum STATUS OperationCount(char *filename, char *input)
{
	FILE *file;
	int count = 0;
	char line[100];
	
	if (NULL == filename)
	{
		return FALSE;
	}
	
	file = fopen(filename, "r");
	if (file == NULL) 
	{
		return FALSE;
	}
    
	while (fgets(line, sizeof(line), file) != NULL)
	{
		++count;
	}
    
	fclose(file);
	printf("number of lines in file %s: %d\n", filename, count);
    
	return TRUE;
}


void AppendToFile(char *filename, char *str)
{
    FILE *file;
    if (filename == NULL)
    {
    	return;
    }
    
    file = fopen(filename, "a");
    if (file == NULL)
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
