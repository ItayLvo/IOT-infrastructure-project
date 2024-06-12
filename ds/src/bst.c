/*
date:
reviewer:
status:
*/



/* dummy in the beginning that everything is to its' left
prev and next - search parent until the node is the left/right son of its' parent.
also need to save parent pointer in every node
how do i make sure the prev/next works with dummy?
reuse with helper func (next, prev) 
use getleft, getrigth, gethead funcs*/


#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */

#include "bst.h"	/* bst_t typedef, bst_t functions */

static bst_iterator_t BSTCreateIterator(void *data);

struct binary_search_tree_t
{
    struct bst_node_t *root;
    bst_compare_func_t compare_func;
}


struct bst_node_t
{
    void *data;
    struct bst_node_t *left;
    struct bst_node_t *right;
    struct bst_node_t *parent;
}



static bst_iterator_t BSTCreateIterator(void *data)
{
	bst_node_t *node = malloc(sizeof(bst_node_t));
	if (NULL == node)
	{
		return NULL;	/* TODO return invalid iterator */
	}
	
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	
	return node;
}

bst_t *BSTCreate(bst_compare_func_t cmp_func)
{
	bst_t *bst = (bst_t *)malloc(sizeof(bst_t));
	if (NULL == bst)
	{
		return NULL;
	}
	
	bst->root = BSTCreateIterator(NULL);
	
	bst->compare_func = cmp_func;
	
	return bst;
}


void BSTDestroy(bst_t *tree)
{
	assert(tree);
	
	/* implement in-order loop to free - foreach maybe? */
	
	free(tree->root);
	free(tree);
}


bst_iterator_t BSTInsert(bst_t *tree, void *data)
{
	bst_iterator_t new_node = BSTCreateIterator(data);
 
	bst_iterator_t runner = tree->root->left;
 
	bst_iterator_t prev = NULL /* TODO invalid iter */
 
	while (NULL != runner) 
	{
		prev = runner;

		if (data < runner->data)
		{
			runner = runner->left;
		}
		else
		{
			runner = runner->right;
		}
	}
 
	if (NULL == prev)
	{
		prev = newnode;
 	}
 	
	else if (data < prev->data)
	{
		prev->left = newnode;
 	}

	else
	{
		prev->right = newnode;
	}


	return prev;
}


slist_iterator_t SListRemove(slist_iterator_t iter)
{
	assert(NULL != iter.iter);
	iter.iter = DLListRemove(iter.iter);
	
	return iter;
}


void *SListPopFront(slist_t *list)
{	
	assert(NULL != list);
	return (DLListPopFront(list->d_list));
}


void *SListPopBack(slist_t *list)
{
	assert(NULL != list);
	return (DLListPopBack(list->d_list));
}


int SListIsEqualIter(slist_iterator_t it1, slist_iterator_t it2)
{
	#ifndef NDBUG
	if (it1.sorted_list != it2.sorted_list)
	{
		return 1;
	}
	#endif
	
	return (DLListIsEqualIter(it1.iter, it2.iter));
}


slist_iterator_t SListBegin(const slist_t* list)
{
	slist_iterator_t iterator;
	assert(list);
	
	iterator.iter = DLListBegin(list->d_list);
	
	#ifndef NDEBUG
	iterator.sorted_list = (slist_t*)list;;
	#endif /* NDEBUG */
	
	return iterator;
}


slist_iterator_t SListEnd(const slist_t* list)
{
	slist_iterator_t iterator;
	assert(list);
	
	iterator.iter = DLListEnd(list->d_list);
	
	#ifndef NDEBUG
	iterator.sorted_list = (slist_t*)list;
	#endif /* NDEBUG */
	
	return iterator;
}


slist_iterator_t SDLListForeach(slist_iterator_t start_iterator, slist_iterator_t end_iterator, void* data, slist_action_func_t func)
{	
	assert(start_iterator.sorted_list == end_iterator.sorted_list);
	
	start_iterator.iter = DLListForeach(start_iterator.iter, end_iterator.iter, data, func);
	
	return start_iterator;
}


size_t SListCount(const slist_t *list)
{
	return (DLListCount(list->d_list));
}


void *SListGetData(slist_iterator_t iter)
{
	assert(NULL != iter.iter);
	return DLListGetData(iter.iter);
}


slist_iterator_t SListNext(slist_iterator_t iter)
{
	assert(NULL != iter.iter);
	iter.iter = DLListNext(iter.iter);
	return iter;
}

slist_iterator_t SListPrev(slist_iterator_t iter)
{
	assert(NULL != iter.iter);
	iter.iter = DLListPrev(iter.iter);
	return iter;
}


/* uses compare_func to find the first element in the range that is larger than 'data' argument */
slist_iterator_t SListFind(slist_t *list, slist_iterator_t start_iter,
                           slist_iterator_t end_iter, void* data)
{
	slist_compare_func_t compare_func = list->compare_func;
	
	assert(start_iter.sorted_list == end_iter.sorted_list);
	
	while (!SListIsEqualIter(start_iter, end_iter))
	{
		if (compare_func(SListGetData(start_iter), data) > 0)
		{
			return start_iter;
		}
		
		start_iter = SListNext(start_iter);
	}
	

	return start_iter;
}


slist_iterator_t SListFindIf(slist_iterator_t start_iter, slist_iterator_t end_iter, void* data, slist_match_func_t match_func)
{
	start_iter.iter = DLListFind(start_iter.iter, end_iter.iter, data, match_func);
	
	return start_iter;
}


int SListIsEmpty(const slist_t *list)
{
	assert(NULL != list);
	return DLListIsEmpty(list->d_list);
}


slist_t* SDLListMerge(slist_t *dest_list, slist_t *src_list)
{
	slist_iterator_t src_runner_segment_start = SListBegin(src_list);
	slist_iterator_t src_runner_segment_end = SListBegin(src_list);
	
	/* assign dest_runner to be the first node of dest_list that is larger than the first node of src_list */
	slist_iterator_t dest_runner = SListFind(dest_list, SListBegin(dest_list), SListEnd(dest_list), SListGetData(SListBegin(src_list)));
	
	while (!SListIsEqualIter(dest_runner, SListEnd(dest_list)) &&
		!SListIsEqualIter(src_runner_segment_start, SListEnd(src_list)))
	{
		/* find the first element in src_list (between src_runner_segment_start and end of src list) that is larger than dest_runner, and assign it to src_runner_end */
		src_runner_segment_end = SListFind(src_list, src_runner_segment_start, SListEnd(src_list), SListGetData(dest_runner));
		
		/* skip splice if src_runner == dest_runner. nothing to splice at this iteration */
		if (!SListIsEqualIter(src_runner_segment_start, src_runner_segment_end))
		{
			DLListSplice(src_runner_segment_start.iter, src_runner_segment_end.iter, dest_runner.iter);
		}
		src_runner_segment_start = src_runner_segment_end;
		
		dest_runner = SListNext(dest_runner);
	}
	
	/* if dest_runner reaches end of dest_list and src_list still has remaining elements, splice all remaining src elements to the end of dest_list */
	if (SListIsEqualIter(dest_runner, SListEnd(dest_list)) &&
		!SListIsEqualIter(src_runner_segment_start, SListEnd(src_list)))
	{
		DLListSplice(src_runner_segment_start.iter, SListEnd(src_list).iter, dest_runner.iter);
	}
	
	return dest_list;
}


/* helper function for printing list in sortrd_list_test.c file, not a part of the API */
dll_t *SListGetDLL(slist_t *list)
{
	return list->d_list;
}


