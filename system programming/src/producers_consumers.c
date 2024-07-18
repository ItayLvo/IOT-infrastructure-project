/*
Date: 
Status: 
Reviwer: 
*/
#define _BSD_SOURCE	/* usleep */

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

#include "producers_consumers.h"
#include "sllist.h"	/* singly linked list data structure */


#define MAX_MESSAGES 5
#define NUM_THREADS 3

/* forward declerations */
static void *ProduceBusyWait(void *param);
static void *ConsumeBusyWait(void *param);
static void *ProduceMutex(void *param);
static void *ConsumeMutex(void *param);

/* global variables */
static atomic_int counter = ATOMIC_VAR_INIT(0);	/* atomic product counter for exercise 1 */
static pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static linked_list_t *list = NULL;




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
		usleep(200000);
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
		
		usleep(200000);
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
