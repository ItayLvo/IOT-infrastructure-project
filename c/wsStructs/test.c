#include <stdio.h> /*printf*/
#include "structs.h"



int main()
{
	generic_type_t *arrptr = CreateGenericArray(5);
	generic_type_t test_struct = {INT, 123, 1.5, "hello"};
	arrptr = InitGenericArray(arrptr, 0, test_struct);
	
	
	return 0;
}
