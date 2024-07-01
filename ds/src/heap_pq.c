#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */

#include "priority_queue.h"
#include "heap.h" 	/* heap functions (Create, Add, Destroy, Pop, IsEqual, GetData, Remove, GetEnd, GetBegin) */


struct priorityq
{
	heap_t *heap;
};


pq_t *PQCreate(pq_compare_func_t cmp_func)
{
	pq_t *priority_queue = (pq_t *)malloc(sizeof(pq_t));
	if (NULL == priority_queue)
	{
		return NULL;
	}
	
	priority_queue->heap = HeapCreate(cmp_func);
	if (NULL == priority_queue->heap)
	{
		free(priority_queue);
		return NULL;
	}
	
	return priority_queue;
}


void PQDestroy(pq_t *queue)
{
	assert(queue);
	
	HeapDestroy(queue->heap);
	free(queue);
}


int PQEnqueue(pq_t *queue, void *data)
{
	int push_status = HeapPush(queue->heap, data);
	
	return push_status;
}


void *PQDequeue(pq_t *queue)
{
	void *data = NULL;
	
	assert(queue);
	
	data = HeapPeek(queue->heap);
	HeapPop(queue->heap);
	
	return data;
}


void *PQPeek(const pq_t *queue)
{
	void *data = NULL;
	
	assert(queue);
	
	data = HeapPeek(queue->heap);
	
	return data;
}


int PQIsEmpty(const pq_t *queue)
{
	assert(queue);
	
	return HeapIsEmpty(queue->heap);
}


size_t PQSize(const pq_t *queue)
{
	assert(queue);
	
	return HeapSize(queue->heap);
}



void *PQErase(pq_t *queue, pq_match_func_t is_match, void *param)
{
	assert(queue);
	
	return HeapRemove(queue->heap, is_match, param);
}


void PQClear(pq_t *queue)
{
	assert(queue);
	
	while (!PQIsEmpty(queue))
	{
		PQDequeue(queue);
	}
}

