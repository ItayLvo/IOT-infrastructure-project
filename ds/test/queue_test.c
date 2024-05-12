#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/singly_linked_list.h"
#include "../include/queue.h"


int main()
{
	int x1 = 5;
	int *xp = &x1;
	queue_t *queue1 = QueueCreate();
	
	int y1 = 10;
	int y2 = 11;
	int y3 = 12;
	queue_t *queue2 = QueueCreate();
	
	QueueEnqueue(queue1, xp);
	printf("number of elements after pushing (5) one time in queue1: %ld\n", QueueSize(queue1));
	printf("Peek (queue1): %d\n", *(int *)QueuePeek(queue1));
	
	QueueEnqueue(queue2, &y1);
	QueueEnqueue(queue2, &y2);
	QueueEnqueue(queue2, &y3);
	printf("Peek (queue2) after pushing 10->11->12: %d\n", *(int *)QueuePeek(queue2));
	
	queue1 = QueueAppend(queue1, queue2);
	printf("number of elements after queue1 after append: %ld\n", QueueSize(queue1));
	
	
	QueueDequeue(queue1);
	QueueDestroy(queue1);
	QueueDestroy(queue2);
	return 0;
}

