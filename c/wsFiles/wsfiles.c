#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wsfiles.h"

struct print_me
{
   	int data;
   	void (*Print)(int);
};



int main(int argc, char *argv[])
{
	char input[100];
	struct print_me array[10];
	int i = 0;
	
	for (i = 0; i < 10; i++)
	{
		array[i].data = i;
		array[i].Print = &Print;
	}

	for (i = 0; i < 10; i++)
	{
		array[i].Print(array[i].data);
	}


    	printf("Enter strings: \n");
    	while (1)
    	{
        	printf("\n");
        	fgets(input, sizeof(input), stdin);
        	
        	input[strcspn(input, "\n")] = '\0';
        	
        	if (strcmp(input, "") == 0)
           	{
           		break;
           	}
        
        AppendToFile(argv[1], input);
    	}
	

	return 0;
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
