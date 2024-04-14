#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */
#include <assert.h>	/* assert */
#include <string.h>	/*strlen in main() for testing purposes, tolower */
#include <ctype.h>	/* tolower */
#include "ws2.h"	

int main()
{
	return 0;
}


void SevenBoom(int from, int to)
{
	int i;
	char str[3];
	for (i = from; i < to; i++)
	{
		sprintf(str, "%d", i);
		/* printf("--%s--\n", str); */
		if (strchr(str, 7 + '0') || i % 7 == 0)
		{
			printf("BOOM\n");
		}
		else
		{
			printf("%d\n", i);
		}
	}
}

size_t StrLen(const char *str)
{
	size_t count = 0;
	
	
	assert(str);
	
	
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
	int i = 0;
	assert(destination);
	assert(source);
	
	while (source[i] != '\0')
	{
		destination[i] = source[i];
		i++;
	}
	destination[i] = '\0';
	return destination;
}

char *StrnCpy(char *dest, const char *src, size_t n)
{
	size_t i = 0;
	assert(dest);
	assert(src);
	
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
	size_t count = 1;
	assert(str1);
	assert(str2);
	
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
			return (char *)ptr;
		}
		ptr++;
	}
	return NULL;
}


char *StrDup(const char *s)
{
	int i = 0;
	char *newstr = (char *)malloc(sizeof(char) * strlen(s) + 1);
	assert(s);
	
	if (newstr == NULL)
	{
		return NULL;
	}
	
	while (s[i] != '\0')
	{
		newstr[i] = s[i];
		i++;
	}
	newstr[i] = s[i];
	
	return newstr;
}


char *StrCat(char *s1, const char *s2)
{
	char *ptr1 = s1;
	const char *ptr2 = s2;
	assert(s1);
	assert(s2);
	
	ptr1 = ptr1 + strlen(s1); /* initialize ptr1 to point at the null terminator of s1 */
	
	while (*ptr2 != '\0')
	{
		*ptr1 = *ptr2;
		ptr1++;
		ptr2++;
	}
	*ptr1 = '\0';
	
	return s1;
}


char *StrnCat(char *s1, const char *s2, size_t n)
{
	size_t count = 0;
	char *ptr1 = s1;
	const char *ptr2 = s2;
	
	assert(s1);
	assert(s2);
	
	ptr1 = ptr1 + strlen(s1); /* initialize ptr1 to point at the null terminator of s1 */
	
	while (*ptr2 != '\0' && count < n)
	{
		*ptr1 = *ptr2;
		ptr1++;
		ptr2++;
		count++;
	}
	
	*ptr1 = '\0';
	
	return s1;
}


char *StrStr(const char *haystack, const char *needle)
{
	int count = 0;
	int string_size = strlen(needle);
	const char *ptrhay = haystack;
	const char *ptrneedle = needle;
	const char *ptrresult = NULL;
	
	assert(haystack && needle);
	if (needle == NULL)
	{
		return (char *)haystack;
	}
	
	while (*ptrhay != '\0')
	{
		
		while (*ptrhay != '\0' && *ptrhay == *ptrneedle)
		{
			
			if (count == 0)
			{
				ptrresult = ptrhay;
			}
			count++;
			ptrhay++;
			ptrneedle++;
		}
		
		if (count == string_size)
		{
			return (char *)ptrresult;
		}
		else
		{
			count = 0;
			ptrneedle = needle;
			ptrresult = NULL;
		}
		
		if (*ptrhay == '\0')
			return (char *)ptrresult;
		else
			ptrhay++;
		
	}
	
	return (char *)ptrresult;
}



size_t StrSpn(const char *str1, const char *str2)
{
	assert(str1 && str2);
	
	if(StrStr(str1, str2) == str1)
	{
		return strlen(str2);
	}
	
	else
		return 0;
}


/*
char *StrTok(char *str, const char *delim)
{	
	static int count = 0;
	int tmpcount = 0;
	char *ptr = str;
	char *result;
	char *tmpptr;
	
	
	printf("entered strtok\n");
	
	if (str != NULL)	
	{
		result = (char *)realloc(result, strlen(str));
		tmpptr = result;
		
		while (*ptr != '\0' &&  StrChr(delim, *ptr) == NULL)
		{
			*tmpptr = *ptr;
			ptr++;
			tmpptr++;
		}
		*tmpptr = '\0';
		count++;
		return result;
	}
	
	else			
	{
		while (tmpcount < count)
		{
			while (*ptr != '\0' && StrChr(delim, *ptr) == NULL)
			{
				ptr++;
			}
			
			if (*ptr == '\0')	
			{
				return NULL;
			}
			else			
			{
				ptr++;
			}
			tmpcount++;
		}
		
		result = (char *)malloc(sizeof(char) * strlen(str));
		tmpptr = result;
		
		while (*ptr != '\0' &&  StrChr(delim, *ptr) == NULL)
		{
			*tmpptr = *ptr;
			ptr++;
			tmpptr++;
		}
		*tmpptr = '\0';
		count++;
		
		return result;
	}
	
}
*/


int IsPalindrome(char *str)
{
	char *ptrstart = str;
	char *ptrend = (str + strlen(str));
	ptrend--;
	
	while (ptrstart <= ptrend)
	{
		if (*ptrstart != *ptrend)
		{
			return 0;
		}
		
		ptrstart++;
		ptrend--;
	}
	return 1;
}


char *DeleteSpace(char *str)
{
	char *ptrstart = str;
	char *ptrend = str + strlen(str);
	ptrend--;
	
	printf("started func %s\n", ptrstart);
	
	while (*ptrstart == ' ')
	{
		str++;
		ptrstart++;
		printf("first while: %s\n\n", ptrstart);
	}
	
	while(*ptrend == ' ')
	{
		*ptrend = '\0';
		ptrend--;
		printf("second while: %s\n\n", ptrstart);
	}
	
	while (*ptrstart != '\0')
	{
		while ((strlen(ptrstart) > 1) && (*ptrstart == ' ' && *(ptrstart + 1) == ' '))
		{

			ptrstart = StrCpy(ptrstart, (ptrstart + 1));
			printf("third while: %s\n\n", ptrstart);
		}
		ptrstart++;
	}
	
	return str;
	
}











