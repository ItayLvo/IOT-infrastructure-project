#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h> /* size_t */

typedef struct queue queue_t;

/*
Description: Create the queue
Return Value: on success - pointer to created queue, on failure - NULL
Time Complexity - O(1)
Space: O(1)
*/
queue_t *QueueCreate(void);

/*
Description: Destroy the queue
Return Value: void
Time Complexity - O(n) 
Space: O(1)
*/
void QueueDestroy(queue_t *queue);

/*
Description: Add new element to end
Return Value: int (1 - new element added successfully, or 0 - failed to add new element)
Time Complexity - O(1) 
Space: O(1)
*/
int QueueEnqueue(queue_t *queue, void *data);

/*
Description: Remove element from start
Return Value: void *  - pointer to the data that was removed from the queue
Time Complexity - O(1) 
Space: O(1)
*/
void QueueDequeue(queue_t *queue);

/*
Description:  see next element will de removed (get last element in queue start)
Return Value: void *  - pointer to the data of the last (start) element in the queue. User may access unvalid data by using peek 
Time Complexity - O() 
Space: O()
*/
void *QueuePeek(const queue_t *queue);

/*
Description: see count elements
Return Value: size_t
Time Complexity - O(n)
Space: O(1)
*/
size_t QueueSize(queue_t *queue);

/*
Description: see if queue is empty
Return Value: int (bool value, 0- queue is empty, 1- is not empty)
Time Complexity - O(1) 
Space: O(1)
*/
int QueueIsEmpty(queue_t *queue);

/*
Description: links end of queue1 to start of queue2
Return Value: queue_t *(returning ptr to appended queues starting from queue 1)
Time Complexity - O(1) 
Space: O(1)
*/
queue *QueueAppend(queue_t *queue1, queue_t *queue2);


#endif /* end __QUEUE_H__ */
