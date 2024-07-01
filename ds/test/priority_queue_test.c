#include <stdio.h>	/* printf */
#include "priority_queue.h"

int MatchInt(const void *item, const void *data_to_compare);
int CompareInt(const void *item, const void *data_to_compare);

int PQTestCreateDestroy(void);
int PQTestEnqueueDequeue(void);
int PQTestPeek(void);
int PQTestIsEmptySize(void);
int PQTestEraseClear(void);

int main()
{
	int success_status = 0;
	
	success_status += PQTestCreateDestroy();
	success_status += PQTestEnqueueDequeue();
	success_status += PQTestPeek();
	success_status += PQTestIsEmptySize();
	success_status += PQTestEraseClear();
	
	printf("\nTests summary: failed in %d tests\n", success_status);
	
	return success_status;
}



int PQTestCreateDestroy(void)
{
	pq_t *p_queue = PQCreate(CompareInt);
	PQDestroy(p_queue);
	
	return 0;
}

int PQTestEnqueueDequeue(void)
{
	pq_t *p_queue = PQCreate(CompareInt);
	size_t element_count = 5, i = 0;
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 = 5;
	int count_failed_enqueues = 0;
	
	count_failed_enqueues += PQEnqueue(p_queue, &x4);
	count_failed_enqueues += PQEnqueue(p_queue, &x1);
	count_failed_enqueues += PQEnqueue(p_queue, &x5);
	count_failed_enqueues += PQEnqueue(p_queue, &x3);
	count_failed_enqueues += PQEnqueue(p_queue, &x2);
	
	printf("Enqueueing 4->1->5->3->2\nDequeueing and printing every element. Expected result: 5, 4, 3, 2, 1,\n");
	
	for (i = 0; i < element_count; ++i)
	{
		printf("%d, ", *(int *)PQDequeue(p_queue));
	}
	printf("\n");
	
	PQDestroy(p_queue);
	
	/* if count_failed_enqueues != 0, that means some enqueues failed -> return non-zero */
	return (0 != count_failed_enqueues);
}


int PQTestPeek(void)
{
	pq_t *p_queue = PQCreate(CompareInt);
	int x1 = 1, x4 = 4, x5 = 5;
	int count_failed_peeks = 0;
	
	PQEnqueue(p_queue, &x4);
	if (*(int *)PQPeek(p_queue) != 4)
	{
		++count_failed_peeks;
	}
	
	PQEnqueue(p_queue, &x1);
	if (*(int *)PQPeek(p_queue) != 1)
	{
		++count_failed_peeks;
	}
	
	PQEnqueue(p_queue, &x5);
	if (*(int *)PQPeek(p_queue) != 1)
	{
		++count_failed_peeks;
	}

	PQDestroy(p_queue);
	
	/* if count_failed_peeks != 0, that means some peeks failed -> return non-zero */
	return (0 != count_failed_peeks);
}


int PQTestIsEmptySize(void)
{
	pq_t *p_queue = PQCreate(CompareInt);
	int x1 = 1, x4 = 4;
	
	if(1 != PQIsEmpty(p_queue))
	{
		return 1;
	}
	
	
	if (0 != PQSize(p_queue))
	{
		return 1;
	}
	
	
	PQEnqueue(p_queue, &x4);
	PQEnqueue(p_queue, &x1);
	if (2 != PQSize(p_queue))
	{
		return 1;
	}
	
	PQDequeue(p_queue);
	if (1 != PQSize(p_queue))
	{
		return 1;
	}
	
	if(0 != PQIsEmpty(p_queue))
	{
		return 1;
	}
	
	PQDestroy(p_queue);
	(void)x1;
	(void)x4;

	return 0;
}


int PQTestEraseClear(void)
{
	pq_t *p_queue = PQCreate(CompareInt);
	int x1 = 1, x2 = 2, x3 = 3;
	int *test_result = NULL;
	
	PQEnqueue(p_queue, &x3);
	PQEnqueue(p_queue, &x1);
	PQEnqueue(p_queue, &x2);

	
	test_result = (int *)PQErase(p_queue, MatchInt, &x3);
	if (*test_result != 3)
	{
		return 1;
	}
	
	test_result = (int *)PQErase(p_queue, MatchInt, &x1);
	if (*test_result != 1)
	{
		return 1;
	}
	
	test_result = (int *)PQErase(p_queue, MatchInt, &x2);
	if (*test_result != 2)
	{
		return 1;
	}
	
	if (NULL != PQErase(p_queue, MatchInt, &x2))
	{
		return 1;
	}
	
	PQEnqueue(p_queue, &x3);
	PQEnqueue(p_queue, &x2);
	
	PQClear(p_queue);
	if (!PQIsEmpty(p_queue))
	{
		return 1;
	}
	
	
	PQDestroy(p_queue);
	
	return 0;
}


int MatchInt(const void *item, const void *data_to_compare)
{
 	return (*(int *)item == *(int *)data_to_compare);
}


int CompareInt(const void *item, const void *data_to_compare)
{
 	return (*(int *)item - *(int *)data_to_compare);
}
