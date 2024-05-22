#include <stdio.h>	/* printf */

#include "dllist.h"
#include "sorted_list.h"

int MatchInt(const void *item, const void *data_to_compare);
int CompareInt(const void *item, const void *data_to_compare);

static void SListPrintList(slist_t *list);
static int PrintListHelper(void *data, void *dummy);

dll_t *SListGetDLL(slist_t *list);
int TestMergeSortedLists(void);
int TestInsertRemoveSortedList(void);
int TestPopSortedList(void);
int TestFindIfSortedList(void);

int main()
{
	int success_status = 0;
	
	success_status += TestInsertRemoveSortedList();
	success_status += TestPopSortedList();
	success_status += TestFindIfSortedList();
	success_status += TestMergeSortedLists();
	
	printf("\nSummary:\nRan 4 tests. failed in %d tests.\n",
		success_status);
		
	return success_status;
}

int TestPopSortedList(void)
{
	slist_t *list = SListCreate(CompareInt);
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4;
	size_t list_size = 0;
	
	printf("---Testing Pop: \n");
	printf("Pushing 2->4->3->1 \n");
	SListInsert(list, &x2);
	SListInsert(list, &x4);
	SListInsert(list, &x3);
	SListInsert(list, &x1);
	
	printf("Popping front, popping back: \n");
	SListPopFront(list);
	SListPopBack(list);
	list_size = SListCount(list);
	printf("List count = %ld\n", list_size);
	SListPrintList(list);
	
	printf("---------------\n\n");
	SListDestroy(list);
	
	return (list_size == 2) ? 0 : 1;

}

int TestInsertRemoveSortedList(void)
{
	slist_t *list = SListCreate(CompareInt);
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4;
	slist_iterator_t iterator;
	
	printf("---Testing Insert, Remove: \n");
	
	if (SListIsEmpty(list) != 1)
	{
		printf("SListIsEmpty failed. Expected 1, result is %d\n",
			SListIsEmpty(list));
		return 1;	/* SListIsEmpty failed */
	}
	
	if (SListCount(list)!= 0)
	{
		printf("SListCount failed. Expected 0, result is %ld\n",
			SListCount(list));
		return 1;	/* SListCount failed */
	}
	
	
	printf("Inserting 3->1->4->2 \n");
	SListInsert(list,  &x3);
	SListInsert(list,  &x1);
	SListInsert(list,  &x4);
	iterator = SListInsert(list,  &x2);
	if (SListIsEqualIter(iterator, SListEnd(list)))
	{
		return 1;	/* insert failed */
	}
	SListPrintList(list);
	
	printf("Removing iterator. \n");
	SListRemove(iterator);
	SListPrintList(list);
	if (SListCount(list)!= 3)
	{
		printf("SListCount or Remove failed. Expected 3, result is %ld\n",
			SListCount(list));
		return 1;	/* SListCount or Remove failed */
	}
	
	SListDestroy(list);
	printf("---------------\n\n");
	
	return 0;
}


int TestFindIfSortedList(void)
{
	slist_t *list = SListCreate(CompareInt);
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4;
	slist_iterator_t iterator;
	
	printf("---Testing Find:\n");
	
	printf("Inserting 4->2->1->3 \n");
	SListInsert(list,  &x4);
	SListInsert(list,  &x2);
	SListInsert(list,  &x1);
	SListInsert(list,  &x3);
	SListPrintList(list);
	
	printf("Finding iterator where data == 2.\n");
	iterator = SListFindIf(SListBegin(list), SListEnd(list), &x2, MatchInt);
	if (*(int *)SListGetData(iterator) != 2)
	{
		printf("Find or GetData failed. Expected 2, result is %d.\n",
			*(int *)SListGetData(iterator));
		return 1;
	}

	SListDestroy(list);
	printf("---------------\n\n");
	
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
