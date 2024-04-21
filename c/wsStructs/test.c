#define _DEFAULT_SOURCE	/*to allow stdup*/
#include <stdio.h> 	/*printf*/
#include <string.h> 	/*strdup*/
#include "structs.h"

#define MAX2(a,b) (((a)>(b))?(a):(b))
#define MAX3(a,b,c) ((c)>(MAX2(a,b))?(c):(MAX2(a,b)))

#define SIZEOF_VAR(var) ((char *)(&var + 1) - (char *)(&var))


int main()
{
	element_t element_arr[3];
	int arr_size = 3;
	int i = 0;
	
	int x = 5;
	int *ptr_x = &x;
	float f = 3.3;
	float *ptr_f = &f;
	char str[256] = "itay";
	
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
	
	/*testing the MAX2/3 macros:*/
	printf("%d\n", MAX2(3, 5));
	printf("%d\n", MAX3(2,6,3));


	/*testing the SIZEOF_VAR macro:*/
	printf("%ld\n", SIZEOF_VAR(ptr_x));
	
	return 0;
}



