#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/dllist.h"

/*
static int TestListFunctions(linked_list_t *list);
*/

static int MatchInt(const void *item, const void *data_to_compare);
static void DLLPrintList(dll_t *list);
static int PrintListHelper(void *data, void *dummy);

int main()
{
	dll_t *list = DLListCreate();
	
	dll_iterator_t iterator = NULL;
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4;
	
	printf("is the list empty (1 for yes, 0 for no)? : %d\n\n", DLListIsEmpty(list));
	printf("list size = %ld\n\n",DLListCount(list));
	printf("pushing 1->2->3\n\n");
	
	iterator = DLListBegin(list);
	iterator = DLListInsert(list, iterator, &x3);
	iterator = DLListInsert(list, iterator, &x2);
	iterator = DLListInsert(list, iterator, &x1);
	
	DLLPrintList(list);
	
	printf("list size = %ld\n",DLListCount(list));
	
	printf("is the list empty (0 for no)? : %d\n\n", DLListIsEmpty(list));

	printf("head = %d\n", *(int *)DLListGetData(DLListBegin(list)));
	printf("head->next = %d\n", *(int *)DLListGetData(DLListNext(DLListBegin(list))));
	printf("tail->prev = %d\n", *(int *)DLListGetData(DLListPrev(DLListEnd(list))));
	
	printf("iterator = %d\n", *(int *)DLListGetData(iterator));
	DLListRemove(list, iterator);
	printf("data after remove() = %d\n", *(int *)DLListGetData(iterator));

	DLListPopFront(list);
	printf("head after popfront = %d\n", *(int *)DLListGetData(DLListBegin(list)));
	
	DLLPrintList(list);
	
	DLListSetData(iterator, &x4);
	printf("data after setdata(4) = %d\n", *(int *)DLListGetData(iterator));

	DLLPrintList(list);

	DLListPushBack(list, &x1);
	DLListPushBack(list, &x2);
	
	DLLPrintList(list);
	
	DLListPopBack(list);
	DLLPrintList(list);
	
	DLListPopFront(list);
	DLLPrintList(list);
	
	DLListPushBack(list, &x2);
	DLListPushBack(list, &x3);
	
	iterator = DLListFind(DLListBegin(list), DLListEnd(list), &x2, MatchInt);
	printf("searched for 2 -> %d\n", *(int *)DLListGetData(iterator));
	
	DLListDestroy(list);
	return 0;
}




static void DLLPrintList(dll_t *list)
{
	printf("Printing list: \n");
	DLListForeach(DLListBegin(list), DLListEnd(list), NULL, PrintListHelper);
	printf("\n");
}

static int PrintListHelper(void *data, void *dummy)
{
	(void)dummy;
	if (data != NULL)
	{
		printf("[%d] -> ", *(int *)data);
		
	}

	return 0;
}

int MatchInt(const void *item, const void *data_to_compare)
{
 	return (*(int *)item == *(int *)data_to_compare);
}

/*
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
	SLListRemove(iterator);
	SLLPrintList(list);
	printf("\n");
	
	printf("list size = %ld\n",SLListCount(list));
	
	printf("is the list empty (0 for no)? : %d\n", SLListIsEmpty(list));
	
	printf("removing node\n");
	SLListRemove(SLListGetBegin(list));
	printf("removing node\n");
	SLListRemove(SLListGetBegin(list));
	printf("removing node\n");
	SLListRemove(SLListGetBegin(list));
	printf("removing node\n");
	SLListRemove(SLListGetBegin(list));
	
	printf("list size = %ld\n",SLListCount(list));
	printf("is the list empty (0 for no)? : %d\n", SLListIsEmpty(list));
	
	return return_status;
}








*/
