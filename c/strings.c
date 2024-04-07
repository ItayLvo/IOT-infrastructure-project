#include <stdio.h>
#include <assert.h>
#include "strfuncs.h"

int main()
{

	char *st="abcde";
	printf("%zu\n\n", StrLen(st));
	
	char *str2 = "abcee";
	
	printf("%d\n\n", StrCmp(st, str2));
	
	int num = 4;
	
	num = 2 * num++;
	
	
	printf("%d \n", num);
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


int StrCmp(const char *str1, const char *str2)
{
	assert(str1);
	assert(str2);
	while (*str1 == *str2 && *str1 != '\0' && *str2 != '\0')
	{
		str1++;
		str2++;
	}
	return ((*str1) - (*str2));
}
