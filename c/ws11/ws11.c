#include <stdio.h>	/* printf */
#include <string.h>	/* strlen */
#include <math.h>	/* pow */

#include "ws11.h"

static char *ReverseStringHelper(char *str);
static int ReverseDigits(int num);

#define ASCII_CHR_TO_DEC 87

int Atoi(const char *str, int base)
{
	int result = 0;
	char negative_flag = *str;
	const char *runner = str;
	int digit_counter = strlen(str);
	
	if (negative_flag == '-')
	{
		++runner;
		--digit_counter;
	}
	
	while ('\0' != *runner)
	{
		if (10 >= base && (*runner < '0' || *runner > '9'))
		{
			return 0;
		}
		
		if (*runner >= 'a' && *runner <= 'z')
		{
			result += ((int)*runner - ASCII_CHR_TO_DEC) * pow(base, digit_counter - 1);
		}
		else
		{
			result += (*runner - '0') * pow(base, digit_counter - 1);
		}
		
		++runner;
		--digit_counter;
	}
	
	/* result = ReverseDigits(result); */
	
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
		if (9 >= remainder)
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


static int ReverseDigits(int num) 
{ 
	int rev_num = 0; 
	while (num > 0)
	{ 
		rev_num = rev_num * 10 + num % 10; 
		num = num / 10; 
	} 
	return rev_num; 
} 
