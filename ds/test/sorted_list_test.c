#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/dllist.h"
#include "../include/sorted_list.h"

int MatchInt(const void *item, const void *data_to_compare);
int CompareInt(const void *item, const void *data_to_compare);

static void SListPrintList(slist_t *list);
static int PrintListHelper(void *data, void *dummy);

dll_t *SListGetDLL(slist_t *list);
int TestMergeSortedLists(void);
int TestInsertRemoveSortedList(void);
int TestPopSortedList(void);

int main()
{
	int success_status = 0;
	
	success_status += TestInsertRemoveSortedList();
	success_status += TestPopSortedList();
	success_status += TestMergeSortedLists();

	return success_status;
}

int TestPopSortedList(void)
{
	slist_t *list = SListCreate(CompareInt);
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4;
	
	printf("---Testing Pop: \n");
	printf("Pushing 2->4->3->1 \n");
	SListInsert(list, &x2);
	SListInsert(list, &x4);
	SListInsert(list, &x3);
	SListInsert(list, &x1);
	
	printf("Popping front, popping back: \n");
	SListPopFront(list);
	SListPopBack(list);
	printf("List count = %ld\n", SListCount(list));
	SListPrintList(list);
	
	printf("---------------\n");
	SListDestroy(list);
	
	return 0;
}

int TestInsertRemoveSortedList(void)
{
	slist_t *list = SListCreate(CompareInt);
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4;
	slist_iterator_t iterator;
	
	printf("---Testing Insert, Remove: \n");
	printf("IsEmpty? %d\n", SListIsEmpty(list));
	printf("List count = %ld\n\n", SListCount(list));
	
	printf("Pushing 3->1->4->2 \n");
	SListInsert(list,  &x3);
	iterator = SListInsert(list,  &x1);
	SListInsert(list,  &x4);
	SListInsert(list,  &x2);
	
	printf("IsEmpty? %d\n\n", SListIsEmpty(list));
	printf("List count = %ld\n\n", SListCount(list));
	
	SListPrintList(list);
	
	
	iterator = SListFindIf(SListBegin(list), SListEnd(list), &x2, MatchInt);
	SListRemove(iterator);
	printf("\niterator after remove(iterator): %d\n", *(int *)SListGetData(iterator));
	printf("List count = %ld\n\n", SListCount(list));
	
	SListDestroy(list);
	printf("---------------\n");
	
	return 0;
}

int TestMergeSortedLists(void)
{
	slist_t *list1 = SListCreate(CompareInt);
	slist_t *list2 = SListCreate(CompareInt);
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 = 5, x6 = 6, x7 = 7, x8 = 8, x9 = 9;
	
	printf("---Testing Merge: \n");
	SListInsert(list1,  &x1);
	SListInsert(list1,  &x3);
	SListInsert(list1,  &x7);
	SListInsert(list1,  &x4);
	SListInsert(list1,  &x9);
	
	SListInsert(list2,  &x2);
	SListInsert(list2,  &x2);
	SListInsert(list2,  &x5);
	SListInsert(list2,  &x1);
	SListInsert(list2,  &x6);
	SListInsert(list2,  &x8);
	
	SListPrintList(list1);
	SListPrintList(list2);
	
	printf("List1 count = %ld\n", SListCount(list1));
	printf("List2 count = %ld\n", SListCount(list2));
	
	list1 = SDLListMerge(list1, list2);
	printf("\nafter merge: \n");
	SListPrintList(list1);
	
	printf("List1 count = %ld\n", SListCount(list1));
	printf("List2 count = %ld\n", SListCount(list2));
	
	SListDestroy(list1);
	SListDestroy(list2);
	printf("---------------\n");
	
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



static void SListPrintList(slist_t *list)
{
	dll_t *d_list = SListGetDLL(list);
	printf("Printing list: \n");
	DLListForeach(DLListBegin(d_list), DLListEnd(d_list), NULL, PrintListHelper);
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
