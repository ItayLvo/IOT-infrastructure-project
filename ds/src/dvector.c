#include <stddef.h>	/* size_t */
#include <string.h>	/* memcpy */
#include <stdlib.h>	/* malloc */
#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */

#include "dvector.h"	/* dvector functions */

/* GROWTH_FACTOR and SHRINK_FACTOR macros defined in header file as 2 */

struct vector
{
	size_t capacity;
	size_t element_size;
	size_t element_count;
	void *buffer;
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
	void *ptr = NULL;
	assert(vector);
	
	ptr = (char *)(vector->buffer) + (index * vector->element_size);
	
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
	void *ptr_write_location = NULL;
	
	if (vector->element_count == vector->capacity)
	{
		vector->buffer = realloc(vector->buffer,
				vector->element_size * vector->capacity *
				GROWTH_FACTOR);
			
		if (NULL == vector->buffer)
		{
			return 1;
		}
		vector->capacity = vector->capacity * GROWTH_FACTOR;
	}
	
	ptr_write_location = (char *)(vector->buffer) +
		(vector->element_count * vector->element_size);
		
	ptr_write_location = memcpy(ptr_write_location,
		new_element_data, vector->element_size);
	
	++(vector->element_count);
	
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
		vector->buffer = realloc(vector->buffer,
			vector->element_size *
			(vector->capacity / SHRINK_FACTOR));
			
		vector->capacity = vector->capacity / SHRINK_FACTOR;
	}
	
	--(vector->element_count);
}


void VectorShrink(vector_t *vector)
{
	vector->buffer = realloc(vector->buffer,
		vector->element_count * vector->element_size);
		
	vector->capacity = vector->element_count;
}


int VectorReserve(vector_t *vector, size_t n_new_elements)
{
	vector->buffer = realloc(vector->buffer,
		(vector->element_size *
		(vector->capacity + n_new_elements)));
		
	if (NULL == vector->buffer)
	{
		return 1;
	}
	
	vector->capacity += n_new_elements;
	
	return 0;
}
