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
#include "../include/sorted_list.h"

struct sorted_list
{
	dll_t *d_list;
	slist_compare_func_t func;
};


/*
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
*/


slist_t *SListCreate(slist_compare_func_t cmp_func)
{
	slist_t *s_list = (slist_t *)malloc(sizeof(slist_t));
	if (NULL == s_list)
	{
		return NULL;
	}
	
	s_list->d_list = DLListCreate();
	s_list->func = cmp_func;
	
	return s_list;
}


void SListDestroy(slist_t *list)
{
	assert(list);
	
	DLListDestroy(list->d_list);
	free(list);
}


slist_iterator_t SListInsert(slist_t *list,  void *data)
{
	/***/
	slist_iterator_t tmp;
	
	/***/
	return tmp;
}


slist_iterator_t SListRemove(slist_iterator_t iter)
{
	iter.iter = DLListRemove(iter.iter);
	
	return iter;
}


void *SListPopFront(slist_t *list)
{	
	return (DLListPopFront(list->d_list));
}


void *SListPopBack(slist_t *list)
{
	return (DLListPopBack(list->d_list));
}


int SListIsEqualIter(slist_iterator_t it1, slist_iterator_t it2)
{
	/*
	can't use assert on structs
	assert(it1);
	assert(it2);
	*/
	
	return (DLListIsEqualIter(it1.iter, it2.iter));
}

slist_iterator_t SListBegin(const slist_t* list)
{
	slist_iterator_t iter;
	assert(list);
	
	/*slist_iterator_t iter = DLListBegin(list->d_list);*/
	return iter;
}


slist_iterator_t SListEnd(const slist_t* list)
{
	slist_iterator_t iter;
	assert(list);
	
	/*slist_iterator_t iter = DLListEnd(list->d_list);*/
	return iter;
}


slist_iterator_t SDLListForeach(slist_iterator_t start_iterator, slist_iterator_t end_iterator, void* data, slist_action_func_t func)
{
	/*
	slist_iterator_t runner = NULL;
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
	*/
	return end_iterator;
}


size_t SListCount(const slist_t *list)
{
	return (DLListCount(list->d_list));
}


void *SListGetData(slist_iterator_t iter)
{
	return DLListGetData(iter.iter);
}


slist_iterator_t SListNext(slist_iterator_t iter)
{
	iter.iter = DLListNext(iter.iter);
	return iter;
}

slist_iterator_t SListPrev(slist_iterator_t iter)
{
	iter.iter = DLListPrev(iter.iter);
	return iter;
}


slist_iterator_t SListFind(slist_iterator_t *list, slist_iterator_t start_iter,
                           slist_iterator_t end_iter, void* data)
{
	/*...*/
	
	slist_iterator_t iter;
	
	
	/*...*/
	return iter;
}


slist_iterator_t SListFindIf(slist_iterator_t start_iter, slist_iterator_t end_iter, void* data, slist_match_func_t func)
{
	/*...*/
	
	
	slist_iterator_t iter;
	
	
	/*...*/
	return iter;
}


int SListIsEmpty(const slist_t *list)
{
	return DLListIsEmpty(list->d_list);
}




