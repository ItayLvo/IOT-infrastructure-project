#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/singly_linked_list.h"

int main()
{
	linked_list_t *list = SLListCreate();
	iterator_t iterator = SLListGetBegin(list);
	int x1 = 5, x2 = 4, x3 = 3, x4 = 2;
	
	iterator = SLListInsert(list, &x1, iterator);
	iterator = SLListInsert(list, &x2, iterator);
	
	iterator = SLListInsert(list, &x3, iterator);
	iterator = SLListInsert(list, &x4, iterator);
	
	
	printf("%d\n", *(int *)SLListGetData(SLListGetBegin(list)));
	
	
	
	
	SLListDestroy(list);
	return 0;
}

