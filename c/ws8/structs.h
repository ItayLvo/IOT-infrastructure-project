#ifndef __STRUCTS_H__
#define __STRUCTS_H__
#include <string.h> /*sprintf*/

typedef void(AddFunc)(int n, void *value);
typedef void(PrintFunc)(void *value);
typedef void(CleanFunc)(void *value);


typedef struct element {
	void *value;	/*pointer to int, or char[], or struct, etc*/
	PrintFunc *print;
	AddFunc *add;
	CleanFunc *clean;
	} element_t;
	

void AddInt(int n, void *value);
void AddFloat(int n, void *value);
void AddString(int n, void *value);
void PrintFloat(void *value);
void PrintString(void *value);
void PrintInt(void *value);
void CleanNumbers(void *value);
void CleanString(void *value);


#endif /* __STRUCTS_H__ */


