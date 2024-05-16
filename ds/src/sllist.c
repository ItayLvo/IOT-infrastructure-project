/*
date: 16/05
reviewer: vova
status: done after fixing CR comments
*/

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

iterator_t CreateIterator(iterator_t iterator);
void FreeIterator(iterator_t iterator);
int IsNullIterator(iterator_t iterator);

static int CountHelper(void *node_data, void *counter);

static void *IteratorGetData(iterator_t iterator)
{
	return iterator->data;
}
static iterator_t IteratorGetNext(iterator_t iterator)
{
	return iterator->next;
}

static void IteratorSetData(iterator_t node, void *data)
{
	node->data = data;
}

static void IteratorSetNext(iterator_t node, iterator_t next_node)
{
	node->next = next_node;
}


linked_list_t *SLListCreate(void)
{
	linked_list_t *list = (linked_list_t *)malloc(sizeof(linked_list_t));
	node_t *dummy_node = NULL;
	
	if (NULL == list)
	{
		return NULL;
	}
	
	dummy_node = CreateIterator(dummy_node);
	if (IsNullIterator(dummy_node))
	{
		free(list);
		return NULL;
	}
	IteratorSetData(dummy_node, list);
	IteratorSetNext(dummy_node, NULL);
	
	list->head = dummy_node;
	list->tail = dummy_node;
	
	return list;
}


iterator_t SLListInsert(linked_list_t *list, void *data, iterator_t iterator)
{
	node_t *new_node = NULL;
	node_t *tmp_node = NULL;
	
	assert(list);
	assert(!IsNullIterator(iterator));
	
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
	
	IteratorSetData(new_node, data);
	
	IteratorSetData(tmp_node, IteratorGetData(new_node));
	IteratorSetData(new_node, IteratorGetData(iterator));
	
	IteratorSetNext(new_node, IteratorGetNext(iterator));
	IteratorSetData(iterator, IteratorGetData(tmp_node));
	
	IteratorSetNext(iterator, new_node);
	
	if (SLListIsEqual(list->tail, iterator))
	{
		list->tail = new_node;
	}
	
	FreeIterator(tmp_node);
	return iterator;
}


void SLListDestroy(linked_list_t *list)
{
	iterator_t runner = NULL;
	iterator_t tmp_next_node = NULL;
	assert(list);
	runner = SLListGetBegin(list);
	
	while (!SLListIsEqual(runner, list->tail))
	{
		tmp_next_node = IteratorGetNext(runner);
		FreeIterator(runner);
		runner = tmp_next_node;
	}
	
	FreeIterator(list->tail);
	free(list);
}


int SLListIsEqual(iterator_t node1, iterator_t node2)
{
	assert(node1);
	assert(node2);
	
	return (node1 == node2);
}

iterator_t SLListGetBegin(linked_list_t* list)
{
	assert(list);
	
	return list->head;
}


iterator_t SLListGetEnd(linked_list_t* list)
{
	assert(list);
	
	return list->tail;
}



int SLListForEach(iterator_t start, iterator_t end, action_func_t func, void *data)
{
	iterator_t runner = NULL;
	int function_exit_status = 0;
	
	assert(start);
	assert(end);
	
	runner = start;
	
	while (!SLListIsEqual(runner, end))
	{
		function_exit_status += func(SLListGetData(runner), data);
		runner = SLListNext(runner);
	}
	
	return function_exit_status;
}


size_t SLListCount(const linked_list_t *list)
{
	size_t count_nodes = 0;
	
	SLListForEach(list->head, list->tail, CountHelper, &count_nodes);
	
	return (count_nodes);
}

static int CountHelper(void *node_data, void *counter)
{
	++*(size_t *)counter;

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


iterator_t SLListNext(iterator_t iter)
{
	return iter->next;
}


iterator_t SLListFind(iterator_t start, iterator_t end,
			void* data, const comp_func_t iterator_comapre_func)
{
	iterator_t runner = start;
	assert(!IsNullIterator(start));
	assert(!IsNullIterator(end));
	
	while (!SLListIsEqual(runner, end))
	{
		if (iterator_comapre_func(SLListGetData(runner), data))
		{
			return runner;
		}
		runner = SLListNext(runner);
	}
	
	return end;
}



void SLListRemove(iterator_t iterator)
{
	iterator_t node_to_remove = NULL;
	assert(!IsNullIterator(iterator));
	
	node_to_remove = iterator->next;
		
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
	return (list->head == list->tail);
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



