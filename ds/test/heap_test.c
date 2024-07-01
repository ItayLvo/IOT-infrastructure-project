#include <stdio.h>	/* printf */

#include "heap.h"

int CompareInt(const void *item, const void *data_to_compare);
int MatchInt(const void *item, const void *data_to_compare);

int main()
{
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 = 5, x6 = 6;
	int *remove_test = NULL;
	
	heap_t *heap = HeapCreate(CompareInt);
	
	printf("IsEmpty?\t%d\n", HeapIsEmpty(heap));
	
	HeapPush(heap, &x1);
	
	printf("peek:\t%d\n", *(int *)HeapPeek(heap));
	
	HeapPush(heap, &x3);
	
	printf("peek:\t%d\n", *(int *)HeapPeek(heap));
	
	HeapPush(heap, &x2);
	
	printf("peek:\t%d\n", *(int *)HeapPeek(heap));
	
	HeapPush(heap, &x4);
	
	printf("peek:\t%d\n", *(int *)HeapPeek(heap));
	
	HeapPush(heap, &x6);
	
	printf("peek:\t%d\n", *(int *)HeapPeek(heap));
	
	HeapPush(heap, &x5);
	
	printf("IsEmpty?\t%d\n", HeapIsEmpty(heap));
	
	remove_test = (int *)HeapRemove(heap, MatchInt, &x4);
	printf("removed: %d\n", *remove_test);
	
	remove_test = (int *)HeapRemove(heap, MatchInt, &x3);
	printf("removed: %d\n", *remove_test);
	
	
	HeapPush(heap, &x6);
	
	HeapPop(heap);
	HeapPop(heap);
	HeapPop(heap);
	
	HeapDestroy(heap);
	
	return 0;
}



int CompareInt(const void *item, const void *data_to_compare)
{
 	return (*(int *)item - *(int *)data_to_compare);
}


int MatchInt(const void *item, const void *data_to_compare)
{
 	return (*(int *)item == *(int *)data_to_compare);
}
