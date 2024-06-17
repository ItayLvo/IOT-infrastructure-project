/*
date: 
reviewer:
status: 
*/

#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */
#include <string.h>	/* strncmp */

#include "recursion.h"	/* recursion functions */



struct node_t
{
	struct node_t *next;
	void *data;
};

int Fibonacci(int element_index)
{
	if (element_index == 1)
	{
		return 1;
	}
	
	if (element_index == 0)
	{
		return 0;
	}


	return (Fibonacci(element_index - 1) + Fibonacci(element_index - 2));
}


node_t *FlipList(node_t *node)
{
	node_t *head;
	
	/* if empty/invalid list */
	if (node == NULL)
	{
		return NULL;
	}
	
	/* stop condition: when the recursion reaches the last node before NULL */
	if (node->next == NULL)
	{
		return node;
	}
	
	/* set the new head to be the last node */
	head = FlipList(node->next);
	
	/* set the next node to point to current node */
	node->next->next = node;
	
	/* set current node to point to NULL */
	node->next = NULL;
	
	/* return the new head */
	return head;
	
}


/*
void SortStack(stack_t *stack)
{
	int a, b;
	
	if (IsEmpty(stack))
	{
		return;
	}
	
	a = StackPop(stack);
	
	if (!IsEmpty(stack))
	{
		b = StackPop(stack);
	}	
}
*/

size_t Strlen(char *str)
{
	if (*str == '\0')
	{
		return 0;
	}
	
	return 1 + (Strlen(++str));
}


int Strcmp(char *str1, char *str2)
{
	if (*str1 == '\0' && *str2 == '\0')
	{
		return 1;
	}
	
	if (*str1 != *str2)
	{
		return 0;
	}
	
	return (Strcmp(++str1, ++str2));
}


char *Strcpy(char *dest, char *src)
{
	char *original_dest_head = dest;
	
	if (*src == '\0')
	{
		*dest = *src;
		return NULL;
	}
	
	*dest = *src;
	
	Strcpy(++dest, ++src);
	
	return original_dest_head;
}






char *Strcat(char *dest, char *src)
{
	char *original_dest_head = dest;
	if (*dest != '\0')
	{
		Strcat(++dest, src);
	}
	else
	{
		Strcpy(dest, src);
		return original_dest_head;
	}
}

/* implementation without using Strcpy: */
/*	
void Strcat(char *dest, char *src)
{
	
	if (*dest != '\0')
	{
		Strcat(++dest, src);
	}
	else
	{
		*dest = *src;
		if (*src != '\0')
		{
			Strcat(++dest, ++src);
		}
		else
		{
			return; 
		}
	}
}
*/



char *Strstr(const char *haystack, const char *needle)
{
	if (strlen(haystack) < strlen(needle) || *haystack == '\0')
	{
		return NULL;
	}
	
	if (strcmp(haystack, needle))
	{
		return (char *)haystack;
	}
	
	return Strstr(++haystack, needle);
}



/**** helper singly linked list functions for FlipList exercise *****/
node_t *CreateNode(void *data)
{
	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	if (!new_node)
	{
		return NULL;
	}
	
	new_node->data = data;
	new_node->next = NULL;
	
	return new_node;
}
void AppendNode(node_t **head, void *data)
{
	node_t *current;
	node_t *new_node = CreateNode(data);
	if (*head == NULL)
	{
		*head = new_node;
		return;
	}
	
	current = *head;
	
	while (current->next != NULL)
	{
		current = current->next;
	}
	
	current->next = new_node;
}
void PrintList(node_t *head, void (*PrintData)(void *))
{
	node_t *current = head;
	
	while (current != NULL)
	{
		PrintData(current->data);
		current = current->next;
	}
	
	printf("\n");
}
void FreeList(node_t *head)
{
	node_t *current = head;
	node_t *next_node;
	
	while (current != NULL)
	{
		next_node = current->next;
		free(current);
		current = next_node;
	}
}
void PrintInt(void *data)
{
	printf("%d ", *(int *)data);
}
/*******************************************************/


