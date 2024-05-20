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
	dll_t *list2 = DLListCreate();
	dll_iterator_t iterator = NULL;
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4;
	int count_multi = 0;
	
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
	DLListRemove(iterator);
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
	
	DLLPrintList(list);
	iterator = DLListFind(DLListBegin(list), DLListEnd(list), &x2, MatchInt);
	printf("searched for 2 -> %d\n", *(int *)DLListGetData(iterator));
	
	
	DLListPushBack(list, &x2);
	count_multi = DLListMultiFind(list2 ,DLListBegin(list), DLListEnd(list), &x2, MatchInt);
	printf("found %d times\n", count_multi);
	DLLPrintList(list2);
	
	printf("\n\n\n");
	DLLPrintList(list);
	DLLPrintList(list2);
	printf("\n\n");
	
	iterator = DLListSplice(DLListBegin(list),
				DLListEnd(list), 
				DLListBegin(list2));
	
	
	DLLPrintList(list);
	DLLPrintList(list2);
	
	
	DLListDestroy(list);
	DLListDestroy(list2);
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
