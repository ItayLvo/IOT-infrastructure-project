#include <string.h>	/* memcpy */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf, size_t */
#include <assert.h>	/* assert */

#include "../include/sllist.h"

typedef struct node
{
	void *data;
	struct node *next;
} node_t;


struct linked_list
{
	node_t *head;
	node_t *tail;
};

static int CountHelper(void *data, void *dummy);
static int PrintListHelper(void *data, void *dummy);
void SLLPrintList(linked_list_t list, void *dummy);

iterator_t CreateIterator(iterator_t iterator);
void FreeIterator(iterator_t iterator);
int IsNullIterator(iterator_t iterator);
void *IteratorGetData(iterator_t iterator);
iterator_t IteratorGetNext(iterator_t iterator);

/* static because i want to limit the visibility of this var to this file */
static size_t count_nodes = 0;


linked_list_t *SLListCreate(void)
{
	linked_list_t *list = (linked_list_t *)malloc(sizeof(linked_list_t));
	node_t *dummy_node = NULL;
	
	if (NULL == list)
	{
		return NULL;
	}
	
	dummy_node = CreateIterator(dummy_node);
	if (NULL == IsNullIterator(dummy_node))
	{
		free(list);
		return NULL;
	}
	dummy_node->data = list;
	dummy_node->next = NULL;
	
	list->head = dummy_node;
	list->tail = dummy_node;
	
	return list;
}


iterator_t SLListInsert(linked_list_t *list, void *data, iterator_t iterator)
{
	node_t *new_node = NULL;
	node_t *tmp_node = NULL;
	
	new_node = CreateIterator(new_node);
	if(IsNullIterator(new_node))
	{
		return list->tail;
	}
	
	tmp_node = CreateIterator(tmp_node);
	if(IsNullIterator(tmp_node))
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
	
	FreeIterator(tmp_node);
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



int SLListForEach(iterator_t start, iterator_t end, action_func_t IteratorHandler, void *data)
{
	node_t *runner = start;
	
	while (runner != end)
	{
		IteratorHandler(runner->data, NULL);
		runner = runner->next;
	}
	IteratorHandler(runner->data, NULL);
}

static int CountHelper(void *data, void *dummy)
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
	
	SLListForEach(list->head, list->tail, CountHelper, NULL);
	
	return (count_nodes - 1);
}



iterator_t SLListNext(iterator_t iter)
{
	return iter->next;
}


iterator_t SLListFind(iterator_t start, iterator_t end,
			void* data, const comp_func_t IteratorComapreFunc)
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



void SLListRemove(iterator_t iterator)
{
	iterator_t node_to_remove = iterator->next;
		
	iterator->data = iterator->next->data;
	iterator->next = iterator->next->next;
	
	if (NULL == iterator->next)
	{
		((linked_list_t *)(iterator->data))->tail = iterator;
	}
	
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


int SLListAppendLists(linked_list_t *list1, linked_list_t *list2)
{
	iterator_t tmp_iterator = CreateIterator(tmp_iterator);
	if (IsNullIterator(tmp_iterator))
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
	
	FreeIterator(tmp_iterator);
	return 0;
}



void SLLPrintList(linked_list_t *list, void *dummy)
{
	printf("Printing list: \n");
	SLListForEach(list->head, list->tail, PrintListHelper, NULL);
}

static int PrintListHelper(void *data, void *dummy)
{
	if (data != NULL)
	{
		printf("[%d] -> ", *(int *)data);
		
	}

	return 0;
}

int IsNullIterator(iterator_t iterator)
{
	return (NULL == iterator);
}


iterator_t CreateIterator(iterator_t iterator)
{
	iterator = (iterator_t)malloc(sizeof(node_t));
	return iterator;
}


void FreeIterator(iterator_t iterator)
{
	free(iterator);
}
