/*
date: 
reviewer: 
status:
*/

#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */





#include <stdio.h>	/* printf */






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


void PQDestroy(priorityq_t *queue)
{
	assert(queue);
	
	SListDestroy(queue->sorted_list);
	free(queue);
}


int PQEnqueue(priorityq_t *queue, void *data)
{
	slist_iterator_t insert_fail_check = SListInsert(queue->sorted_list, data);
	
	if (SListIsEqualIter(insert_fail_check, SListEnd(queue->sorted_list)))
	{
		return 1;
	}
	
	return 0;
}


void *PQDequeue(priorityq_t *queue)
{
	assert(queue);
	
	return (SListPopBack(queue->sorted_list));
}


void *PQPeek(const priorityq_t *queue)
{
	return SListGetData(SListPrev(SListEnd(queue->sorted_list)));
}


int PQIsEmpty(const priorityq_t *queue)
{
	assert(queue);
	
	return SListIsEmpty(queue->sorted_list);
}


size_t PQSize(const priorityq_t *queue)
{
	return SListCount(queue->sorted_list);
}


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

