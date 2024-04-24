#include <stdio.h>	/*printf*/
#include <stddef.h>	/*size_t*/
#include "ws10.h"


int main(int argc, char *argv[])
{
	char str[50];
	char *ptr = (char *)str;
	int c = 'z';
	size_t count = 10;
	int i = 0;
	
	for (i = 0; i < 50; ++i)
	{
		*ptr = 'a';
		++ptr;
	}
	
	ptr = str;
	
	ptr = Memset(ptr+5, c, count);
	
	for(i = 0; i < 50; ++i, ++ptr)
	{
		printf("%c\n", *ptr);
	}
	
	return 0;
}


