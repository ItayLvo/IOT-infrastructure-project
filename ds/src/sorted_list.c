/*
date: 
reviewer: 
status: 
*/

#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */

#include "../include/dllist.h"
#include "../include/sorted_list.h"

struct sorted_list
{
	dll_t *d_list;
	slist_compare_func_t compare_func;
};



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
	assert(start_iterator.sorted_list == end_iterator.sorted_list);
	
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
	
	assert(start_iter.sorted_list == end_iter.sorted_list);
	
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
	slist_iterator_t src_runner_segment_start = SListBegin(src_list);
	slist_iterator_t src_runner_segment_end = SListBegin(src_list);
	
	/* assign dest_runner to be the first node of dest_list that is larger than the first node of src_list */
	slist_iterator_t dest_runner = SListFind(dest_list, SListBegin(dest_list), SListEnd(dest_list), SListGetData(SListBegin(src_list)));
	
	while (!SListIsEqualIter(dest_runner, SListEnd(dest_list)) &&
		!SListIsEqualIter(src_runner_segment_start, SListEnd(src_list)))
	{
		/* find the first element in src_list (between src_runner_segment_start and end of src list) that is larger than dest_runner, and assign it to src_runner_end */
		src_runner_segment_end = SListFind(src_list, src_runner_segment_start, SListEnd(src_list), SListGetData(dest_runner));
		
		/* skip splice of src_runner == dest_runner. nothing to splice at this iteration */
		if (!SListIsEqualIter(src_runner_segment_start, src_runner_segment_end))
		{
			DLListSplice(src_runner_segment_start.iter, src_runner_segment_end.iter, dest_runner.iter);
		}
		src_runner_segment_start = src_runner_segment_end;
		
		dest_runner = SListNext(dest_runner);
	}
	
	/* if dest_runner reaches end of dest_list and src_list still has remaining elements, splice all remaining src elements to the end of dest_list */
	if (SListIsEqualIter(dest_runner, SListEnd(dest_list)) &&
		!SListIsEqualIter(src_runner_segment_start, SListEnd(src_list)))
	{
		DLListSplice(src_runner_segment_start.iter, SListEnd(src_list).iter, dest_runner.iter);
	}
	
	return dest_list;
}


dll_t *SListGetDLL(slist_t *list) /* helper function for print */
{
	return list->d_list;
}


