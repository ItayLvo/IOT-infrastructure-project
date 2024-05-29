#include <stdio.h>	/* printf */
#include "priority_queue.h"

int MatchInt(const void *item, const void *data_to_compare);
int CompareInt(const void *item, const void *data_to_compare);

int PQTestCreateDestroy(void);
int PQTestEnqueueDequeue(void);


int main()
{
	int success_status = 0;
	
	success_status += PQTestCreateDestroy();
	success_status += PQTestEnqueueDequeue();
	
	printf("\nTests summary: failed in %d tests\n", success_status);
	return success_status;
}



int PQTestCreateDestroy(void)
{
	priorityq_t *p_queue = PQCreate(CompareInt);
	PQDestroy(p_queue);
	
	return 0;
}

int PQTestEnqueueDequeue(void)
{
	priorityq_t *p_queue = PQCreate(CompareInt);
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



int MatchInt(const void *item, const void *data_to_compare)
{
 	return (*(int *)item == *(int *)data_to_compare);
}


int CompareInt(const void *item, const void *data_to_compare)
{
 	return (*(int *)item - *(int *)data_to_compare);
}
