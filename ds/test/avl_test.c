#include <stdio.h>	/* printf */
#include <stdlib.h>	/* free */
#include "avl.h"

static int CompareInt(const void *item, const void *data_to_compare);
static int PrintNodes(void *data, void *params);


int main()
{
	
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 =5, x6 = 6, x7 = 7, x8 = 8, x10 = 10, x15 = 15, x11 = 11, x12 = 12, x13 = 13;
	int status = 0;
	
	avl_t *avl = AVLCreate(CompareInt);
	printf("is empty? %d\n", AVLIsEmpty(avl));
	printf("tree size = %lu\n", AVLCount(avl));
	
	status = AVLInsert(avl, &x5);
	status = AVLInsert(avl, &x3);
	status = AVLInsert(avl, &x4);
	status = AVLInsert(avl, &x7);
	status = AVLInsert(avl, &x8);
	status = AVLInsert(avl, &x6);
	status = AVLInsert(avl, &x2);
	status = AVLInsert(avl, &x1);
	
	printf("is empty? %d\n", AVLIsEmpty(avl));
	printf("tree size = %lu\n", AVLCount(avl));
	printf("searching: %d\n", *(int *)AVLFind(avl, &x4));
	printf("root height: %lu\n\n", AVLHeight(avl));
	
	
	AVLForEach(avl, PrintNodes, NULL);
	printf("\n");
	
	
	AVLRemove(avl, &x8);
	AVLRemove(avl, &x1);
	AVLRemove(avl, &x5);
	/*
	AVLRemove(avl, &x4);
	AVLRemove(avl, &x7);
	AVLRemove(avl, &x6);
	AVLRemove(avl, &x2);
	*/
	AVLRemove(avl, &x3);
	

	AVLDestroy(avl);
	
	/*
	
	free(avl);
	printf("tree size = %lu\n", AVLCount(avl));
	printf("root height: %lu\n", AVLHeight(avl));
	AVLRemove(avl, &x7);
	AVLRemove(avl, &x6);
	status = AVLInsert(avl, &x1);
	*/
	return 0;
}

/*
p *(int *)tree->root->
p node_to_remove->height
p AVLCount(avl)
p AVLHeight(avl)
*/


static int CompareInt(const void *item, const void *data_to_compare)
{
 	return (*(int *)item - *(int *)data_to_compare);
}




static int PrintNodes(void *data, void *params)
{
	int *num_ptr = (int *)data;
	(void)params;
	
	printf("%d, ", *num_ptr);
	
	return 0;
}

