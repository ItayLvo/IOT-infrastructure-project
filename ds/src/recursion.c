/*
date: 
reviewer:
status: 
*/

#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */

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




