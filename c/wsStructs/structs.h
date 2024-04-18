#ifndef __STRUCTS_H__
#define __STRUCTS_H__

typedef union types {
	int n;
	float f;
	char *string;
	} object_u;
	
	
typedef enum type_t = {INT, FLOAT, STRING} data_type_t;


typedef void(*AddFunc)(int n, type_t type, object_u obj);
typedef void(*PrintFunc)(object_u obj, type_t type);
typedef void(*CleanFunc)(object_u obj, type_t type);


typedef struct element {
	data_type_t type;	/*enum*/
	types_u object;		/*union*/
	PrintFunc print = NULL;
	AddFunc add = NULL;
	CleanFunc clean = NULL;
	} element_t;
	
	
generic_type_t *CreateGenericArray(int n);
generic_type_t *InitGenericArray(generic_type_t *arr, int index, generic_type_t element);



#endif /* __STRUCTS_H__ */
