#include <stdio.h>	/* printf */
#include <stdlib.h>	/* malloc */

#include "../include/stack.h"


int main()
{
	
	stack_t *stack_ptr;
	int x = 1;
	int *xp = &x;
	int *test_ptr = (int *)malloc(sizeof(int) * 1);
	
	stack_ptr = StackCreate(50, sizeof(int));
	printf("stack size = %ld, stack capacity = %ld\n", StackSize(stack_ptr), StackCapacity(stack_ptr));
	printf("pushed 1\n");
	StackPush(stack_ptr, xp);
	printf("stack size = %ld, stack capacity = %ld\n", StackSize(stack_ptr), StackCapacity(stack_ptr));
	
	x = 2;
	StackPush(stack_ptr, xp);
	printf("pushed 2\n");
	x = 3;
	StackPush(stack_ptr, xp);
	printf("pushed 3\n");
	StackPeek(stack_ptr, test_ptr);
	printf("peek: %d\n", *test_ptr);
	StackPop(stack_ptr);
	printf("popped\n");
	StackPeek(stack_ptr, test_ptr);
	printf("peek: %d\n", *test_ptr);
	printf("stack size = %ld, stack capacity = %ld\n", StackSize(stack_ptr), StackCapacity(stack_ptr));
	
	StackDestroy(stack_ptr);
	free(test_ptr);
	
	return 0;
}

