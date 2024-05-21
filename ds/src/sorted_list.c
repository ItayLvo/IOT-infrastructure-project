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
	slist_compare_func_t compare_func;
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
	s_list->compare_func = cmp_func;
	
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
	slist_iterator_t new_node = SListFind(list, SListBegin(list), SListEnd(list), data);
	new_node.iter = DLListInsert(list->d_list, new_node.iter, data);
	
	#ifndef NDEBUG
	new_node.sorted_list = list;
	#endif /* NDEBUG */

	return new_node;
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
	slist_iterator_t iterator;
	assert(list);
	
	iterator.iter = DLListBegin(list->d_list);
	
	#ifndef NDEBUG
	iterator.sorted_list = (slist_t*)list;;
	#endif /* NDEBUG */
	
	return iterator;
}


slist_iterator_t SListEnd(const slist_t* list)
{
	slist_iterator_t iterator;
	assert(list);
	
	iterator.iter = DLListEnd(list->d_list);
	
	#ifndef NDEBUG
	iterator.sorted_list = (slist_t*)list;
	#endif /* NDEBUG */
	
	return iterator;
}


slist_iterator_t SDLListForeach(slist_iterator_t start_iterator, slist_iterator_t end_iterator, void* data, slist_action_func_t func)
{	
	/*
	can't assert structs
	
	assert(start_iterator);
	assert(end_iterator);
	*/
	
	start_iterator.iter = DLListForeach(start_iterator.iter, end_iterator.iter, data, func);
	
	return start_iterator;
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


/* uses compare_func to find the first element in the range that is larger than data */
slist_iterator_t SListFind(slist_t *list, slist_iterator_t start_iter,
                           slist_iterator_t end_iter, void* data)
{
	slist_compare_func_t compare_func = list->compare_func;
	
	while (!SListIsEqualIter(start_iter, end_iter))
	{
		if (compare_func(SListGetData(start_iter), data) > 0)
		{
			return start_iter;
		}
		
		start_iter = SListNext(start_iter);
	}
	

	return start_iter;
}


slist_iterator_t SListFindIf(slist_iterator_t start_iter, slist_iterator_t end_iter, void* data, slist_match_func_t match_func)
{
	start_iter.iter = DLListFind(start_iter.iter, end_iter.iter, data, match_func);
	
	return start_iter;
}


int SListIsEmpty(const slist_t *list)
{
	return DLListIsEmpty(list->d_list);
}


slist_t* SDLListMerge(slist_t *dest_list, slist_t *src_list)
{
	slist_t dest_runner = SListBegin(dest_list);
	slist_t src_runner_segment_start = SListBegin(src_list);
	slist_t src_runner_segment_end = SListBegin(src_list);
	
	while (!SListEqual(dest_runner, SListEnd(dest_list)) &&
		!!SListEqual(src_runner_segment_start, SListEnd(src_list)))
	{
		/* find the first element in src_list (between src_runner_segment_start and end of src list) that is larger than dest_runner, and assign it to src_runner_end */
		src_runner_segment_end = SListFind(src_list, src_runner_segment_start, SListEnd(src_list), SListGetData(dest_runner));
		slist_t tmp_next_iterator = SListNext(src_runner_segment_end);
		
		dest_runner = SListNext(dest_runner);
	}
}

























