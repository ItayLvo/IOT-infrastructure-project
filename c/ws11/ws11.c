#include <stdio.h>	/* printf */
#include <string.h>	/* strlen */

#include "ws11.h"


static char *ReverseStringHelper(char *str);

int Atoi(const char *str)
{
	int result = 0;
	return 0;
}


char *Itoa(int num, char* buffer, int base)
{
	int remainder = 0;
	char *runner = buffer;
	char tmp_ch = '\0';
	
	if (num < 0)
	{
		*runner = '-';
		++runner;
	}
	
	while (num > 0)
	{
		remainder = num % base;
		
		if (remainder <= 9)
		{
			tmp_ch = (char)(remainder + '0');
		}
		else
		{
			tmp_ch = (char)((remainder - 10) + 'A');
		}
		*runner = tmp_ch;
		++runner;
		num = num / base;
	}
	*runner = '\0';
	
	buffer = ReverseStringHelper(buffer);
	
	return buffer;
}



static char *ReverseStringHelper(char *str)
{
	int length = strlen(str);
	char *start = str;
	char *end = str + length - 1;
	char temp = '\0';
	
	while (start < end)
	{
		temp = *start;
		*start = *end;
		*end = temp;
		--end;
		++start;
	}
	return str;
}
