#include <stdio.h>	/*printf*/
#include <stddef.h>	/*size_t*/
#include <string.h>	/*strcpy*/
#include "ws10.h"


int main(int argc, char *argv[])
{
	char str[30];
	char str2[30];
	char *ptr = str;
	char *ptr2 = str2;
	int c = 'z';
	size_t count = 11;
	
	str[30]='\0';
	str2[30]='\0';
	
	strcpy(ptr,"!!!!!");
	strcpy(ptr2,"?????");
	
	ptr = Memset(ptr, c, 2);
	ptr2 = Memcpy(ptr2, ptr, 3);
	
	printf("%s\n%s\n", ptr, ptr2);
	
	
	return 0;
}


