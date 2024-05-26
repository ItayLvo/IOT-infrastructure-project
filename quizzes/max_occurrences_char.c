#include <stdio.h>


size_t MaxCharLUT(size_t *char_LUT);

static size_t ascii_LUT[256] = {0}; 

void OneMillionChars(char ch)
{	
	if('0' == ch)
	{
		PrintMax(ascii_LUT);
	}
	else
	{
		++ascii_LUT[ch - '0'];
	}	
}



static void PrintMax(size_t *look_up_table)
{
	size_t max = 0;
	size_t i = 0;
	char result = 0;
	
	for (i = 0; i < 256; ++i)
	{
		if(look_up_table[i] > max)
		{
			max = look_up_table[i];
			result = i + '0';
		}
	}
	
	
	printf("%c", result);
	
	for (i = 0; i < 256; ++i)
	{
		look_up_table[i] = 0;
	}

}


int main(void)
{
	OneMillionChars('g');
	OneMillionChars('d');
	OneMillionChars('a');
	OneMillionChars('b');
	OneMillionChars('c');
	OneMillionChars('c');
	OneMillionChars('j');
	OneMillionChars('1');
	OneMillionChars('5');
	
	return 0;
}

