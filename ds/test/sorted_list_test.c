#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/dllist.h"
#include "../include/sorted_list.h"

int MatchInt(const void *item, const void *data_to_compare);
int CompareInt(const void *item, const void *data_to_compare);

int main()
{
	slist_t *list = SListCreate(CompareInt);
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
