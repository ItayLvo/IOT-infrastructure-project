#include <stddef.h>	/* size_t */
#include <string.h>	/* memcpy */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "../include/singly_linked_list.h"

/* we can add 1 func to singly linked list if needed */

typedef struct node
{
	void *data;
	struct node* next;
} node_t;


struct linked_list
{
	node_t *head;
	node_t *tail;
};

static int CountHelper(void *data);
static int PrintListHelper(void *data);


static size_t count_nodes = 0;


int SLListAppendLists(linked_list_t *list1, linked_list_t *list2)
{
	iterator_t tmp_iterator = (iterator_t )malloc(sizeof(node_t));
	if (NULL == tmp_iterator)
	{
		return 1;
	}
	tmp_iterator->data = list1->tail->data;
	tmp_iterator->next = list1->tail->next;
	
	list1->tail->data = list2->head->data;
	list1->tail->next = list2->head->next;
	
	list2->head->data = tmp_iterator->data;
	list2->head->next = tmp_iterator->next;
	
	list1->tail = list2->tail;
	list2->tail = list2->head;
	
	free(tmp_iterator);
	return 0;
}


linked_list_t *SLListCreate(void)
{
	linked_list_t *list = (linked_list_t *)malloc(sizeof(linked_list_t));
	node_t *dummy_node = NULL;
	
	if (NULL == list)
	{
		return NULL;
	}
	
	dummy_node = malloc(sizeof(node_t));
	if (NULL == dummy_node)
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
	node_t *new_node;
	node_t *tmp_node;
	
	new_node = malloc(sizeof(node_t));
	if(NULL == new_node)
	{
		return list->tail;
	}
	
	tmp_node = malloc(sizeof(node_t));
	if(NULL == tmp_node)
	{
		return list->tail;
	}
	
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
	
	free(list->tail);
	free(list);
}


int SLListIsEqual(iterator_t node1, iterator_t node2)
{
	return (node1->data == node2->data);
}

iterator_t SLListGetBegin(linked_list_t* list)
{
	return list->head;
}


iterator_t SLListGetEnd(linked_list_t* list)
{
	return list->tail;
}



void SLListForEach(iterator_t start, iterator_t end, action_func_t IteratorHandler)
{
	node_t *runner = start;
	
	while (runner != end)
	{
		IteratorHandler(runner->data);
		runner = runner->next;
	}
	IteratorHandler(runner->data);
}

static int CountHelper(void *data)
{
	++count_nodes;
	
	return 0;
}

void *SLListGetData(const iterator_t iterator)
{
	return iterator->data;
}


void SLListSetData(iterator_t iterator, void* data)
{
	iterator->data = data;
}


size_t SLListCount(const linked_list_t *list)
{
	count_nodes = 0;
	
	SLListForEach(list->head, list->tail, CountHelper);
	
	return (count_nodes - 1);
}



iterator_t SLListNext(iterator_t iter)
{
	return iter->next;
}


iterator_t SLListFind(iterator_t start, iterator_t end,
			const void* data, const comp_func_t IteratorComapreFunc)
{
	iterator_t runner = start;
	
	while (runner != end)
	{
		if (IteratorComapreFunc(runner->data, data))
		{
			return runner;
		}
		runner = runner->next;
	}
	
	return NULL;
}



void SLListRemove(linked_list_t *list, iterator_t iterator)
{
	iterator_t node_to_remove = iterator->next;
	
	if (node_to_remove == list->tail)
	{
		list->tail = iterator;
		iterator->data = NULL;
	}
	else
	{
		iterator->data = (iterator->next)->data;
	}
	iterator->next = (iterator->next)->next;
	
	
	free(node_to_remove);
}


int SLListIsEmpty(const linked_list_t *list)
{
	if (list->head == list->tail)
	{
		return 1;
	}
	
	return 0;
	
}



void SLLPrintList(linked_list_t *list)
{
	printf("Printing list: \n");
	SLListForEach(list->head, list->tail, PrintListHelper);
}

static int PrintListHelper(void *data)
{
	if (data != NULL)
	{
		printf("[%d] -> ", *(int *)data);
		
	}

	return 0;
}

