#include <stdio.h>	/*printf*/
#include "sllist.h"

iterator_t IterativeFind(iterator_t head, int n);

/* iterator_t is of type node pointer */
int main(void)
{
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4;
	iterator_t iterator = NULL;
	linked_list_t *sllist = SLListCreate();
	
	SLListInsert(sllist, &x1, SLListGetBegin(sllist));
	SLListInsert(sllist, &x2, SLListGetBegin(sllist));
	SLListInsert(sllist, &x3, SLListGetBegin(sllist));
	SLListInsert(sllist, &x4, SLListGetBegin(sllist));
	
	iterator = IterativeFind(SLListGetBegin(sllist), 2);
	printf("%d\n", *(int *)SLListGetData(iter));
	
	iterator = RecursiveFind(SLListGetBegin(sllist), 2);
	printf("%d\n", *(int *)SLListGetData(iter));

	return 0;
}



iterator_t IterativeFind(iterator_t head, int n)
{
	int i = 0, count = 0;
	iterator_t runner = head;
	iterator_t result_runner = head;
	
	while (runner != NULL)
	{
		if (count >= n)
		{
			result_runner = result_runner->next;
		}
		
		runner = runner->next;
		++count;
	}
	
	return result_runner;
}



iterator_t RecursiveFind(iterator_t head, int n)
{
	static int s_count = 0;
	
	if (head == NULL)
	{
		return;
	}
	
	RecursiveFind(head->next, n);
	
	++s_count;
	if (s_count == n)
	{
		return head;
	}
}







