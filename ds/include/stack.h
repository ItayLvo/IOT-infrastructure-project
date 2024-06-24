#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h>	/* size_t */

typedef struct stack stack_t;

/*
Description: Create stack
Params: size_t, size_t
Return Value: pointer to stack ds
Time Complexity: O(1)
Space: O(1)
*/
stack_t *StackCreate(size_t capacity, size_t type_size);
stack_t *StackCreateOneMalloc(size_t capacity, size_t type_size);
/*
Description: Destroy stack. doesn't 
Params: stack_t *
Return Value: exit status
Time Complexity: O(1)
Space: O(1)
*/
void StackDestroy(stack_t *stack);

/*
Description: remove the top item from the stack and return the item to the user
Params: stack_t *
Return Value: stack_t
Time Complexity: O(1)
Space: O(1)
*/
void StackPop(stack_t *stack);

/*
Description: Push item to the first location in stack
Params: stack_t, void *
Return Value: stack_t, void *
Time Complexity: O(1)
Space: O(1)
*/
void StackPush(stack_t *stack, const void *item);

/*
Description: return the first item in the stack (without removing it)
Params: stack_t
Return Value: void * to top element of stack
Time Complexity: O(1)
Space: O(1)
*/
void *StackPeek(const stack_t *stack);

/*
Description: return current amount of items in stack
Params: stack_t
Return Value: void
Time Complexity: O(1)
Space: O(1)
*/
size_t StackSize(const stack_t *stack);

/*
Description: check if stack is empty
Params: 
Return Value:  1 if empty, 0 if not empty
Time Complexity: O(1)
Space: O(1)
*/
int StackIsEmpty(const stack_t *stack);

/*
Description: return capacity of stack
Params: 
Return Value: size_t
Time Complexity: O(1)
Space: O(1)
*/
size_t StackCapacity(const stack_t *stack);


#endif /* __STACK_H__ */


