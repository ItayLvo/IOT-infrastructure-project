#include <stddef.h>	/* size_t */
#include <string.h>	/* memcpy */
#include <stdlib.h>	/* malloc */

#include "../include/stack.h"

struct stack
{
    void *buffer;
    size_t capacity;
    size_t type_size;
    size_t current_size;
};


stack_t *StackCreate(size_t capacity, size_t type_size)
{
	stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
	if (NULL == stack)
	{
		return NULL;
	}
	
	stack->buffer = (void *)malloc(capacity * type_size);
	stack->current_size = 0;
	stack->type_size = type_size;
	stack->capacity = capacity;
	
	return stack;
}


void StackDestroy(stack_t *stack)
{
	free(stack->buffer);
	free(stack);
}


void StackPop(stack_t *stack)
{
	if (0 == stack->current_size)
	{
		return;
	}
	
	--(stack->current_size);
}

void StackPush(stack_t *stack, const void *item)
{
	void *ptr = (char *)(stack->buffer) + (stack->current_size * stack->type_size);
	ptr = memcpy(ptr, item, stack->type_size);
	++(stack->current_size);
}

void StackPeek(const stack_t *stack, void *dest)
{
	char *ptr = NULL;

	if (0 == stack->current_size)
	{
		dest = NULL;
		return;
	}
	
	ptr = (char *)(stack->buffer) + ((stack->current_size - 1) * stack->type_size);
	dest = memcpy(dest, ptr, stack->type_size);
}

size_t StackSize(const stack_t *stack)
{
	return stack->current_size;
}

int StackIsEmpty(const stack_t *stack)
{
	return (stack->current_size == 0);
}

size_t StackCapacity(const stack_t *stack)
{
	return (stack->capacity);
}
