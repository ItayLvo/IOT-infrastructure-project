#include <stddef.h>	/* size_t */
#include <string.h>	/* memcpy */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "../include/singly_linked_list.h"

/* we can add 1 func to singly linked list if needed */

typedef struct node
{
	void *data;
	struct node* next;
} node_t;


struct linked_list
{
	node_t *head;
	node_t *tail;
};

struct queue
{
    struct linked_list *list; 
};


static int CountHelper(void *data);
static int PrintListHelper(void *data);


static size_t count_nodes = 0;


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

/*
linked_list_t *SLListCreate(void)
{
	linked_list_t *list = (linked_list_t *)malloc(sizeof(linked_list_t));
	node_t *dummy_node = NULL;
	
	if (NULL == list)
	{
		return NULL;
	}
	
	dummy_node = malloc(sizeof(node_t));
	if (NULL == dummy_node)
	{
		free(list);
		return NULL;
	}
	dummy_node->data = NULL;
	dummy_node->next = NULL;
	
	list->head = dummy_node;
	list->tail = dummy_node;
	
	return list;
}
*/

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

/*
iterator_t SLListInsert(linked_list_t *list, void *data, iterator_t iterator)
{
	node_t *new_node;
	node_t *tmp_node;
	
	new_node = malloc(sizeof(node_t));
	if(NULL == new_node)
	{
		return list->tail;
	}
	
	tmp_node = malloc(sizeof(node_t));
	if(NULL == tmp_node)
	{
		return list->tail;
	}
	
	new_node->data = data;
	
	tmp_node->data = new_node->data;
	new_node->data = iterator->data;
	
	new_node->next = iterator->next;
	iterator->data = tmp_node->data;
	
	iterator->next = new_node;
	
	if (list->tail == iterator)
	{
		list->tail = new_node;
	}
	
	free(tmp_node);
	return iterator;
}
*/

void QueueDequeue(queue_t *queue)
{
	SLListRemove(SLListGetBegin(queue->list);
}

/*
void SLListRemove(linked_list_t *list, iterator_t iterator)
{
	iterator_t node_to_remove = iterator->next;
	
	if (node_to_remove == list->tail)
	{
		list->tail = iterator;
		iterator->data = NULL;
	}
	else
	{
		iterator->data = (iterator->next)->data;
	}
	iterator->next = (iterator->next)->next;
	
	
	free(node_to_remove);
}
*/

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
	size_t index = 0;
	
	while (i < QueueSize(queue1))
	{
		
	}
}




void SLListForEach(iterator_t start, iterator_t end, action_func_t IteratorHandler)
{
	node_t *runner = start;
	
	while (runner != end)
	{
		IteratorHandler(runner->data);
		runner = runner->next;
	}
	IteratorHandler(runner->data);
}

static int CountHelper(void *data)
{
	++count_nodes;
	
	return 0;
}



iterator_t SLListFind(iterator_t start, iterator_t end,
			const void* data, const comp_func_t IteratorComapreFunc)
{
	iterator_t runner = start;
	
	while (runner != end)
	{
		if (IteratorComapreFunc(runner->data, data))
		{
			return runner;
		}
		runner = runner->next;
	}
	
	return NULL;
}






void SLLPrintList(linked_list_t *list)
{
	printf("Printing list: \n");
	SLListForEach(list->head, list->tail, PrintListHelper);
}

static int PrintListHelper(void *data)
{
	if (data != NULL)
	{
		printf("[%d] -> ", *(int *)data);
		
	}

	return 0;
}

