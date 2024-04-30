#include <stdio.h>	/* printf */
#include <string.h>	/* strlen */

#include "ws11.h"


static char *ReverseStringHelper(char *str);

int Atoi(const char *str)
{
	int result = 0;
	char negative_flag = *str;
	const char *runner = str;
	
	if (negative_flag == '-')
	{
		++runner;
	}
	
	while (*runner != '\0')
	{
		if (*runner < '0' || *runner > '9')
		{
			return 0;
		}
		result = result * 10;
		result = result + (*runner - '0');
		++runner;
	}
	
	if (negative_flag == '-')
	{
		result = result * -1;
	}
	
	return result;
}


char *Itoa(int num, char* buffer, int base)
{
	int remainder = 0;
	char *runner = buffer;
	char tmp_ch = '\0';
	int negative_flag_buffer = 0;
	
	if (num < 0)
	{
		if (10 == base)
		{
			*runner = '-';
			num = num * -1;
			++runner;
			negative_flag_buffer = 1;
		}
		else
		{
			return NULL;
		}
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
			tmp_ch = (char)((remainder - 10) + 'a');
		}
		*runner = tmp_ch;
		++runner;
		num = num / base;
	}
	*runner = '\0';
	
	buffer = ReverseStringHelper(buffer + negative_flag_buffer);
	
	if (negative_flag_buffer == 1)
	{
		--buffer;
	}
	
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
