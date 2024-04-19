#define _DEFAULT_SOURCE	/*to allow stdup*/
#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc, realloc*/
#include <string.h> /*strcpy*/
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
	char *string_place_holder = strdup(value);
	
	free(value);
	
	value = (char *)malloc(sizeof(char) * (original_len + n_number_of_digits) + 1);
	sprintf((char *)value, "%s", string_place_holder);
	sprintf((char *)value + original_len, "%d", n);
	
	free(string_place_holder);
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
	free((char *)value);
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

