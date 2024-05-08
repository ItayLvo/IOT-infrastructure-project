#include <stddef.h>	/* size_t */
#include <string.h>	/* memcpy */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "../include/singly_linked_list.h"

typedef struct node
{
	void *data;
	struct node* next;
	int is_dummy;
} node_t;


struct linked_list
{
	node_t *head;
	node_t *tail;
};


linked_list_t *SLListCreate(void)
{
	linked_list_t *list = (linked_list_t *)malloc(sizeof(linked_list_t));
	if (NULL == list)
	{
		return NULL;
	}
	
	node_t *dummy_node = malloc(sizeof(node_t));
	if (NULL == dummy_node_ptr)
	{
		free(list);
		return NULL;
	}
	dummy_node->data = NULL;
	dummy_node->next = NULL;
	
	list->head = dummy_node;
	list->tail = dummy_node;
	
	return list;
}


iterator_t SLListInsert(linked_list_t *list, void *data, iterator_t iterator)
{
	node_t *new_node = malloc(sizeof(node_t));
	node_t *tmp_node = malloc(sizeof(node_t));
	
	if (list->tail == iterator)
	
	
	new_node->data = data;
	
	tmp_node->data = new_node->data;
	new_node->data = iterator->data;
	
	new_node->next = iterator->next;
	iterator->data = tmp_node->data;
	
	iterator->next = new_node;
	
	if (list->tail == iterator)
	{
		list->tail = new_node;
	}
	
	free(tmp_node);
	return iterator;
}


void SLListDestroy(linked_list_t *list)
{
	node_t *runner = list->head;
	node_t *tmp_next_node = NULL;
	
	while (runner != list->tail)
	{
		tmp_next_node = runner->next;
		free(runner);
		runner = tmp_next_node;
	}
	
	free(list);
}


/* what does it check for equal??? */
int SLListIsEqual(iterator_t it1, iterator_t it2)
{
	return ((size_t)it1 == (size_t)it2);
}

iterator_t SLListGetBegin(linked_list_t* list)
{
	return list->head;
}

iterator_t SLListGetEnd(linked_list_t* list)
{
	return list->tail;
}



void SLListForEach(iterator_t start, iterator_t end, action_func_t func)
{
	node_t *runner = start;
	
	while (node != end)
	{
		func(runner->data);
		node = node->next;
	}
	func(runner->data);
}




