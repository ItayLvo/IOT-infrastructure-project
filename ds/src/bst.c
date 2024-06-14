/*
date: 14/7
reviewer:
status: waiting for CR | need to implement static functions to handle iterators/nodes
*/


#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */

#include "bst.h"	/* bst_t typedef, bst_t functions */

static bst_iterator_t BSTCreateIterator(void *data);
static void *BSTIteratorGetData(bst_iterator_t iterator);
static bst_iterator_t GetSmallestLeaf(bst_iterator_t sub_tree_root);
static bst_iterator_t GetBiggestLeaf(bst_iterator_t sub_tree_root);


struct binary_search_tree_t
{
    struct bst_node_t *root;
    bst_compare_func_t compare_func;
};


struct bst_node_t
{
    void *data;
    struct bst_node_t *left;
    struct bst_node_t *right;
    struct bst_node_t *parent;
};




bst_t *BSTCreate(bst_compare_func_t cmp_func)
{
	bst_t *bst = NULL;
	
	assert(NULL != cmp_func);
	
	bst = (bst_t *)malloc(sizeof(bst_t));
	if (NULL == bst)
	{
		return NULL;
	}
	
	bst->root = BSTCreateIterator(NULL);
	if (NULL == bst->root)
	{
		free(bst);
		return NULL;
	}

	bst->compare_func = cmp_func;
	
	return bst;
}


void BSTDestroy(bst_t *tree)
{
	assert(tree);
	
	while(!BSTIsEmpty(tree))
	{
		BSTRemove(BSTBegin(tree));
	}
	
	free(tree->root);
	free(tree);
}



bst_iterator_t BSTInsert(bst_t *tree, void *data)
{
	bst_iterator_t new_node = BSTCreateIterator(data);
	bst_iterator_t runner = NULL;
	bst_iterator_t parent = NULL; /* todo invalid iter */
	int compare_result = 0;
	
	assert(tree);
	
	parent = tree->root;
	runner = tree->root->left;
	
	if (NULL == new_node)
	{
		return BSTEnd(tree);
	}

 
	while (NULL != runner) 
	{
		parent = runner;
		compare_result = (tree->compare_func)(runner->data, data);
		
		/* if (runner->data) > data => go to left sub-tree */
		if (compare_result > 0)
		{
			runner = runner->left;
		}
		else
		{
			runner = runner->right;
		}
	}
 
	if (parent == tree->root)
	{
		parent->left = new_node;
 	}
	else if (compare_result > 0)
	{
		parent->left = new_node;
 	}
	else
	{
		parent->right = new_node;
	}

	new_node->parent = parent;

	return new_node;
}




void *BSTRemove(bst_iterator_t iterator_to_remove)
{
	void *data = NULL;
	bst_iterator_t next_iterator = NULL;
	assert(iterator_to_remove);
	
	data = BSTGetData(iterator_to_remove);
	
	/* if the iterator to remove is a leaf node */
	if(iterator_to_remove->left == NULL && iterator_to_remove->right == NULL)
	{
		/* if the iterator to remove is a left child */
		if(iterator_to_remove->parent->left == iterator_to_remove)
		{
			iterator_to_remove->parent->left = NULL;
		}
		/* if the iterator to remove is a right child */
		else
		{
			iterator_to_remove->parent->right = NULL;
		}
		
		free(iterator_to_remove);
	}
	/* if the iterator to remove only has a left child */
	else if (NULL == iterator_to_remove->right)
	{
		/* if the iterator to remove is a left child */
		if (iterator_to_remove->parent->left == iterator_to_remove)
		{
			iterator_to_remove->parent->left = iterator_to_remove->left;
		}
		else
		{
			iterator_to_remove->parent->right = iterator_to_remove->left;
		}
		
		iterator_to_remove->left->parent = iterator_to_remove->parent;
		free(iterator_to_remove);
	}
	else if (NULL == iterator_to_remove->left)
	{
		
		if (iterator_to_remove->parent->left == iterator_to_remove)
		{
			iterator_to_remove->parent->left = iterator_to_remove->right;
		}
		else
		{
			iterator_to_remove->parent->right = iterator_to_remove->right;
		}
		
		iterator_to_remove->right->parent = iterator_to_remove->parent;
		free(iterator_to_remove);
	}
	else	/* iterator to remove has 2 children */
	{
		next_iterator = BSTNext(iterator_to_remove);
		
		iterator_to_remove->data = next_iterator->data;
		
		if (BSTIsSameIter(next_iterator->parent, iterator_to_remove))
		{
			iterator_to_remove->right = next_iterator->right;
			if (NULL != next_iterator->right)
			{
				next_iterator->right->parent = iterator_to_remove;
			}
		}
		else
		{
			next_iterator->parent->left = next_iterator->right;
			if (NULL != next_iterator->right)
			{
				next_iterator->right->parent = next_iterator->parent;
			}
		}
		
		free(next_iterator);
	}
	
	return data;
}




bst_iterator_t BSTFind(const bst_t *tree, void *data)
{
	bst_iterator_t runner = NULL; /* todo invalid iter */
	int compare_result = 0;
	
	assert(tree);
	runner = tree->root->left;
	
	while (NULL != runner) 
	{
		compare_result = (tree->compare_func)(runner->data, data);
		
		if (compare_result == 0)
		{
			return runner;
		}
		else if (compare_result > 0)
		{
			runner = runner->left;
		}
		else
		{
			runner = runner->right;
		}
	}
	
	return BSTEnd(tree);
}


void *BSTGetData(bst_iterator_t iterator)
{
	return BSTIteratorGetData(iterator);
}


int BSTIsEmpty(const bst_t *tree)
{
	assert(tree);

	return (NULL == tree->root->left);
}


size_t BSTSize(const bst_t *tree)
{
	size_t count = 0;
	bst_iterator_t runner = {0};	/* todo get invalid iter */
	
	assert(tree);
	
	runner = BSTBegin(tree);
	if (NULL == runner)
	{
		return 0;
	}
	
	while (!BSTIsSameIter(runner , BSTEnd(tree)))
	{
		runner = BSTNext(runner);
		++count;
	}
	
	return count;
}



bst_iterator_t BSTNext(const bst_iterator_t iterator)
{
	bst_iterator_t runner = iterator;
	bst_iterator_t parent = NULL;
	
	assert(runner);
	
	if (NULL == runner->right)
	{
		parent = runner->parent;
		while (runner != parent->left)
		{
			runner = parent;
			parent = runner->parent;
		}
		
		return parent;
	}
	else
	{
		runner = GetSmallestLeaf(runner->right);
		return runner;
	}
}



bst_iterator_t BSTPrev(const bst_iterator_t iterator)	/*iterisequal*/
{
	bst_iterator_t runner = iterator;
	bst_iterator_t parent = NULL;
	
	assert(NULL != runner);
	
	if (NULL == runner->left)
	{
		parent = runner->parent;
		while (NULL != parent && runner != parent->right)
		{
			runner = parent;
			parent = runner->parent;
		}
		
		return (parent == NULL)? iterator : parent;
	}
	else
	{
		runner = GetBiggestLeaf(runner->left);
		return runner;
	}
}



bst_iterator_t BSTEnd(const bst_t *tree)
{
	assert(NULL != tree);
	
	return (tree->root);
}



bst_iterator_t BSTBegin(const bst_t *tree)
{
	assert(tree);
	
	return GetSmallestLeaf(tree->root);
}



int BSTForEach(bst_t *tree, action_func_t action_func, void *param)
{
	bst_iterator_t runner = {0};
	int action_func_result = 0;
	
	assert(tree);
	assert(action_func);
	
	runner = BSTBegin(tree);
	
	while (!BSTIsSameIter(runner , BSTEnd(tree)))
	{
		action_func_result += (action_func)(BSTGetData(runner), param);
		runner = BSTNext(runner);
	}
	
	return action_func_result;
}



int BSTIsSameIter(bst_iterator_t iter1, bst_iterator_t iter2)
{
	return iter1 == iter2; /* TODO iter to node */
}





/******** static functions ********/

static bst_iterator_t BSTCreateIterator(void *data)
{
	bst_iterator_t node = malloc(sizeof(struct bst_node_t));
	if (NULL == node)
	{
		return NULL;	/* TODO return invalid iterator */
	}
	
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->parent = NULL;
	
	return node;
}



static void *BSTIteratorGetData(bst_iterator_t iterator)
{
	return iterator->data;
}



static bst_iterator_t GetSmallestLeaf(bst_iterator_t sub_tree_root)
{
	bst_iterator_t runner = sub_tree_root;
	if (NULL == sub_tree_root)	/* todo invalid iterator */
	{
		return NULL;
	}
	
	while (NULL != runner->left)
	{
		runner = runner->left;
	}
	
	return runner;
}


static bst_iterator_t GetBiggestLeaf(bst_iterator_t sub_tree_root)
{
	bst_iterator_t runner = sub_tree_root;
	
	while (NULL != runner->right)
	{
		runner = runner->right;
	}
	
	return runner;
}
