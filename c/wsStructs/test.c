#include <stdio.h> /*printf*/
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
	element_t element_arr[5];
	int x = 5;
	int *ptr_x = &x;
	element_t e_int;
	
	e_int.value = ptr_x;
	e_int.print = &PrintInt;
	e_int.add = &AddNumbers;
	e_int.clean =&CleanNothing;
	
	e_int.print(e_int.value);
	
	
	return 0;
}
