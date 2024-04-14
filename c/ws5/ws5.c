#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ws5.h"

struct print_me
{
   	int data;
   	void (*Print)(int);
};


struct flag
{
    char *inputString;
    int (*CompareFunc)(char *);
    void (*OperationFunc)(char *, char *);
};


int main(int argc, char *argv[])
{
	char input[100];
	struct print_me array[10];
	int i = 0;
	
	struct flag flags[] = {
	{"-remove", CompareRemove, OperationRemove}, 
	{"-count", CompareCount, OperationCount}, 
	{"-exit", CompareExit, OperationExit},
	{"<", CompareAppend, OperationAppend}
	};



	while (1)
	{
		int handled = 0;
		int i = 0;
		printf("\n");
		
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = '\0';	/* replace enter with null terminator to create valid string */

		
		for (i = 0; i < 4; ++i)			/* loop through 4 possible options of flags[] - "chain of responsibility" design pattern */
		{
		 	if (flags[i].CompareFunc(input) == 1)
		 	{
				flags[i].OperationFunc(argv[1], input);
		        	handled = 1;
		        	break;
			}
		}
		
		if ((handled == 1 && strcmp(input, "-exit") == 0) || strcmp(input, "") == 0)
		{
			break;
		}
		
		else if (handled == 0)
		{
			AppendToFile(argv[1], input);
		}
	}

	

	return 0;
}



int CompareRemove(char *input)
{
	if (strcmp(input, "-remove") == 0)
	{
		return 1;
	}
	
	else
	{
		return 0;
	}
}


int CompareCount(char *input)
{
    if (strcmp(input, "-count") == 0)
	{
		return 1;
	}
	
	else
	{
		return 0;
	}
}


int CompareExit(char *input)
{
	if (strcmp(input, "-exit") == 0)
	{
		return 1;
	}
	
	else
	{
		return 0;
	}
}


int CompareAppend(char *input)
{
	if (input[0] == '<')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void OperationExit(char *filename, char *input)
{
    printf("closing program\n");
}





void OperationRemove(char *filename, char *input)
{
 	if (remove(filename) == 0)
 	{
		printf("file %s removed\n", filename);
	}
	else
	{
        	printf("failed to remove file %s.\n", filename);
        }
}


void OperationAppend(char *filename, char *input)
{
	FILE *file = fopen(filename, "r+");
	size_t file_size;
	char *fileContent;
	
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fileContent = (char *)malloc(file_size + 1);
	rewind(file);
	fread(fileContent, 1, file_size, file);
	fileContent[file_size] = '\0';

	rewind(file);

	fprintf(file, "%s\n%s", input + 1, fileContent);

	free(fileContent);
	fclose(file);

	printf("String %s appended to the start of file %s\n", input + 1, filename);
}



void OperationCount(char *filename, char *input)
{
    FILE *file = fopen(filename, "r");
    int count = 0;
    char line[100];
    
    if (file == NULL) 
    {
        return;
    }
    
    while (fgets(line, sizeof(line), file) != NULL)
    {
        count++;
    }
    
    fclose(file);
    printf("number of lines in file %s: %d\n", filename, count);
}


void AppendToFile(char *filename, char *str)
{
    FILE *file = fopen(filename, "a");
    fprintf(file, "%s\n", str);
    fclose(file);
}

void Print(int num)
{
	printf("%d\n", num);
}
