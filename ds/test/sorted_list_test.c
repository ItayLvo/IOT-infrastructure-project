#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/dllist.h"
#include "../include/sorted_list.h"

int MatchInt(const void *item, const void *data_to_compare);
int CompareInt(const void *item, const void *data_to_compare);

int main()
{
	slist_t *list = SListCreate(CompareInt);
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4;
	slist_iterator_t iterator;
	
	iterator = SListInsert(list,  &x2);
	iterator = SListInsert(list,  &x1);
	iterator = SListInsert(list,  &x4);
	iterator = SListInsert(list,  &x3);
	
	iterator = SListBegin(list);
	printf("%d\n", *(int *)SListGetData(iterator));
	iterator = SListNext(iterator);
	printf("%d\n", *(int *)SListGetData(iterator));
	iterator = SListNext(iterator);
	printf("%d\n", *(int *)SListGetData(iterator));
	iterator = SListNext(iterator);
	printf("%d\n", *(int *)SListGetData(iterator));
	
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
