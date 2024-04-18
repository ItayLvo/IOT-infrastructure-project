#include <stdio.h> /*printf*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*strcpy*/
#include "structs.h"

/*
typedef union types {
	int n;
	float f;
	char *string;
	} object_u;
	
	
typedef enum type_t = {INT, FLOAT, STRING} data_type_t;


typedef void(*AddFunc)(int n, type_t type);
typedef void(*PrintFunc)(object_u obj, type_t type);
typedef void(*CleanFunc)(object_u obj, type_t type);


typedef struct element {
	data_type_t type;	
	types_u object;		
	PrintFunc print = NULL;
	AddFunc add = NULL;
	CleanFunc clean = NULL;
	} element_t;
	
*/


void *AddFunc(int n, type_t type, object_u obj)
{
	
}

generic_type_t *CreateGenericArray(int n)
{
	generic_type_t *arr = malloc(sizeof(generic_type_t) * n);
	return arr;
}


generic_type_t *InitGenericArray(generic_type_t *arr, int index, generic_type_t element)
{
	arr[index].type = element.type;
	arr[index].number = element.number;
	arr[index].f = element.f;
	arr[index].string = strdup(arr[index].string, element.string);
	return arr;
}



