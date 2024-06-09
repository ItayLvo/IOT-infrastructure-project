#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <assert.h>	/* assert */

#include "sllist.h"
#include "queue.h"

struct queue
{
    struct linked_list *list; 
};

queue_t *QueueCreate(void)
{
	queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
	linked_list_t *list = SLListCreate();
	if (NULL == queue || NULL == list)
	{
		return NULL;
	}
	
	queue->list = list;
	
	return queue;
}


void QueueDestroy(queue_t *queue)
{
	SLListDestroy(queue->list);
	free(queue);
}


int QueueEnqueue(queue_t *queue, void *data)
{
	if (SLListInsert(queue->list, data, SLListGetEnd(queue->list)) ==
			SLListGetEnd(queue->list))
	{
		return 0;
	}
	
	return 1;
}


void *QueuePeek(const queue_t *queue)
{
	return (SLListGetData(SLListGetBegin(queue->list)));
}


void QueueDequeue(queue_t *queue)
{
	/* Changed on 9th june when working on makefile to match sllist.h remove prototype */
	/* the previous remove command is in comment. new remove not tested */
	
	/* SLListRemove(queue->list, SLListGetBegin(queue->list)); */
	SLListRemove(SLListGetBegin(queue->list));
}



size_t QueueSize(queue_t *queue)
{
	return SLListCount(queue->list);
}


int QueueIsEmpty(queue_t *queue)
{
	return SLListIsEmpty(queue->list);
}


queue_t *QueueAppend(queue_t *queue1, queue_t *queue2)
{
	int function_status = 0;
	function_status = SLListAppendLists(queue1->list, queue2->list);
	if (0 != function_status)
	{
		return NULL;
	}
	
	return queue1;
}
