#include <stddef.h>	/* size_t */
#include <string.h>	/* memcpy */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "../include/vector.h"

#define SHRINK_FACTOR 2
#define GROWTH_FACTOR 2

struct vector
{
	size_t capacity;
	size_t element_size;
	size_t element_count;
	void *buffer ;
};


vector_t *VectorCreate(size_t capacity, size_t element_size)
{
	vector_t *vector = (vector_t *)malloc(sizeof(vector_t));
	if (NULL == vector)
	{
		return NULL;
	}
	
	vector->buffer = (void *)malloc(capacity * element_size);
	if (NULL == vector->buffer)
	{
		return NULL;
	}
	
	vector->element_count = 0;
	vector->element_size = element_size;
	vector->capacity = capacity;
	
	return vector;
}


void VectorDestroy(vector_t *vector)
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
		vector->buffer = realloc(vector->buffer, vector->element_size * vector->capacity * GROWTH_FACTOR);
		if (NULL == vector->buffer)
		{
			return 1;
		}
		vector->capacity = vector->capacity * GROWTH_FACTOR;
	}
	
	ptr = (char *)(vector->buffer) + (vector->element_count * vector->element_size);
	ptr = memcpy(ptr, new_element_data, vector->element_size);
	
	++(vector->element_count);
	
	printf("pushed\n");
	
	return 0;
}



void VectorPopBack(vector_t *vector)
{
	assert(vector);
	
	if (0 == vector->element_count)
	{
		return;
	}
	
	if ((vector->capacity / SHRINK_FACTOR) >= vector->element_count)
	{
		vector = realloc(vector,
			vector->element_size * (vector->capacity / SHRINK_FACTOR));
			
		vector->capacity = vector->capacity / SHRINK_FACTOR;
	}
	
	--(vector->element_count);
	
	printf("popped\n");
}


void VectorShrink(vector_t *vector)
{
	void *ptr = vector->buffer;
	
	ptr = realloc(ptr, vector->element_count * vector->element_size);
}


int VectorReserve(vector_t *vector, size_t n_new_elements)
{
	void *ptr = vector->buffer;
	
	ptr = realloc(ptr, (vector->element_size * (vector->capacity + n_new_elements)));
	if (NULL == ptr)
	{
		return 1;
	}
	
	vector->capacity += n_new_elements;
	
	return 0;
}
