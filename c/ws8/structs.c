#define _DEFAULT_SOURCE	/*to allow stdup*/
#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc, realloc*/
#include <string.h> /*strcpy, strlen, memcpy*/
#include "structs.h"



static int NumberOfDigits (int n);

void AddInt(int n, void *value)
{
	int *int_ptr = (int *)value;
	*int_ptr = *int_ptr + n;
}

void AddFloat(int n, void *value)
{
	float *float_ptr = (float *)value;
	*float_ptr = *float_ptr + n;
}

void AddString(int n, void *value)
{
	int original_len = strlen((char *)value);
	int n_number_of_digits = NumberOfDigits(n);
	char *new_value = (char *)malloc(sizeof(char) * (original_len + n_number_of_digits + 1));
	if (new_value == NULL)
	{
		printf("failed malloc in AddString\n");
		return;
	}
	
	strcpy(new_value, (char *)value);
	sprintf(new_value + original_len, "%d", n);
	memcpy(value, new_value, strlen(new_value) + 1);
	
	free(new_value);
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

void CleanNumbers(void *value)
{
	/* do nothing */
}

void CleanString(void *value)
{
	/* do nothing */
}

static int NumberOfDigits (int n)
{
	int num_digits = 0;
	
	while (0 != n)
	{
		n /= 10;
		++num_digits;
	}
	return num_digits;
}

