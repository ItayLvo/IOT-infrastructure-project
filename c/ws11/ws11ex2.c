#include <stdio.h>	/* printf */
#include <string.h>	/* strlen */

#define IS_BIG_ENDIAN (*(unsigned short *)"\0\xff" < 0x0100)
 


void PrintOnlyInTwoArrays(char *a, char *b, char *c);
void CheckEndian();
void CheckEndianMacro();
int main()
{
	char *a = "ab de";
	char *b = "abcde";
	char *c = " bc e";
	PrintOnlyInTwoArrays(a, b, c);
	
	CheckEndian();
	
	CheckEndianMacro();
	
	return 0;
}


void PrintOnlyInTwoArrays(char *a, char *b, char *c)
{
	char ascii[256] = {0};
	size_t i = 0;
	
	for (i = 0; i < 256; ++i)
	{
		ascii[i] = 'k';
	}
	
	for (i = 0; i < strlen(a); ++i)
	{
		ascii[a[i]] = '1';
	}
	
	for (i = 0; i < strlen(c); ++i)
	{
		ascii[c[i]] = '0';
	}
	
	for (i = 0; i < strlen(b); ++i)
	{
		if (ascii[b[i]] == '1')
		{
			printf("%c ", b[i]);
		}
	}
	
	printf("\n");
}

void CheckEndian()
{
	int x = 1;
	
	if (*(char*)(&x) == 1)
	{
		printf("\nlittle endian\n");
	}
	else
	{
		printf("\big endian\n");
	}
}

void CheckEndianMacro()
{

	if (IS_BIG_ENDIAN)
	{
		printf("macro: big endian\n");
	}
	else
	{
		printf("macro: little endian\n");
	}
}
