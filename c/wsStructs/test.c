#define _DEFAULT_SOURCE	/*to allow stdup*/
#include <stdio.h> 	/*printf*/
#include <string.h> 	/*strdup*/
#include "structs.h"

/*
typedef struct element {
	void *value = NULL;	
	PrintFunc print = NULL;
	AddFunc add = NULL;
	CleanFunc clean = NULL;
	} element_t;
	
void AddNumbers(int n, void *value);
void AddString(int n, void *value);
void PrintFloat(void *value);
void PrintString(void *value);
void PrintInt(void *value);
void CleanNothing(void *value);
void CleanString(void *value);
*/

int main()
{
	element_t element_arr[3];
	int arr_size = 3;
	int i = 0;
	
	int x = 5;
	int *ptr_x = &x;
	float f = 3.3;
	float *ptr_f = &f;
	char *str = strdup("itay");
	
	element_t e_int;
	element_t e_string;
	element_t e_float;
	
	e_int.value = ptr_x;
	e_int.print = &PrintInt;
	e_int.add = &AddInt;
	e_int.clean =&CleanNumbers;
	e_float.value = ptr_f;
	e_float.print = &PrintFloat;
	e_float.add = &AddFloat;
	e_float.clean = &CleanNumbers;
	e_string.value = str;
	e_string.print = &PrintString;
	e_string.clean = &CleanString;
	e_string.add = &AddString;
	
	element_arr[0] = e_int;
	element_arr[1] = e_float;
	element_arr[2] = e_string;
	
	for (i = 0; i < arr_size; i++)
	{
		element_arr[i].print(element_arr[i].value);
		element_arr[i].add(10, element_arr[i].value);
		element_arr[i].print(element_arr[i].value);
		element_arr[i].clean(element_arr[i].value);
	}

	return 0;
}




