#include <stddef.h>	/* size_t */
#include <stdlib.h>	/* malloc */
#include <assert.h>	/* assert */

#include "../include/singly_linked_list.h"

struct queue
{
    struct linked_list *list; 
};

queue_t *QueueCreate(void)
{
	queue_t *queue = (queue_t)malloc(sizeof(queue_t));
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
	SLListRemove(SLListGetBegin(queue->list);
}



size_t QueueSize(queue_t *queue)
{
	return SLListCount(queue->list);
}


int QueueIsEmpty(queue_t *queue)
{
	return SLListIsEmpty(queue->list);
}


queue *QueueAppend(queue_t *queue1, queue_t *queue2)
{
	int function_status = 0;
	function_status = SLListSwapNodes(SLListGetEnd(queue1->list), SLListGetBegin(queue2->list));
	if (0 != function_status)
	{
		return NULL;
	}
	
	SLListGetEnd(queue2->list) = SLListGetBegin(queue2->list);
}
