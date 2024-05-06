#include <stddef.h>	/* size_t */
#include <string.h>	/* memcpy */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "../include/vector.h"

struct vector
{
	size_t capacity;
	size_t element_size;
	size_t element_count
	void *buffer 
};


vector_t *VectorCreate(size_t capacity, size_t element_size)
{
	vector_t *vector = (vector_t *)malloc(sizeof(vector_t));
	if (NULL == vector)
	{
		return NULL;
	}
	
	vector->buffer = (void *)malloc(capacity * element_size);
	vector->element_count = 0;
	vector->element_size = element_size;
	vector->capacity = capacity;
	
	return vector;
}


void VectorDestroy(stack_t *stack)
{
	free(vector->buffer);
	free(vector);
}


void *VectorAccessVal(vector_t *vector, size_t index)
{
	void *ptr = (char *)(vector->buffer) + (index * vector->element_size);
	
	return ptr;
}

size_t VectorElementCount(const vector_t *vector)
{
	return vector->element_count;
}


size_t VectorCapacity(const vector_t *vector)
{
	return vector->capacity;
}


int VectorPushBack(vector_t *vector, const void *new_element_data)
{
	void *ptr = NULL;
	
	if (vector->element_count == vector->capacity)
	{
		/*reserve...*/
	}
	
	ptr = (char *)(vector->buffer) + (vector->element_count * vector->element_size);
	ptr = memcpy(ptr, new_element_data, stack->element_size);
	++(stack->element_count);
	
	return 0;
}



void VectorPopBack(vector_t *vector)
{
	assert(vector);
	
	if (0 == stack->current_size)
	{
		return;
	}
	
	if ( /*low enough to shrink...*/ )
	{
		/*shrink...*/
	}
	
	--(stack->element_count);
}


void VectorShrink(vector_t *vector)
{
	void *ptr = vector->buffer;
	
	ptr = realloc(ptr, vector->element_count);
}


void StackPop(stack_t *stack)
{
	assert(stack);
	if (0 == stack->current_size)
	{
		return;
	}
	
	--(stack->current_size);
}


int VectorReserve(vector_t *vector, size_t n_new_elements)
{
	void *ptr = vector->buffer;
	
	ptr = realloc(ptr, (vector->element_size * (vector->element_count + n_new_elements));
	if (NULL == ptr)
	{
		return 1;
	}
	
	return 0;
}

/*
void StackPush(stack_t *stack, const void *item)
{
	
	void *ptr = NULL;
	assert(stack);
	
	ptr = (char *)(stack->buffer) + (stack->current_size * stack->type_size);
	
	if (stack->current_size == stack->capacity)
	{
		printf("Stack is full\n");
		return;
	}

	
	ptr = memcpy(ptr, item, stack->type_size);
	++(stack->current_size);
}

void StackPeek(const stack_t *stack, void *dest)
{
	char *ptr = NULL;
	assert(stack);
	
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
	assert(stack);
	return stack->current_size;
}

int StackIsEmpty(const stack_t *stack)
{
	assert(stack);
	return (stack->current_size == 0);
}

size_t StackCapacity(const stack_t *stack)
{
	assert(stack);
	return (stack->capacity);
}

*/
