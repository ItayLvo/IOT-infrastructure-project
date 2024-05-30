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
};


pq_t *PQCreate(pq_compare_func_t cmp_func)
{
	pq_t *priority_queue = (pq_t *)malloc(sizeof(pq_t));
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
	
	return priority_queue;
}


void PQDestroy(pq_t *queue)
{
	assert(queue);
	
	SListDestroy(queue->sorted_list);
	free(queue);
}


int PQEnqueue(pq_t *queue, void *data)
{
	slist_iterator_t insert_fail_check = SListInsert(queue->sorted_list, data);
	
	if (SListIsEqualIter(insert_fail_check, SListEnd(queue->sorted_list)))
	{
		return 1;
	}
	
	return 0;
}


void *PQDequeue(pq_t *queue)
{
	assert(queue);
	
	return (SListPopBack(queue->sorted_list));
}


void *PQPeek(const pq_t *queue)
{
	return SListGetData(SListPrev(SListEnd(queue->sorted_list)));
}


int PQIsEmpty(const pq_t *queue)
{
	assert(queue);
	
	return SListIsEmpty(queue->sorted_list);
}


size_t PQSize(const pq_t *queue)
{
	return SListCount(queue->sorted_list);
}


void PQClear(pq_t *queue)
{
	assert(queue);
	
	while (!PQIsEmpty(queue))
	{
		PQDequeue(queue);
	}
}


void *PQErase(pq_t *queue, pq_match_func_t is_match, void *param)
{
	void *data = NULL;
	
	slist_iterator_t element_found = SListFindIf(SListBegin(queue->sorted_list), SListEnd(queue->sorted_list), param, is_match);
	
	if (SListIsEqualIter(element_found, SListEnd(queue->sorted_list)))
	{
		return NULL;
	}

	data = SListGetData(element_found);
	SListRemove(element_found);
	
	return data;
}

