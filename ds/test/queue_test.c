#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/singly_linked_list.h"


int CompareInt(void *item, const void *data_to_compare);

int TestListFunctions(linked_list_t *list);


int main()
{
	linked_list_t *list = SLListCreate();
	
	TestListFunctions(list);

	SLListDestroy(list);
	
	return 0;
}


int TestListFunctions(linked_list_t *list)
{
	iterator_t iterator = NULL;
	int return_status = 0;
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 = 5;
	
	printf("pushing 1,2,3,4,5\n\n");
	iterator = SLListGetBegin(list);
	iterator = SLListInsert(list, &x1, iterator);
	iterator = SLListInsert(list, &x2, iterator);
	iterator = SLListInsert(list, &x3, iterator);
	iterator = SLListInsert(list, &x4, iterator);
	iterator = SLListInsert(list, &x5, iterator);
	
	if (iterator == SLListGetEnd(list))
	{
		return_status += 1;
	}
	SLLPrintList(list);
	printf("\n\n");
	
	printf("finding node where data = 3\n");
	iterator = SLListFind(SLListGetBegin(list), SLListGetEnd(list), &x3, CompareInt);
	if (NULL == iterator)
	{
		return_status += 1;
	}
	
	printf("found node where data = %d\n", *(int *)SLListGetData(iterator));
	
	printf("removing the node: \n");
	SLListRemove(list, iterator);
	SLLPrintList(list);
	printf("\n");
	
	printf("list size = %ld\n",SLListCount(list));
	
	printf("is the list empty (0 for no)? : %d\n", SLListIsEmpty(list));
	
	printf("removing node\n");
	SLListRemove(list, SLListGetBegin(list));
	printf("removing node\n");
	SLListRemove(list, SLListGetBegin(list));
	printf("removing node\n");
	SLListRemove(list, SLListGetBegin(list));
	printf("removing node\n");
	SLListRemove(list, SLListGetBegin(list));
	
	printf("list size = %ld\n",SLListCount(list));
	printf("is the list empty (0 for no)? : %d\n", SLListIsEmpty(list));
	
	return return_status;
}



int CompareInt(void *item, const void *data_to_compare)
{
 	return (*(int *)item == *(int *)data_to_compare);
}

