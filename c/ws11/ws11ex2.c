#include <stdio.h>	/* printf */
#include <string.h>	/* strlen */

void PrintOnlyInTwoArrays(char *a, char *b, char *c);

int main()
{
	char *a = "ab de";
	char *b = "abcde";
	char *c = " bc e";
	PrintOnlyInTwoArrays(a, b, c);
	
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
