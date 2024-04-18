#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*strcpy*/
#include "structs.h"


void AddNumbers(int n, void *value)
{
	int *intptr = (int *)value;
	*intptr = *intptr + n;
}

void AddString(int n, void *value)
{
	int original_len = strlen((char *)value);
	sprintf((char *)value + original_len, "%d", n);
}

void PrintFloat(void *value)
{
	printf("%f\n", *((float *)value));
}

void PrintString(void *value)
{
	printf("%s\n", (char *)value);
}

void PrintInt(void *value)
{
	printf("%d\n", *((int *)value));
}

void CleanNothing(void *value)
{
	/* do nothing */
}

void CleanString(void *value)
{
	free((char *)value);
}
