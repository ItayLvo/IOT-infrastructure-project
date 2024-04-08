#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */
#include <assert.h>	/* assert */
#include <string.h>	/*strlen in main() for testing purposes, tolower */
#include "strings.h"	/* all implementations of string.h exercises */
#include <ctype.h>	/* tolower */

int main()
{

	char st[] = "abEs";
	char st2[] = "abe";
	printf("%zu \n\n", strlen(st));
	printf("%zu \n\n", sizeof(st));
	char *dest = (char *)malloc(strlen(st) +1 );
	
	if (dest == NULL)
		return 1;
	
	char *ch = StrChr(st, 's');
	
	printf("%c \n", *ch);
	
	free(dest);
	
	return 0;
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


char* StrCpy(char* destination, const char* source)
{
	assert(destination);
	assert(source);
	int i = 0;
	while (source[i] != '\0')
	{
		destination[i] = source[i];
		i++;
	}
	destination[i] = source[i];
	return destination;
}

char *StrnCpy(char *dest, const char *src, size_t n)
{
	assert(dest);
	assert(src);
	int i = 0;
	while (src[i] != '\0'&& i < n)
	{
		dest[i] = src[i];
		i++;
	}
	
	
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	
	}
	dest[i] = '\0';
	
	return dest;
}


int StrnCmp(const char *str1, const char *str2, size_t n)
{
	assert(str1);
	assert(str2);
	int count = 1;
	while (*str1 == *str2 && *str1 != '\0' && *str2 != '\0' && count < n)
	{
		str1++;
		str2++;
		count++;
	}
	return ((*str1) - (*str2));
}


int StrCaseCmp(const char *s1, const char *s2)
{
 	assert(s1);
	assert(s2);
	while (tolower(*s1) == tolower(*s2) && *s1 != '\0' && *s2 != '\0')
	{
		s1++;
		s2++;
	}
	return ((*s1) - (*s2));
 }
 
 
char *StrChr(const char *str, int c)
{
	const char *ptr = str;
	while (*ptr != '\0')
	{
		if (*ptr == c)
		{
			return ptr;
		}
		ptr++;
	}
	return NULL;
}



