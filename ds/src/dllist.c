/*
date: 
reviewer: 
status: 
*/

#include <string.h>	/* memcpy */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf, size_t */
#include <assert.h>	/* assert */

#include "../include/dllist.h"

#define UNUSED(x) (void)(x)

typedef struct node
{
	void *data;
	struct node *next;
	struct node *prev;
} node_t;


struct dll
{
	node_t *head;
	node_t *tail;
};


static int CountHelper(void *node_data, void *counter);

static void *IteratorGetData(dll_iterator_t iterator)
{
	return iterator->data;
}
static dll_iterator_t IteratorGetNext(dll_iterator_t iterator)
{
	return iterator->next;
}
static dll_iterator_t IteratorGetPrev(dll_iterator_t iterator)
{
	return iterator->prev;
}
static void IteratorSetData(dll_iterator_t node, void *data)
{
	node->data = data;
}
static void IteratorSetNext(dll_iterator_t node, dll_iterator_t next_node)
{
	node->next = next_node;
}
static void IteratorSetPrev(dll_iterator_t node, dll_iterator_t prev_node)
{
	node->prev = prev_node;
}
static dll_iterator_t CreateIterator(dll_iterator_t iterator)
{
	iterator = (dll_iterator_t)malloc(sizeof(node_t));
	return iterator;
}
static void FreeIterator(dll_iterator_t iterator)
{
	free(iterator);
}
static int IsNullIterator(dll_iterator_t iterator)
{
	return (NULL == iterator);
}



dll_t *DLListCreate(void)
{
	dll_t *list = (dll_t *)malloc(sizeof(dll_t));
	node_t *tail_dummy = NULL;
	node_t *head_dummy = NULL;
	if (NULL == list)
	{
		return NULL;
	}
	
	tail_dummy = CreateIterator(tail_dummy);
	if (IsNullIterator(tail_dummy))
	{
		free(list);
		return NULL;
	}
	
	head_dummy = CreateIterator(head_dummy);
	if (IsNullIterator(head_dummy))
	{
		free(tail_dummy);
		free(list);
		return NULL;
	}
	
	
	IteratorSetData(tail_dummy, list);
	IteratorSetNext(tail_dummy, NULL);
	IteratorSetPrev(tail_dummy, head_dummy);
	
	IteratorSetData(head_dummy, list);
	IteratorSetNext(head_dummy, tail_dummy);
	IteratorSetPrev(head_dummy, NULL);
		
	list->head = head_dummy;
	list->tail = tail_dummy;
	
	return list;
}


void DLListDestroy(dll_t *list)
{
	dll_iterator_t runner = NULL;
	dll_iterator_t tmp_next_node = NULL;
	assert(list);
	runner = DLListBegin(list);
	
	while (!DLListIsEqualIter(runner, list->tail))
	{
		tmp_next_node = IteratorGetNext(runner);
		FreeIterator(runner);
		runner = tmp_next_node;
	}
	
	FreeIterator(list->head);
	FreeIterator(list->tail);
	free(list);
}


dll_iterator_t DLListInsert(dll_t *list, dll_iterator_t iterator, void *data)
{
	node_t *new_node = NULL;
	void *tmp_data_holder = NULL;
	
	assert(list);
	assert(!IsNullIterator(iterator));
	
	new_node = CreateIterator(new_node);
	if(IsNullIterator(new_node))
	{
		return list->tail;
	}
	
	IteratorSetNext(new_node, IteratorGetNext(iterator));
	IteratorSetNext(iterator, new_node);
	IteratorSetPrev(new_node, iterator);
	
	tmp_data_holder = IteratorGetData(iterator);
	IteratorSetData(iterator, data);
	IteratorSetData(new_node, tmp_data_holder);
	
	if (DLListIsEqualIter(list->tail, iterator))
	{
		list->tail = new_node;
	}
	else
	{
		IteratorSetPrev(IteratorGetNext(new_node), new_node);
	}
	
	return iterator;
}


dll_iterator_t DLListPushFront(dll_t *list, void *data)
{
	return(DLListInsert(list, DLListBegin(list), data));
}


dll_iterator_t DLListPushBack(dll_t *list, void *data)
{
	return(DLListInsert(list, list->tail, data));
}


dll_iterator_t DLListRemove(dll_iterator_t iterator)
{
	dll_iterator_t node_to_remove = NULL;
	assert(!IsNullIterator(iterator));
	
	node_to_remove = IteratorGetNext(iterator);
		
	IteratorSetData(iterator, IteratorGetData(IteratorGetNext(iterator)));
	IteratorSetNext(iterator, IteratorGetNext(IteratorGetNext(iterator)));
	
	if(IsNullIterator(IteratorGetNext(iterator)))
	{
		((dll_t *)(IteratorGetData(iterator)))->tail = iterator;
	}
	else
	{
		IteratorSetPrev(IteratorGetNext(iterator), iterator);
	}
	
	free(node_to_remove);
	
	return iterator;
}


void *DLListPopFront(dll_t *list)
{
	if (DLListIsEmpty(list))
	{
		return NULL;
	}
	
	return(IteratorGetData(DLListRemove(DLListBegin(list))));
}


void *DLListPopBack(dll_t *list)
{
	if (DLListIsEmpty(list))
	{
		return NULL;
	}
	
	return(IteratorGetData(DLListRemove(IteratorGetPrev(list->tail))));
}


dll_iterator_t DLListSplice(dll_iterator_t start_iterator,
				dll_iterator_t end_iterator,
				dll_iterator_t dest_iterator)
{
	dll_iterator_t iterator_before_start = IteratorGetPrev(start_iterator);
	dll_iterator_t iterator_before_end = IteratorGetPrev(end_iterator);
	dll_iterator_t iterator_before_dest = IteratorGetPrev(dest_iterator);
	
	/*connecting node before dest to start_iter*/
	IteratorSetNext(iterator_before_dest, start_iterator);
	IteratorSetPrev(start_iterator, iterator_before_dest);
	
	/*connecting node before end_iter with dest*/
	IteratorSetNext(iterator_before_end, dest_iterator);
	IteratorSetPrev(dest_iterator, iterator_before_end);
	
	/*connecting node before start_iterator to end_iterator*/
	/*connecting "dangling" end to iterator before start_iterator*/
	IteratorSetNext(iterator_before_start, end_iterator);
	IteratorSetPrev(end_iterator, iterator_before_start);
	
	return dest_iterator;
}


int DLListIsEqualIter(dll_iterator_t it1, dll_iterator_t it2)
{
	assert(it1);
	assert(it2);
	
	return (it1 == it2);
}

dll_iterator_t DLListBegin(const dll_t* list)
{
	assert(list);
	
	return IteratorGetNext(list->head);
}


dll_iterator_t DLListEnd(const dll_t* list)
{
	assert(list);
	
	return list->tail;
}


dll_iterator_t DLListForeach(dll_iterator_t iter_start, dll_iterator_t iter_end, void* data, dll_action_func_t func)
{
	dll_iterator_t runner = NULL;
	int function_exit_status = 0;
	
	assert(iter_start);
	assert(iter_end);
	
	runner = iter_start;
	
	while (!DLListIsEqualIter(runner, iter_end))
	{
		function_exit_status = func(DLListGetData(runner), data);
		if (0 != function_exit_status)
		{
			return runner;
		}
		
		runner = DLListNext(runner);
	}
	
	return iter_end;
}


size_t DLListCount(const dll_t *list)
{
	size_t count_nodes = 0;
	
	DLListForeach(DLListBegin(list), list->tail, &count_nodes, CountHelper);
	
	return (count_nodes);
}

static int CountHelper(void *node_data, void *counter)
{
	UNUSED(node_data);
	++*(size_t *)counter;

	return 0;
}


void *DLListGetData(dll_iterator_t iterator)
{
	return IteratorGetData(iterator);
}


void DLListSetData(dll_iterator_t iterator, void* data)
{
	IteratorSetData(iterator, data);
}


dll_iterator_t DLListNext(dll_iterator_t iter)
{
	return IteratorGetNext(iter);
}

dll_iterator_t DLListPrev(dll_iterator_t iter)
{
	return IteratorGetPrev(iter);
}


dll_iterator_t DLListFind(dll_iterator_t start, dll_iterator_t end, void *data, dll_match_func_t func)
{
	dll_iterator_t runner = start;
	
	assert(!IsNullIterator(start));
	assert(!IsNullIterator(end));
	
	while (!DLListIsEqualIter(runner, end))
	{
		if (func(DLListGetData(runner), data))
		{
			return runner;
		}
		runner = DLListNext(runner);
	}
	
	return end;
}


int DLListMultiFind(dll_t *dest_list ,dll_iterator_t start, dll_iterator_t end, void* data, dll_match_func_t func)
{
	size_t count_found = 0;
	dll_iterator_t runner = start;
	assert(!IsNullIterator(start));
	assert(!IsNullIterator(end));
	
	while (!DLListIsEqualIter(runner, end))
	{
		if (func(DLListGetData(runner), data))
		{
			++count_found;
			if (DLListPushBack(dest_list, data) == DLListEnd(dest_list)) /* if pushback failed */
			{
				return -1;
			}
		}
		runner = DLListNext(runner);
	}
	
	return count_found;
}


int DLListIsEmpty(const dll_t* list)
{
	return (DLListBegin(list) == list->tail);
}




