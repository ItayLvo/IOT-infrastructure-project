#include <stdio.h>	/* printf */
#include <stdlib.h>	/* atoi, itoa */
#include <assert.h>	/* assert */
#include "ws11.h"

void TestAtoi(char *str);
void TestItoa();

int main()
{
	char str[100];
	puts("enter a string that represents a number: \n");
	scanf("%s", str);
	TestAtoi(str);
	TestAtoi("0");
	TestAtoi("12345678");
	TestAtoi("-12345678");
	TestAtoi("4294967295"); /* 2^32 */
	TestAtoi("abc123abc");
	
	TestItoa();
	

	return 0;
}

void TestAtoi(char *str)
{
	printf("\natoi: %d\nAtoi: %d\n", atoi(str), Atoi(str));
	assert(atoi(str) == Atoi(str));
	printf("assert Atoi equal to atoi for string = '%s' passed\n", str);
}

void TestItoa()
{
	char str[100];

	printf("\nItoa test: \n");
	
	printf("number: %d\nbase: %d\tconverted String: %s\n", 1234, 10, Itoa(1234, str, 10));
	
	printf("base: %d\t\tConverted String: %s\n", 2, Itoa(1234, str, 2));
	
	printf("base: %d\t\tConverted String: %s\n", 8, Itoa(1234, str, 8));
	
	printf("base: %d\tConverted String: %s\n", 16, Itoa(1234, str, 16));
}
