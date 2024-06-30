#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */

#include <stdio.h>	/*printf - remove me............ */

#include "heap.h"
#include "dvector.h" 	/* dynamic vector data structure and functions */


#define GetParent(index)	(((index)-1)/2)	/* Returns the parent node */
#define GetLeft(index)		(2*(index))+1	/* Returns the left child node */
#define GetRight(index)		(2*(index))+2 	/* Returns the right child node */


static void HeapifyUp(heap_t *heap, size_t index_to_heapify);
static void HeapifyDown(heap_t *heap, size_t index_to_heapify);
static void HeapifyDownRecursiveHelper(vector_t *vector, size_t arr_size, size_t index_to_heapify, heap_compare_func_t cmp_fnc);
static void Swap(void **a, void **b);


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
	
	heap->dvector = VectorCreate(1, sizeof(void *));
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
	push_status = VectorPushBack(heap->dvector, data);
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
	void *last_element_data = NULL;
	void *root_data = NULL;
	
	assert(heap);
	
	last_element_data = VectorAccessVal(heap->dvector, VectorElementCount(heap->dvector) - 1);
	root_data = VectorAccessVal(heap->dvector, 0);
	
	/* swap the "root" with the last element */
	Swap(last_element_data, root_data);
	
	/* remove the old root from the dvector */
	VectorPopBack(heap->dvector);
	
	/* heapify down the new root, to its correct position */
	HeapifyDown(heap, 0);
}


void *HeapPeek(const heap_t *heap)
{
	void *data = NULL;
	
	assert(heap);
	
	data = VectorAccessVal(heap->dvector, 0);
	
	return data;
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
	void *data_to_return = NULL;
	void *last_element_data = NULL;
	void *data_runner = NULL;
	size_t count = 0;
	size_t i = 0;
	
	assert(heap);
	
	count = VectorElementCount(heap->dvector);
	
	for (i = 0; i < count; ++i)
	{
		data_runner = (VectorAccessVal(heap->dvector, i));
		
		if (match_func(data_runner, param))
		{
			last_element_data = VectorAccessVal(heap->dvector, count - 1);
			Swap(data_runner, last_element_data);
			data_to_return = last_element_data;
			VectorPopBack(heap->dvector);
			HeapifyDown(heap, i);
			
			break;
		}
	}
	

	return data_to_return;
}





/*********** Static helper functions ***********/


static void HeapifyUp(heap_t *heap, size_t index_to_heapify)
{
	vector_t *vector = heap->dvector;
	size_t parent_index = GetParent(index_to_heapify);
	void *current_data = VectorAccessVal(vector, index_to_heapify);
	void *parent_data = VectorAccessVal(vector, parent_index);
	heap_compare_func_t cmp_fnc = heap->compare_func;
	
	while (0 != index_to_heapify && cmp_fnc(current_data, parent_data) < 0)
	{
		/* swap the parent and child data */
		Swap(current_data, parent_data);
		
		/* update runners - set current index as parent index, set parent as parent's parent */
		index_to_heapify = parent_index;
		current_data = VectorAccessVal(vector, index_to_heapify);
		
		parent_index = GetParent(parent_index);
		parent_data = VectorAccessVal(vector, parent_index);
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
	
	void *left_data = NULL;
	void *right_data = NULL;
	
	/* if the left child index is valid, compare it to element to heapify */
	if (left_index < arr_size)
	{
		left_data = VectorAccessVal(vector, left_index);
		if (cmp_fnc(VectorAccessVal(vector, index_to_swap), left_data) > 0)
		{
			index_to_swap = left_index;
		}
	}
	
	/* if the right child index is valid, compare it to element to heapify */
	if (right_index < arr_size)
	{
		right_data = VectorAccessVal(vector, right_index);
		if (cmp_fnc(VectorAccessVal(vector, index_to_swap), right_data) > 0)
		{
			index_to_swap = right_index;
		}
	}
	
	/* swap if needed, and then call HeapifyDown recursively with swapped child index */
	if (index_to_swap != index_to_heapify)
	{
		Swap(VectorAccessVal(vector, index_to_swap), VectorAccessVal(vector, index_to_heapify));
		HeapifyDownRecursiveHelper(vector, arr_size, index_to_swap, cmp_fnc);
	}
}


static void Swap(void **a, void **b)
{
    void *temp = *a;
    *a = *b;
    *b = temp;
}
