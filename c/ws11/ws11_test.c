#include <stdio.h>	/*printf*/

#include "ws11.h"


int main()
{
	char str[100];
	
	printf("Number: %d\nBase: %d\tConverted String: %s\n", 1567, 10, Itoa(1567, str, 10));
	
	printf("Base: %d\t\tConverted String: %s\n", 2, Itoa(1567, str, 2));
	
	printf("Base: %d\t\tConverted String: %s\n", 8, Itoa(1567, str, 8));
	
	printf("Base: %d\tConverted String: %s\n", 16, Itoa(1567, str, 16));
	
	
	return 0;
}


