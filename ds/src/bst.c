/*
date: 16/7
reviewer: Amit E.
status: done
*/


#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */

#include "bst.h"	/* bst_t typedef, bst_t functions */


#define IterToNode(x) (struct bst_node_t *)(x)
#define NodeToIter(x) (bst_iterator_t)(x)

enum child_t {HAS_RIGHT_CHILD, HAS_LEFT_CHILD};

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



static void *BSTIteratorGetData(bst_iterator_t iterator);
static struct bst_node_t *BSTCreateNode(void *data);
static struct bst_node_t *GetSmallestLeaf(bst_iterator_t sub_tree_root);
static struct bst_node_t *GetBiggestLeaf(bst_iterator_t sub_tree_root);
static void RemoveHasOneChild(struct bst_node_t *iterator_to_remove, enum child_t child);
static void RemoveHasNoChildren(struct bst_node_t *iterator_to_remove);
static void RemoveHasTwoChildren(struct bst_node_t *iterator_to_remove);


bst_t *BSTCreate(bst_compare_func_t cmp_func)
{
	bst_t *bst = NULL;
	
	assert(NULL != cmp_func);
	
	bst = (bst_t *)malloc(sizeof(bst_t));
	if (NULL == bst)
	{
		return NULL;
	}
	
	bst->root = BSTCreateNode(NULL);
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
	/* post-order traversal*/
	
	struct bst_node_t *runner = NULL;
	struct bst_node_t *parent = NULL;

	assert(tree);

	runner = tree->root->left;
	
	while (NULL != runner && runner != tree->root)
	{
		
		if (NULL != runner->left)
		{
			runner = runner->left;
		}
		else if (NULL != runner->right)
		{
			runner = runner->right;
		}
		else
		{
			parent = runner->parent;
			BSTRemove(runner);
			runner = parent;
		}
	}
	
	free(tree->root);
	free(tree);
}



bst_iterator_t BSTInsert(bst_t *tree, void *data)
{
	struct bst_node_t *new_node = BSTCreateNode(data);
	struct bst_node_t *runner = NULL;
	struct bst_node_t *parent = NULL;
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

	return NodeToIter(new_node);
}




void *BSTRemove(bst_iterator_t iterator_to_remove)
{
	void *data = NULL;
	assert(iterator_to_remove);
	
	data = BSTGetData(iterator_to_remove);
	
	/* if the iterator to remove is a leaf node */
	if(iterator_to_remove->left == NULL && iterator_to_remove->right == NULL)
	{
		RemoveHasNoChildren(iterator_to_remove);
	}
	/* if the iterator to remove only has a left child */
	else if (NULL == iterator_to_remove->right)
	{
		RemoveHasOneChild(iterator_to_remove, HAS_LEFT_CHILD);
	}
	else if (NULL == iterator_to_remove->left)
	{
		
		RemoveHasOneChild(iterator_to_remove, HAS_RIGHT_CHILD);
	}
	else	/* iterator to remove has 2 children */
	{
		RemoveHasTwoChildren(iterator_to_remove);
	}
	
	return data;
}




bst_iterator_t BSTFind(const bst_t *tree, void *data)
{
	struct bst_node_t *runner = NULL;
	int compare_result = 0;
	
	assert(tree);
	runner = tree->root->left;
	
	while (NULL != runner) 
	{
		compare_result = (tree->compare_func)(runner->data, data);
		
		if (compare_result == 0)
		{
			return NodeToIter(runner);
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
	struct bst_node_t *runner = NULL;
	
	assert(tree);
	
	runner = BSTBegin(tree);
	if (NULL == runner)
	{
		return 0;
	}
	
	while (!BSTIsSameIter(NodeToIter(runner) , BSTEnd(tree)))
	{
		runner = IterToNode(BSTNext(NodeToIter(runner)));
		++count;
	}
	
	return count;
}



bst_iterator_t BSTNext(const bst_iterator_t iterator)
{
	struct bst_node_t *runner = IterToNode(iterator);
	struct bst_node_t *parent = NULL;
	
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
		return NodeToIter(runner);
	}
}



bst_iterator_t BSTPrev(const bst_iterator_t iterator)	/*iterisequal*/
{
	struct bst_node_t *runner = IterToNode(iterator);
	struct bst_node_t *parent = NULL;
	
	assert(NULL != runner);
	
	if (NULL == runner->left)
	{
		parent = runner->parent;
		while (NULL != parent && runner != parent->right)
		{
			runner = parent;
			parent = runner->parent;
		}
		
		return (parent == NULL)? iterator : NodeToIter(parent);
	}
	else
	{
		runner = GetBiggestLeaf(runner->left);
		return NodeToIter(runner);
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
	
	return NodeToIter(GetSmallestLeaf(tree->root));
}



int BSTForEach(bst_t *tree, action_func_t action_func, void *param)
{
	struct bst_node_t *runner = NULL;
	int action_func_result = 0;
	
	assert(tree);
	assert(action_func);
	
	runner = IterToNode(BSTBegin(tree));
	
	while (!BSTIsSameIter(NodeToIter(runner) , BSTEnd(tree)))
	{
		action_func_result += (action_func)(BSTGetData(runner), param);
		runner = IterToNode(BSTNext(runner));
	}
	
	return action_func_result;
}



int BSTIsSameIter(bst_iterator_t iter1, bst_iterator_t iter2)
{
	return IterToNode(iter1) == IterToNode(iter2);
}





/******** static functions ********/




static void RemoveHasTwoChildren(struct bst_node_t *iterator_to_remove)
{
	struct bst_node_t *next_iterator = BSTNext(iterator_to_remove);
		
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

static void RemoveHasNoChildren(struct bst_node_t *iterator_to_remove)
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

static void RemoveHasOneChild(struct bst_node_t *iterator_to_remove, enum child_t child)
{
	if (child == HAS_LEFT_CHILD)
	{
		/* if the node to remove only has a right child */
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
	else 	/* node to remove only has a left child */
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
}



static struct bst_node_t *BSTCreateNode(void *data)
{
	struct bst_node_t *node = malloc(sizeof(struct bst_node_t));
	if (NULL == node)
	{
		return NULL;
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



static struct bst_node_t *GetSmallestLeaf(bst_iterator_t sub_tree_root)
{
	struct bst_node_t *runner = sub_tree_root;
	if (NULL == sub_tree_root)
	{
		return NULL;
	}
	
	while (NULL != runner->left)
	{
		runner = runner->left;
	}
	
	return runner;
}


static struct bst_node_t *GetBiggestLeaf(bst_iterator_t sub_tree_root)
{
	struct bst_node_t *runner = sub_tree_root;
	
	while (NULL != runner->right)
	{
		runner = runner->right;
	}
	
	return runner;
}
