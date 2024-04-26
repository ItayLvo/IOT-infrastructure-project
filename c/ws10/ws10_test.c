#include <stdio.h>	/*printf*/
#include <stddef.h>	/*size_t*/
#include <string.h>	/*strcpy*/
#include "ws10.h"


int main()
{
	char str1[30];
	char str2[30];
	char str3[30];
	char str4[30];
	
	char *ptr1 = str1;
	char *ptr2 = str2;
	char *ptr3 = str3;
	char *ptr4;
	
	int c = 's';
	size_t count = 5;
	
	
	strcpy(ptr1,"1111111111");
	strcpy(ptr2,"2222222222");
	
	ptr1 = Memset(ptr1, c, count);
	ptr2 = Memcpy(ptr2, ptr1, count + 2);
	
	strcpy(ptr3,"0123456789");
	
	ptr4 = ptr3 + 3;
	ptr4 = Memmove(ptr4, ptr3, count);


	printf("%s\n%s\n%s\n%s\n", ptr1, ptr2, ptr3, ptr4);
	
	
	return 0;
}


