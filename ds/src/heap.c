#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */
#include <string.h>	/* memcpy */

#include "heap.h"
#include "dvector.h" 	/* dynamic vector data structure and functions */


#define GetParent(index)		(((index)-1)/2)	/* Returns the parent node index */
#define GetLeft(index)			(2*(index))+1	/* Returns the left child node index */
#define GetRight(index)			(2*(index))+2 	/* Returns the right child node index */
#define PTR_SIZE				sizeof(void *)
#define INITIAL_DVECTOR_SIZE	10

static void HeapifyUp(heap_t *heap, size_t index_to_heapify);
static void HeapifyDown(heap_t *heap, size_t index_to_heapify);
static void HeapifyDownRecursiveHelper(vector_t *vector, size_t arr_size, size_t index_to_heapify, heap_compare_func_t cmp_fnc);
static void Swap(void *a, void *b);


struct heap
{
	vector_t *dvector;
	heap_compare_func_t compare_func;
};


heap_t *HeapCreate(heap_compare_func_t compare_func)
{
	heap_t *heap = (heap_t *)malloc(sizeof(heap_t));
	if (NULL == heap)
	{
		return NULL;
	}
	
	heap->dvector = VectorCreate(INITIAL_DVECTOR_SIZE, sizeof(void **));
	if (NULL == heap->dvector)
	{
		free(heap);
		return NULL;
	}
	
	heap->compare_func = compare_func;
	
	return heap;
}


void HeapDestroy(heap_t *heap)
{
	assert(heap);
	
	VectorDestroy(heap->dvector);
	free(heap);
}


int HeapPush(heap_t *heap, void *data)
{
	int push_status = 0;
	size_t new_element_index = 0;
	
	assert(heap);
	
	/* push the new element to the end of the dvector */
	push_status = VectorPushBack(heap->dvector, &data);
	if (0 != push_status)
	{
		return push_status;
	}
	
	/* get the index of the new element */
	new_element_index = VectorElementCount(heap->dvector) - 1;
	
	/* heapify the new element to its correct position in the heap */
	HeapifyUp(heap, new_element_index);
	
	return push_status;
}



void HeapPop(heap_t *heap)
{
	assert(heap);
	
	/* swap the "root" with the last element */
	Swap((void **)VectorAccessVal(heap->dvector, VectorElementCount(heap->dvector) - 1), (void **)VectorAccessVal(heap->dvector, 0));
	
	/* remove the old root from the dvector */
	VectorPopBack(heap->dvector);
	
	/* heapify down the new root, to its correct position */
	HeapifyDown(heap, 0);
}


void *HeapPeek(const heap_t *heap)
{
	void **data = NULL;
	
	assert(heap);
	
	data = (void **)VectorAccessVal(heap->dvector, 0);
	
	return *data;
}


size_t HeapSize(const heap_t *heap)
{
	assert(heap);
	
	return VectorElementCount(heap->dvector);
}


int HeapIsEmpty(const heap_t *heap)
{
	assert(heap);
	
	return (VectorElementCount(heap->dvector) == 0);
}



void *HeapRemove(heap_t *heap, heap_match_func_t match_func, void *param)
{
	void *data_runner = NULL;
	size_t count = 0;
	size_t i = 0;
	
	assert(heap);
	
	count = VectorElementCount(heap->dvector);
	
	for (i = 0; i < count; ++i)
	{
		data_runner = *(void **)(VectorAccessVal(heap->dvector, i));
		
		if (match_func(data_runner, param))
		{
			Swap(VectorAccessVal(heap->dvector, i), VectorAccessVal(heap->dvector, count - 1));
			VectorPopBack(heap->dvector);
			HeapifyDown(heap, i);
			
			return data_runner;
		}
	}

	return NULL;
}





/*********** Static helper functions ***********/


static void HeapifyUp(heap_t *heap, size_t index_to_heapify)
{
	vector_t *vector = heap->dvector;
	size_t parent_index = GetParent(index_to_heapify);
	void **current_data = (void **)VectorAccessVal(vector, index_to_heapify);
	void **parent_data = (void **)VectorAccessVal(vector, parent_index);
	heap_compare_func_t cmp_fnc = heap->compare_func;
	
	while (0 != index_to_heapify && cmp_fnc(*current_data, *parent_data) > 0)
	{
		/* swap the parent and child data */
		Swap((void **)current_data, (void **)parent_data);
		
		/* update runners - set current index as parent index, set parent as parent's parent */
		index_to_heapify = parent_index;
		current_data = (void **)VectorAccessVal(vector, index_to_heapify);
		
		parent_index = GetParent(parent_index);
		parent_data = (void **)VectorAccessVal(vector, parent_index);
	}
}




static void HeapifyDown(heap_t *heap, size_t index_to_heapify)
{
	HeapifyDownRecursiveHelper(heap->dvector, VectorElementCount(heap->dvector), index_to_heapify, heap->compare_func);
}



static void HeapifyDownRecursiveHelper(vector_t *vector, size_t arr_size, size_t index_to_heapify, heap_compare_func_t cmp_fnc)
{
	size_t index_to_swap = index_to_heapify;
	size_t left_index = GetLeft(index_to_heapify);
	size_t right_index = GetRight(index_to_heapify);
	
	void **left_data = NULL;
	void **right_data = NULL;
	
	/* if the left child index is valid, compare it to element to heapify */
	if (left_index < arr_size)
	{
		left_data = (void **)VectorAccessVal(vector, left_index);
		if (cmp_fnc(*(void **)VectorAccessVal(vector, index_to_swap), *left_data) < 0)
		{
			index_to_swap = left_index;
		}
	}
	
	/* if the right child index is valid, compare it to element to heapify */
	if (right_index < arr_size)
	{
		right_data = (void **)VectorAccessVal(vector, right_index);
		if (cmp_fnc(*(void **)VectorAccessVal(vector, index_to_swap), *right_data) < 0)
		{
			index_to_swap = right_index;
		}
	}
	
	/* swap if needed, and then call HeapifyDown recursively with swapped child index */
	if (index_to_swap != index_to_heapify)
	{
		Swap((void **)VectorAccessVal(vector, index_to_swap), (void **)VectorAccessVal(vector, index_to_heapify));
		HeapifyDownRecursiveHelper(vector, arr_size, index_to_swap, cmp_fnc);
	}
}



static void Swap(void* v1, void* v2) 
{ 
    char *buffer = NULL; 
	
	buffer = (char *)malloc(PTR_SIZE);
	
	if (!buffer)
	{
		return;
	}
	
    memcpy(buffer, v1, PTR_SIZE);
    memcpy(v1, v2, PTR_SIZE);
    memcpy(v2, buffer, PTR_SIZE);
    
	free(buffer);
} 
