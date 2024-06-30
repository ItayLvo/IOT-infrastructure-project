#include <stdio.h>	/* printf */

#include "heap.h"

int CompareInt(const void *item, const void *data_to_compare);


int main()
{
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 = 5, x6 = 6;
	
	heap_t *heap = HeapCreate(CompareInt);
	
	HeapPush(heap, &x6);
	HeapPush(heap, &x3);
	HeapPush(heap, &x2);
	HeapPush(heap, &x4);
	HeapPush(heap, &x1);
	HeapPush(heap, &x5);
	
	HeapDestroy(heap);
	
	return 0;
}



int CompareInt(const void *item, const void *data_to_compare)
{
 	return (*(int *)item - *(int *)data_to_compare);
}
