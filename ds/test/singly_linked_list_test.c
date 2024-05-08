#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/singly_linked_list.h"


int CompareInt(void *item, const void *data_to_compare);

int main()
{
	linked_list_t *list = SLListCreate();
	iterator_t iterator = SLListGetBegin(list);
	int x1 = 5, x2 = 4, x3 = 3, x4 = 2;
	
	iterator = SLListInsert(list, &x1, iterator);
	iterator = SLListInsert(list, &x2, iterator);
	
	iterator = SLListInsert(list, &x3, iterator);
	SLListInsert(list, &x4, iterator);
	
	printf("GetData(GetBegin)): %d\n", *(int *)SLListGetData(SLListGetBegin(list)));
	
	printf("SLListGetData(iterator): %d\n", *(int *)SLListGetData(iterator));
	printf("SLListNext\n");
	iterator = SLListNext(iterator);
	printf("getdata(iterator): %d\n", *(int *)SLListGetData(iterator));
	iterator = SLListNext(iterator);
	printf("%d\n", *(int *)SLListGetData(iterator));
	iterator = SLListNext(iterator);
	printf("%d\n", *(int *)SLListGetData(iterator));
	
	
	SLListSetData(iterator, &x3);
	
	
	printf("list size = %ld\n",SLListCount(list));
	SLLPrintList(list);
	printf("iterator: %d\n", *(int *)SLListGetData(iterator));
	
	
	SLListRemove(list, iterator);
	
	
	printf("list size = %ld\n",SLListCount(list));
	SLLPrintList(list);
	
	
	iterator = SLListFind(SLListGetBegin(list), SLListGetEnd(list), &x3, CompareInt);
	printf("getdata(iterator): %d\n", *(int *)SLListGetData(iterator));
	iterator = SLListGetBegin(list);
	printf("is the list empty? %d\n", SLListIsEmpty(list));
	SLListRemove(list, iterator);
	SLLPrintList(list);
	SLListRemove(list, iterator);
	SLLPrintList(list);
	printf("is the list empty? %d\n", SLListIsEmpty(list));
	printf("list size = %ld\n",SLListCount(list));
	SLLPrintList(list);
	SLListRemove(list, iterator);
	SLLPrintList(list);
	printf("is the list empty? %d\n", SLListIsEmpty(list));
	printf("list size = %ld\n",SLListCount(list));
	
	SLListDestroy(list);
	return 0;
}



int CompareInt(void *item, const void *data_to_compare)
{
 	return (*(int *)item == *(int *)data_to_compare);
}

