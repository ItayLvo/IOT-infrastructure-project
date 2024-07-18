/*
Date: 
Status: 
Reviwer: 
*/
#define _BSD_SOURCE	/* usleep */
#define _GNU_SOURCE	/* thread id */

#include <stdatomic.h>	/* for atomic_ types */
#include <stdio.h>	/* printf */
#include <stdlib.h>	/* exit */
#include <string.h> /* strstr, atoi, strcmp */
#include <fcntl.h> /* O_* constants (O_CREAT)*/
#include <semaphore.h>	/* POSIX semaphore functions and definitions */
#include <sys/stat.h>	/* permissions macros */
#include <pthread.h>	/* threads */
#include <unistd.h>		/* gettid */
#include <sys/types.h>	/* thread id */
#include <semaphore.h>	/* POSIX semaphore functions and definitions */

#include "producers_consumers.h"
#include "sllist.h"		/* singly linked list data structure */
#include "cbuffer.h"	/* fixed size queue DS */

#define Q_SIZE 10
#define MAX_MESSAGES 5
#define NUM_THREADS 3

/* forward declerations */
static void *ProduceBusyWait(void *param);
static void *ConsumeBusyWait(void *param);

static void *ProduceMutex(void *param);
static void *ConsumeMutex(void *param);

static void *ProduceSemaphore(void *param);
static void *ConsumeSemaphore(void *param);

static void *ProduceTwoSemaphores(void *param);
static void *ConsumeTwoSemaphores(void *param);


/**** exercise 4+5 fixed size queue ****/
typedef struct fsq
{
	size_t capacity;
	size_t first_elment_index;
	size_t elements_in_queue;
	int arr[Q_SIZE];
} fsq_t;

void EnQueue(fsq_t *queue, int new_data);
void DeQuque(fsq_t *queue);
int PeekQueue(const fsq_t *queue);
/********************************/

fsq_t buffer = {0};


/*** global static variables ***/

/* atomic counter for exercise 1 */
static atomic_int counter = ATOMIC_VAR_INIT(0);	

/* exercise 2 + 3 */
static pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static linked_list_t *list = NULL;
static sem_t g_sem;

/* exercise 4 + 5 */
static sem_t g_empty_sem;
static sem_t g_full_sem;





/******** Exercise 4: Fixed size queue + 2 Semahpre + 1 Mutex ********/

int RunTwoSemaphores(void)
{
	pthread_t consumers[NUM_THREADS - 2];
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
