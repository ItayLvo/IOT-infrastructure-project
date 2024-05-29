/*
date: 
reviewer: 
status:
*/

#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */

#include "priority_queue.h"
#include "sorted_list.h"


struct priorityq
{
	slist_t *sorted_list;
	pqcompare_func_t cmp_func;
};


priorityq_t *PQCreate(pqcompare_func_t cmp_func)
{
	priorityq_t *priority_queue = (priorityq_t *)malloc(sizeof(priorityq_t));
	if (NULL == priority_queue)
	{
		return NULL;
	}
	
	priority_queue->sorted_list = SListCreate(cmp_func);
	if (NULL == priority_queue->sorted_list)
	{
		free(priority_queue);
		return NULL;
	}
	
	priority_queue->cmp_func = cmp_func;
	
	return priority_queue;
}

/*
slist_t *SListCreate(slist_compare_func_t cmp_func)
{
	slist_t *s_list = (slist_t *)malloc(sizeof(slist_t));
	if (NULL == s_list)
	{
		return NULL;
	}
	
	s_list->d_list = DLListCreate();
	if (NULL == s_list->d_list)
	{
		free(s_list);
		return NULL;
	}
	
	s_list->compare_func = cmp_func;
	
	return s_list;
}
*/

void PQDestroy(priorityq_t *queue)
{
	assert(queue);
	
	SListDestroy(queue->sorted_list);
	free(queue);
}

/*
void SListDestroy(slist_t *list)
{
	assert(list);
	
	DLListDestroy(list->d_list);
	free(list);
}
*/


int PQEnqueue(priorityq_t *queue, void *data)
{
	/* if return value of SListInsert is NOT equal to SListEnd, insert was successful */
	return (!SListIsEqualIter(SListInsert(queue->sorted_list, data), SListEnd(queue->sorted_list)));
}

/*
slist_iterator_t SListInsert(slist_t *list,  void *data)
{
	slist_iterator_t new_node = SListFind(list, SListBegin(list), SListEnd(list), data);
	new_node.iter = DLListInsert(list->d_list, new_node.iter, data);
	
	#ifndef NDEBUG
	new_node.sorted_list = list;
	#endif

	return new_node;
}
*/


void *PQDequeue(priorityq_t *queue)
{
	return (SListPopBack(queue->sorted_list));
}

/*
slist_iterator_t SListRemove(slist_iterator_t iter)
{
	assert(NULL != iter.iter);
	iter.iter = DLListRemove(iter.iter);
	
	return iter;
}


void *SListPopBack(slist_t *list)
{
	assert(NULL != list);
	return (DLListPopBack(list->d_list));
}
*/

void *PQPeek(const priorityq_t *queue)
{
	return SListGetData(SListPrev(SListEnd(queue->sorted_list)));
}


int PQIsEmpty(const priorityq_t *queue)
{
	assert(queue);
	
	return SListIsEmpty(queue->sorted_list);
}

/*
int SListIsEmpty(const slist_t *list)
{
	assert(NULL != list);
	return DLListIsEmpty(list->d_list);
}
*/


size_t PQSize(const priorityq_t *queue)
{
	return SListCount(queue->sorted_list);
}

/*
size_t SListCount(const slist_t *list)
{
	return (DLListCount(list->d_list));
}
*/


void PQClear(priorityq_t *queue)
{
	assert(queue);
	
	while (!PQIsEmpty(queue))
	{
		PQDequeue(queue);
	}
}



void *PQErase(priorityq_t *queue, pqmatch_func_t is_match, void *param)
{
	assert(queue);
	
	return SListGetData(SListRemove(SListFindIf(SListBegin(queue->sorted_list), SListEnd(queue->sorted_list), param, is_match)));
}

/*
slist_iterator_t SListFindIf(slist_iterator_t start_iter, slist_iterator_t end_iter, void* data, slist_match_func_t match_func)
{
	start_iter.iter = DLListFind(start_iter.iter, end_iter.iter, data, match_func);
	
	return start_iter;
}
*/







/*
slist_iterator_t SListBegin(const slist_t* list)
{
	slist_iterator_t iterator;
	assert(list);
	
	iterator.iter = DLListBegin(list->d_list);
	
	#ifndef NDEBUG
	iterator.sorted_list = (slist_t*)list;;
	#endif
	
	return iterator;
}

slist_iterator_t SListEnd(const slist_t* list)
{
	slist_iterator_t iterator;
	assert(list);
	
	iterator.iter = DLListEnd(list->d_list);
	
	#ifndef NDEBUG
	iterator.sorted_list = (slist_t*)list;
	#endif
	
	return iterator;
}

void *SListGetData(slist_iterator_t iter)
{
	assert(NULL != iter.iter);
	return DLListGetData(iter.iter);
}


slist_iterator_t SListNext(slist_iterator_t iter)
{
	assert(NULL != iter.iter);
	iter.iter = DLListNext(iter.iter);
	return iter;
}

slist_iterator_t SListPrev(slist_iterator_t iter)
{
	assert(NULL != iter.iter);
	iter.iter = DLListPrev(iter.iter);
	return iter;
}
*/
