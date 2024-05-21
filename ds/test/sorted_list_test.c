#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/dllist.h"
#include "../include/sorted_list.h"

int MatchInt(const void *item, const void *data_to_compare);
int CompareInt(const void *item, const void *data_to_compare);

int main()
{
	slist_t *list = SListCreate(CompareInt);
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 = 5;
	slist_iterator_t iterator;
	
	printf("IsEmpty? %d\n", SListIsEmpty(list));
	printf("List count = %ld\n\n", SListCount(list));
	
	printf("Pushing 5->2->1->4->3: \n");
	iterator = SListInsert(list,  &x5);
	iterator = SListInsert(list,  &x2);
	iterator = SListInsert(list,  &x1);
	iterator = SListInsert(list,  &x4);
	iterator = SListInsert(list,  &x3);
	
	printf("IsEmpty? %d\n\n", SListIsEmpty(list));
	
	iterator = SListBegin(list);
	printf("%d\n", *(int *)SListGetData(iterator));
	iterator = SListNext(iterator);
	printf("%d\n", *(int *)SListGetData(iterator));
	iterator = SListNext(iterator);
	printf("%d\n", *(int *)SListGetData(iterator));
	iterator = SListNext(iterator);
	printf("%d\n", *(int *)SListGetData(iterator));
	iterator = SListNext(iterator);
	printf("%d\n\n", *(int *)SListGetData(iterator));
	
	printf("List count = %ld\n\n", SListCount(list));
	
	printf("Popping front, popping back: \n");
	SListPopFront(list);
	SListPopBack(list);
	printf("List count = %ld\n", SListCount(list));
	
	iterator = SListBegin(list);
	printf("%d\n", *(int *)SListGetData(iterator));
	iterator = SListNext(iterator);
	printf("%d\n", *(int *)SListGetData(iterator));
	iterator = SListNext(iterator);
	printf("%d\n\n", *(int *)SListGetData(iterator));
	
	
	printf("List count = %ld\n", SListCount(list));
	printf("Removing iterator where FindIf == 2: \n");
	iterator = SListFindIf(SListBegin(list), SListEnd(list), &x2, MatchInt);
	printf("found = %d\n", *(int *)SListGetData(iterator));
	iterator = SListRemove(iterator);
	printf("iterator after remove(iterator): %d\n", *(int *)SListGetData(iterator));
	printf("List count = %ld\n\n", SListCount(list));
	
	
	SListDestroy(list);
	
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
