#ifndef __RECURSION_H__
#define __RECURSION_H__

#include <stddef.h> /* size_t */
#include "stack.h"

int Fibonacci(int element_index);

typedef struct node_t node_t;
node_t *FlipList(node_t *node);

size_t Strlen(char *str);
int Strcmp(char *str1, char *str2);
char *Strcpy(char *dest, char *src);
char *Strcat(char *dest, char *src);
char *Strstr(const char *haystack, const char *needle);

void SortStack(stack_t *stack);

/*** helper singly linked list functions for FlipList exercise ***/
node_t *CreateNode(void *data);
void PrintList(node_t *head, void (*PrintData)(void *));
void PrintList(node_t *head, void (*PrintData)(void *));
void FreeList(node_t *head);
void PrintInt(void *data);
void AppendNode(node_t **head, void *data);
/****************************************************************/



#endif /* __RECURSION_H__ */
