#include <stdio.h>	/* printf */
#include <limits.h>	/* CHAR_BIT */
#include <stdlib.h>	/* malloc */

#include "../include/stack.h"




int main()
{
	
	stack_t *stack_ptr;
	int x = 5;
	int *xp = &x;
	int *test_ptr = NULL;
	
	stack_ptr = StackCreate(50, sizeof(int));
	printf("%ld, %ld\n", StackSize(stack_ptr), StackCapacity(stack_ptr));
	
	StackPush(stack_ptr, xp);
	printf("%ld, %ld\n", StackSize(stack_ptr), StackCapacity(stack_ptr));
	
	StackPeek(stack_ptr, test_ptr);
	printf("%d\n", *test_ptr);
	
	return 0;
}

