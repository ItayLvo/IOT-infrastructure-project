#include <stdio.h>	/* printf */
#include <stdlib.h>	/* atoi, itoa */
#include <assert.h>	/* assert */
#include <string.h>	/* strcspn */
#include "ws11.h"

void TestAtoi(char *str);
void TestItoa();

int main()
{
	char buffer[100];
	
	fputs("enter a string that represents a number: \n", stdout);
	fgets(buffer, sizeof(buffer), stdin);
	buffer[strcspn(buffer, "\n")] = '\0';

	TestAtoi(buffer);
	
	
	printf("----\nAtoi test cases: \n");
	TestAtoi("0");
	TestAtoi("12345678");
	TestAtoi("-12345678");
	TestAtoi("abc123abc");
	printf("------\n");
	printf("\nbase = 2, atoi(11001011) = %d\n", Atoi("11001011", 2));
	printf("expected result = 203\n");
	printf("\nbase = 16, atoi(1f2c) = %d\n", Atoi("1f2c", 16));
	printf("expected result = 7980\n");
	
	TestItoa();
	
	return 0;
}

void TestAtoi(char *str)
{
	printf("\natoi: %d\nAtoi: %d\n", atoi(str), Atoi(str, 10));
	assert(atoi(str) == Atoi(str, 10));
	printf("assert Atoi equal to atoi for string = '%s' passed\n", str);
}


void TestItoa()
{
	char str[100];
	char test[100];
	
	printf("\n-----------\nItoa test: \n");
	
	printf("number: %d\nbase: %d\tconverted String: %s\n", 1234, 10, Itoa(1234, str, 10));
	sprintf(test,"%d",1234);
	printf("\t\tsprintf result: %s\n\n", test);
	
	printf("base: %d\t\tConverted String: %s\n\n", 2, Itoa(1234, str, 2));
	
	printf("base: %d\t\tConverted String: %s\n", 8, Itoa(1234, str, 8));
	sprintf(test,"%o",1234);
	printf("\t\tsprintf result: %s\n\n", test);
	
	printf("base: %d\tConverted String: %s\n", 16, Itoa(1234, str, 16));
	sprintf(test,"%x",1234);
	printf("\t\tsprintf result: %s\n\n", test);
	
}
