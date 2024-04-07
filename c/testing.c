#include <stdio.h>
#include <assert.h>
#include "strfuncs.h"

int main()
{

	char *st="12345";
	printf("%zu", StrLen(st));
	
}


size_t StrLen(const char *str)
{
	assert(str);
	size_t count = 0;
	
	while (str[count] != '\0')
	{
		count++;
	}
	
	return count;
}



