/*
Date: 
Status: 
Reviwer: 
*/

#define _BSD_SOURCE	/* usleep */
#define _GNU_SOURCE	/* thread id */

#include <stdatomic.h>	/* for atomic_ types */
#include <stdio.h>	/* printf */
#include <stdlib.h>	/* rand */
#include <semaphore.h>	/* POSIX semaphore functions and definitions */
#include <pthread.h>	/* threads */
#include <unistd.h>		/* gettid, usleep */
#include <sys/types.h>	/* tid */

#include "producers_consumers.h"
#include "sllist.h"		/* singly linked list data structure */


#define Q_SIZE 10
#define MAX_MESSAGES 5
#define NUM_THREADS 3
#define BARRIER_COUNT NUM_THREADS


/* forward declerations */
static void *ProduceBusyWait(void *param);
static void *ConsumeBusyWait(void *param);

static void *ProduceMutex(void *param);
static void *ConsumeMutex(void *param);

static void *ProduceSemaphore(void *param);
static void *ConsumeSemaphore(void *param);

static void *ProduceTwoSemaphores(void *param);
static void *ConsumeTwoSemaphores(void *param);

static void *ProduceThreadSafeFSQ(void *queue);
static void *ConsumeThreadSafeFSQ(void *queue);

static void *ProduceCond(void *param);
static void *ConsumeCond(void *param);

__thread int tls_var = 42;
__thread int tls_var2;

/**** exercise 4+5 fixed size queue data structure ****/
typedef struct fsq
{
	size_t capacity;
	size_t first_elment_index;
	size_t elements_in_queue;
	int arr[Q_SIZE];
	/* exercise 5 */
	pthread_mutex_t enqueue_lock;
	pthread_mutex_t dequeue_lock;
	sem_t empty_sem;
	sem_t full_sem;
} fsq_t;


void EnQueueThreadSafe(fsq_t *queue, int new_data);
void DeQuqueThreadSafe(fsq_t *queue);


void EnQueue(fsq_t *queue, int new_data);
void DeQuque(fsq_t *queue);
int PeekQueue(const fsq_t *queue);
/**********************************************/

/******* static global variables *******/
/* atomic counter for exercise 1 */
static atomic_int counter = ATOMIC_VAR_INIT(0);	

/* exercise 2 + 3 */
static pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static linked_list_t *list = NULL;
static sem_t g_sem;

/* exercise 4 */
static fsq_t buffer = {0};
static sem_t g_empty_sem;
static sem_t g_full_sem;

/* exercise 5 */
static fsq_t queue = {0};

/* exercise 6 */
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int g_count_consumed = 0;
#define NOT_CONSUMED 0
#define CONSUMED 1
static int g_is_consumed = CONSUMED;



/******** Exercise 6: Conditional variable, 1 mutex, 1 semaphore ********/

int RunConditionalVariable(void)
{
	pthread_t consumers[NUM_THREADS];
	pthread_t producers[1];
	int status = SUCCESS;
	size_t i = 0;
	
	list = SLListCreate();
	sem_init(&g_sem, 0, 0);
	
	/* creating and running the threads */
	for (i = 0; i < NUM_THREADS; ++i)
	{
		status = pthread_create(&consumers[i], NULL, &ConsumeCond, NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to create producer thread %lu\n", i);
		    return status;
		}
		status = pthread_create(&producers[i], NULL, &ProduceCond, NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to create producer thread %lu\n", i);
		    return status;
		}
	}
	
	/* joining the threads */
	for (i = 0; i < NUM_THREADS; ++i)
	{
		status = pthread_join(consumers[i], NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to join consumer thread %lu\n", i);
		    return status;
		}
		status = pthread_join(producers[i], NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to join producer thread %lu\n", i);
		    return status;
		}
	}
	
	return status;
}



#define NOT_CONSUMED 0
#define CONSUMED 1
#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 5
#define BARRIER_COUNT NUM_CONSUMERS

static volatile atomic_int g_count_consumed = 0;
static volatile atomic_int g_is_consumed = CONSUMED;

static void *ProducerThread(void *param)
{
	int message = 0;
	int sem_value = 0;
	srand(time(NULL));	
	
	while(1)
	{
		/* produce message */
		message = rand() % 10;
		
		/* lock mutex and check conditional variable */
		pthread_mutex_lock(&job_queue_mutex);
		while (g_is_consumed == NOT_CONSUMED)
		{
			pthread_cond_wait(&cond, &job_queue_mutex);
		}
		
		/* at this point product is consumed - push message to list */
		SLListInsert(list, &message, SLListGetEnd(list));
		
		/* update global vars */	
		g_is_consumed = NOT_CONSUMED;
		g_count_consumed = 0;
		
		/* signal waiting threads and unlock mutex */
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&job_queue_mutex);
		
		/* post to binary semaphore */
		sem_post(&g_sem);
	}
		
	return NULL;
}


static void *ConsumeThread(void *param)
{
	int *msg_ptr = NULL;
	int msg_value = 0;
	
	while(1)
	{
		
		/* lock mutex and check conditional variable */
		pthread_mutex_lock(&job_queue_mutex);
		while (g_is_consumed == CONSUMED)
		{
			pthread_cond_wait(&cond, &job_queue_mutex);
		}
		
		/* at this point - product exists, read message (without removing it) */
		msg_ptr = SLListGetData(SLListGetBegin(list));
		msg_value = *msg_ptr;
		
		/* update count of threads that consumed the product */
		++g_count_consumed;
		
		/* if count reached barrier - remove product (remove from list, wait on binary semaphore, update global var), then signal */
		if (g_count_consumed == BARRIER_COUNT)
		{
			SLListRemove(SLListGetBegin(list));
			g_is_consumed = CONSUMED;
			sem_wait(&g_sem);
			pthread_cond_signal(&cond);
		}
		
		pthread_mutex_unlock(&job_queue_mutex);
	}
	
	/* use product/message */
	printf("%d\n", msg_value);

	return NULL;
}

tal.tabib@infinitylabs.il





/******** Exercise 5: Thread-Safe Fixed size queue (+ 2 Semahpre + 2 Mutex) ********/


int RunThreadSafeFSQ(void)
{
	pthread_t consumers[NUM_THREADS];
	pthread_t producers[NUM_THREADS];
	int status = SUCCESS;
	size_t i = 0;
	
	queue.capacity = Q_SIZE;
	/* init full and empty semaphores, and enqueue and dequeue mutex locks */
	sem_init(&(queue.empty_sem), 0, Q_SIZE);
	sem_init(&(queue.full_sem), 0, 0);
	pthread_mutex_init(&(queue.enqueue_lock), NULL);
	pthread_mutex_init(&(queue.dequeue_lock), NULL);
	
	
	/* creating and running the threads */
	for (i = 0; i < NUM_THREADS; ++i)
	{
		status = pthread_create(&consumers[i], NULL, &ConsumeThreadSafeFSQ, NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to create producer thread %lu\n", i);
		    return status;
		}
		status = pthread_create(&producers[i], NULL, &ProduceThreadSafeFSQ, NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to create producer thread %lu\n", i);
		    return status;
		}
	}
	
	/* joining the threads */
	for (i = 0; i < NUM_THREADS; ++i)
	{
		status = pthread_join(consumers[i], NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to join consumer thread %lu\n", i);
		    return status;
		}
		status = pthread_join(producers[i], NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to join producer thread %lu\n", i);
		    return status;
		}
	}
	
	return status;
}


static void *ProduceThreadSafeFSQ(void *param)
{
	pid_t tid = gettid();
	int message = 0;
	int sem_value = 0;
	srand(time(NULL));	
	message = rand() % 10;
	
	while(1)
	{
		EnQueueThreadSafe(&queue, message);
		
		sem_getvalue(&(queue.full_sem), &sem_value);
		printf("Produced. ID: %d\n", tid);
		printf("Full semaphore value: %d\n", sem_value);
		sem_getvalue(&(queue.empty_sem), &sem_value);
		printf("Empty semaphore value: %d\n\n", sem_value);
				
		message = rand() % 10;
		usleep(message * 1000000);
	}
	
	(void)param;
	return NULL;
}




static void *ConsumeThreadSafeFSQ(void *param)
{
	pid_t tid = gettid();
	int message = 0;
	int sem_value = 0;
	
	while(1)
	{
		message = PeekQueue(&queue);
		DeQuqueThreadSafe(&queue);
		
		sem_getvalue(&(queue.full_sem), &sem_value);
		printf("Consumed. ID: %d\n", tid);
		printf("Full semaphore value: %d\n", sem_value);
		sem_getvalue(&(queue.empty_sem), &sem_value);
		printf("Empty semaphore value: %d\n\n", sem_value);
		
		usleep(message * 1000000);
	}

	(void)param;
	return NULL;
}







/******** Exercise 4: Fixed size queue + 2 Semahpre + 1 Mutex ********/

int RunTwoSemaphores(void)
{
	pthread_t consumers[NUM_THREADS];
	pthread_t producers[NUM_THREADS];
	int status = SUCCESS;
	size_t i = 0;
	
	buffer.capacity = Q_SIZE;
	
	/* initialzie full and empty semaphores. at first, all sems are empty, 0 are full */
	sem_init(&g_empty_sem, 0, Q_SIZE);
	sem_init(&g_full_sem, 0, 0);
	
	/* creating and running the threads */
	for (i = 0; i < NUM_THREADS; ++i)
	{
		status = pthread_create(&consumers[i], NULL, &ConsumeTwoSemaphores, NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to create producer thread %lu\n", i);
		    return status;
		}
		status = pthread_create(&producers[i], NULL, &ProduceTwoSemaphores, NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to create producer thread %lu\n", i);
		    return status;
		}
	}
	
	/* joining the threads */
	for (i = 0; i < NUM_THREADS; ++i)
	{
		status = pthread_join(consumers[i], NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to join consumer thread %lu\n", i);
		    return status;
		}
		status = pthread_join(producers[i], NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to join producer thread %lu\n", i);
		    return status;
		}
	}
	
	return status;
}

/*    pseudo code: */
/*	  P(emptyCount)*/
/*    P(useQueue)*/
/*    putItemIntoQueue(item)*/
/*    V(useQueue)*/
/*    V(fullCount)*/

static void *ProduceTwoSemaphores(void *param)
{
	pid_t tid = gettid();
	int message = 0;
	int sem_value = 0;
	srand(time(NULL));	
	message = rand() % 10;
	
	while(1)
	{
		/* check if there are any empty boxes */
		sem_wait(&g_empty_sem);
		/* if empty box found - lock the mutex and insert message */
		pthread_mutex_lock(&job_queue_mutex);
		EnQueue(&buffer, message);
		
		/* post to full sempahore */		
		sem_post(&g_full_sem);
		
		sem_getvalue(&g_full_sem, &sem_value);
		printf("Produced. ID: %d\n", tid);
		printf("Full semaphore value: %d\n", sem_value);
		sem_getvalue(&g_empty_sem, &sem_value);
		printf("Empty semaphore value: %d\n\n", sem_value);
		
		/* unlock mutex */
		pthread_mutex_unlock(&job_queue_mutex);
		
		
		message = rand() % 10;
		usleep(message * 1000000);
	}
	
	(void)param;
	return NULL;
}


/*	  pseudo code: */
/*    P(fullCount)*/
/*    P(useQueue)*/
/*    item ← getItemFromQueue()*/
/*    V(useQueue)*/
/*    V(emptyCount)*/
static void *ConsumeTwoSemaphores(void *param)
{
	pid_t tid = gettid();
	int message = 0;
	int sem_value = 0;
	
	while(1)
	{
		/* check if there are any full boxes */
		sem_wait(&g_full_sem);
		/* if full box found - lock the mutex and consume message */
		pthread_mutex_lock(&job_queue_mutex);
		message = PeekQueue(&buffer);
		DeQuque(&buffer);
		
		/* post to empty semaphore */
		sem_post(&g_empty_sem);
		
		sem_getvalue(&g_full_sem, &sem_value);
		printf("Consumed. ID: %d\n", tid);
		printf("Full semaphore value: %d\n", sem_value);
		sem_getvalue(&g_empty_sem, &sem_value);
		printf("Empty semaphore value: %d\n\n", sem_value);
		
		/* unlock mutex */
		pthread_mutex_unlock(&job_queue_mutex);
		
		usleep(message * 1000000);
	}


	(void)param;
	return NULL;
}







/******** Exercise 3: Linked List + 1 Semahpre + 1 Mutex ********/

int RunSemaphore(void)
{
	pthread_t consumers[NUM_THREADS];
	pthread_t producers[NUM_THREADS];
	int status = SUCCESS;
	size_t i = 0;
	list = SLListCreate();
	
	/* initialzie semaphore: shared between threads, initial value 0 */
	sem_init(&g_sem, 0, 0);
	
	/* creating and running the threads */
	for (i = 0; i < NUM_THREADS; ++i)
	{
		status = pthread_create(&consumers[i], NULL, &ConsumeSemaphore, NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to create producer thread %lu\n", i);
		    return status;
		}
		status = pthread_create(&producers[i], NULL, &ProduceSemaphore, NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to create producer thread %lu\n", i);
		    return status;
		}
	}
	
	/* joining the threads */
	for (i = 0; i < NUM_THREADS; ++i)
	{
		status = pthread_join(consumers[i], NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to join consumer thread %lu\n", i);
		    return status;
		}
		status = pthread_join(producers[i], NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to join producer thread %lu\n", i);
		    return status;
		}
	}
	
	return status;
}



static void *ProduceSemaphore(void *param)
{
	pid_t tid = gettid();
	int message = 0;
	int sem_value = 0;
	srand(time(NULL));	
	
	message = rand() % 10;
	
	while(1)
	{
		/* lock mutex before accessing semaphore and list */
		pthread_mutex_lock(&job_queue_mutex);
		
		if (sem_getvalue(&g_sem, &sem_value) != 0)
		{
			/* if getvalue() failed */
			printf("getvalue failed!\n");
			pthread_mutex_unlock(&job_queue_mutex);
			return NULL;
		}
		
		if (sem_value < MAX_MESSAGES)
		{

			SLListInsert(list, &message, SLListGetEnd(list));
						
			/* post to sempahore to signal other threads and unlock mutex */		
			sem_post(&g_sem);
			sem_getvalue(&g_sem, &sem_value);
			printf("Produced. Current semaphore value: %d\n", sem_value);
		}
		
		pthread_mutex_unlock(&job_queue_mutex);
		
		message = rand() % 10;
		printf("thread: %d\n\n", tid);
		usleep(message * 1000000);
	}
		
	
	(void)param;
	return NULL;
}


static void *ConsumeSemaphore(void *param)
{
	pid_t tid = gettid();
	int *msg_ptr = NULL;
	int sem_value = 0;
	int sleep_time = 0;
	srand(time(NULL));	
	
	while(1)
	{
		/* try to wait on semaphore. if not empty, lock mutex and do work */
		sem_wait(&g_sem);
		pthread_mutex_lock(&job_queue_mutex);
		
		msg_ptr = SLListGetData(SLListGetBegin(list));
		if (msg_ptr == NULL)
		{
			/* if SLListGetData() failed */
			printf("GetData failed!\n");
			pthread_mutex_unlock(&job_queue_mutex);
			return NULL;
		}
		
		
		SLListRemove(SLListGetBegin(list));
		sem_getvalue(&g_sem, &sem_value);
		printf("Consumed. Current semaphore value: %d\n" ,sem_value);
		
		pthread_mutex_unlock(&job_queue_mutex);
		
		printf("thread: %d\n\n", tid);
		
		srand(time(NULL));
		sleep_time = rand() % 10;
		usleep(sleep_time * 1000000);
	}
		
	
	(void)param;
	return NULL;
}










/******** Exercise 2: Linked List + 1 Mutex ********/
int RunMutex(void)
{
	pthread_t consumers[NUM_THREADS];
	pthread_t producers[NUM_THREADS];
	int status = SUCCESS;
	size_t i = 0;
	list = SLListCreate();
	
	
	/* creating and running the threads */
	for (i = 0; i < NUM_THREADS; ++i)
	{
		status = pthread_create(&consumers[i], NULL, &ConsumeMutex, NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to create producer thread %lu\n", i);
		    return status;
		}
		status = pthread_create(&producers[i], NULL, &ProduceMutex, NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to create producer thread %lu\n", i);
		    return status;
		}
	}
	
	/* joining the threads */
	for (i = 0; i < NUM_THREADS; ++i)
	{
		status = pthread_join(consumers[i], NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to join consumer thread %lu\n", i);
		    return status;
		}
		status = pthread_join(producers[i], NULL);
		if (status != 0)
		{
		    fprintf(stderr, "failed to join producer thread %lu\n", i);
		    return status;
		}
	}
	
	return status;
}


static void *ProduceMutex(void *param)
{
	pid_t tid = gettid();
	int message = 0;
	srand(time(NULL));	
	message = rand() % 10;
	
	while(1)
	{
		pthread_mutex_lock(&job_queue_mutex);
		

		if (SLListCount(list) < MAX_MESSAGES)
		{
			SLListInsert(list, &message, SLListGetEnd(list));
			printf("Produced %d. Current amount: %lu\n", message, SLListCount(list));
		}
		
		
		pthread_mutex_unlock(&job_queue_mutex);
		
		message = rand() % 10;
		printf("thread: %d\n\n", tid);
		usleep(2000000);
	}
		
	
	(void)param;
	return NULL;
}


static void *ConsumeMutex(void *param)
{
	pid_t tid = gettid();
	int message = 0;
	int *msg_ptr = NULL;
	
	while(1)
	{
		pthread_mutex_lock(&job_queue_mutex);

		
		if (SLListIsEmpty(list) != 1)
		{
			msg_ptr = SLListGetData(SLListGetBegin(list));
			message = *msg_ptr;
			SLListRemove(SLListGetBegin(list));
			printf("Consumed %d. Current amount: %lu\n", message, SLListCount(list));
		}
		
		pthread_mutex_unlock(&job_queue_mutex);
				
		printf("thread: %d\n\n", tid);
		usleep(2000000);
	}
		
	
	(void)param;
	return NULL;
}












/******** Exercise 1: spinlock ********/
int RunSpinLock(void)
{
	pthread_t thread_consumer;
	pthread_t thread_producer;
	int status = SUCCESS;
	
	
	status = pthread_create(&thread_consumer, NULL, &ConsumeBusyWait, NULL);
	if (status != 0)
    {
		fprintf(stderr, "failed to create consumer thread\n");
		return status;
    }

    status = pthread_create(&thread_producer, NULL, &ProduceBusyWait, NULL);
    if (status != 0)
    {
        fprintf(stderr, "failed to create producer thread\n");
        return status;
    }
	
	status = pthread_join(thread_consumer, NULL);
	 if (status != 0)
    {
        fprintf(stderr, "thread_consumer join failed\n");
        return status;
    }
	status = pthread_join(thread_producer, NULL);
	 if (status != 0)
    {
        fprintf(stderr, "thread_producer join failed\n");
        return status;
    }
	
	return status;
}


static void *ProduceBusyWait(void *param)
{
	while(1)
	{
		while (atomic_load(&counter) >= MAX_MESSAGES)
		{
			/* busy wait */ ;
		}
		
		atomic_fetch_add(&counter, 1);
		printf("Produced. Current amount: %d\n", atomic_load(&counter));
		
		usleep(200000);
	}
		
	
	(void)param;
	return NULL;
}


static void *ConsumeBusyWait(void *param)
{
	while(1)
	{
		while (atomic_load(&counter) <= 0)
		{
			/* busy wait */ ;
		}
		
		atomic_fetch_sub(&counter, 1);
		printf("Consumed. Current amount: %d\n", atomic_load(&counter));
		
		usleep(200000);
	}
		
	
	(void)param;
	return NULL;
}









/*********** fixed size queue implementation *************/
void EnQueue(fsq_t *queue, int new_data)
{
	if (queue->elements_in_queue == queue->capacity)
	{
		return;
	}
	
	queue->arr[(queue->first_elment_index + queue->elements_in_queue) % queue->capacity] = new_data;
	++queue->elements_in_queue;
	
}

void DeQuque(fsq_t *queue)
{
	if (0 == queue->elements_in_queue)
	{
		return;
	}
	
	queue->first_elment_index = (queue->first_elment_index + 1) % queue->capacity;
	--queue->elements_in_queue;
}

int PeekQueue(const fsq_t *queue)
{
	return queue->arr[queue->first_elment_index];
}
/***************************************************/



/*********** Exercise 5 - Thread safe FSQ *************/
void EnQueueThreadSafe(fsq_t *queue, int new_data)
{
	sem_wait(&(queue->empty_sem));
	pthread_mutex_lock(&(queue->enqueue_lock));
	
	queue->arr[(queue->first_elment_index + queue->elements_in_queue) % queue->capacity] = new_data;
	++queue->elements_in_queue;
	
	pthread_mutex_unlock(&(queue->enqueue_lock));
	sem_post(&(queue->full_sem));
}

void DeQuqueThreadSafe(fsq_t *queue)
{
	sem_wait(&(queue->full_sem));
	pthread_mutex_lock(&(queue->enqueue_lock));
	
	queue->first_elment_index = (queue->first_elment_index + 1) % queue->capacity;
	--queue->elements_in_queue;
	
	pthread_mutex_unlock(&(queue->enqueue_lock));
	sem_post(&(queue->empty_sem));
}


/***************************************************/
