/*
date: 
reviewer: 
status: 
*/

#include <stdlib.h>	/* malloc, free */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */

#include "avl.h"


struct avl
{
    struct avl_node_t *root;
    avl_compare_func_t compare_func;
};


typedef struct avl_node_t
{
    void *data;
    struct avl_node_t *left;
    struct avl_node_t *right;
    size_t height;
} avl_node_t;


/* helper functions related to remove/destroy */
static void AVLDestroyHelper(avl_node_t *node);
static avl_node_t *AVLRemoveHelper(avl_node_t *node_to_remove, avl_compare_func_t compare_func, void *data);
static avl_node_t *AVLRemoveNode(avl_node_t *node_to_remove, avl_compare_func_t compare_func);
static avl_node_t *FindInOrderSuccessor(avl_node_t *node_to_remove);
static avl_node_t *FindInOrderSuccessorRecursive(avl_node_t *runner);

/* helper functions related to insert */
static avl_node_t *AVLInsertNode(avl_node_t *runner, avl_node_t *new_node, avl_compare_func_t);
static size_t MaxHeightOfTwoNodes(avl_node_t *first, avl_node_t *second);
static avl_node_t *AVLRotateLeft(avl_node_t *node);
static avl_node_t *AVLRotateRight(avl_node_t *node);
static avl_node_t *AVLFixBalance(avl_node_t *root, int balance);
static int CalculateBalance(avl_node_t *node);

/* general helper functions */
static size_t AVLCountNodes(avl_node_t *node);
static void *AVLFindNode(avl_node_t *node, avl_compare_func_t compare_func, void *data);
static int AVLForEachHelper(avl_node_t *node, avl_action_func_t action_func, void *params);
static avl_node_t *AVLCreateNode(void *data);
static size_t AVLGetNodeHeight(const avl_node_t *node);
static size_t UpdateNodeHeight(avl_node_t *node);

/******** API functions ********/

avl_t *AVLCreate(avl_compare_func_t cmp_func)
{
	avl_t *avl = NULL;
	
	assert(NULL != cmp_func);
	
	avl = (avl_t *)malloc(sizeof(avl_t));
	if (NULL == avl)
	{
		return NULL;
	}
	
	avl->root = NULL;

	avl->compare_func = cmp_func;
	
	return avl;
}



void AVLDestroy(avl_t *tree)
{
	assert(tree);
	
	AVLDestroyHelper(tree->root);
	
	free(tree);
}




void AVLRemove(avl_t *tree, void *data)
{
	assert(tree);
	
	tree->root = AVLRemoveHelper(tree->root, tree->compare_func, data);
}




int AVLInsert(avl_t *tree, void *data)
{
	struct avl_node_t *new_node = NULL;
	
	assert(tree);
	
	new_node = AVLCreateNode(data);
	if (NULL == new_node)
	{
		return 1;
	}
	
	
	if (NULL == tree->root)	/* if tree is empty: insert first node */
	{
		tree->root = new_node;
	}
	else			/* else: insert in appropriate location using helper function */
	{
 		tree->root = AVLInsertNode(tree->root, new_node, tree->compare_func);
	}
	
	return 0;
}



size_t AVLHeight(const avl_t *tree)
{
	assert(tree);
	
	if(AVLIsEmpty(tree))
	{
		return 0;
	}

	
	return tree->root->height;
}



size_t AVLCount(const avl_t *tree)
{
	assert(tree);
	
	return AVLCountNodes(tree->root);
}



void *AVLFind(const avl_t *tree, const void *data)
{
	return AVLFindNode(tree->root, tree->compare_func, (void *)data);
}



int AVLIsEmpty(const avl_t *tree)
{
	assert(tree);
	
	return (tree->root == NULL);
}



int AVLForEach(avl_t *tree, avl_action_func_t action_func, void* params)
{
	assert(tree);
	
	return AVLForEachHelper(tree->root, action_func, params);
}




/*********** static functions ***********/


static avl_node_t *AVLInsertNode(avl_node_t *runner, avl_node_t *new_node, avl_compare_func_t compare_func)
{
	int compare_result = 0;
	void *data = new_node->data;
	int current_balance = 0;
	
	if (NULL != runner)
	{
		compare_result = (compare_func)(data, runner->data);
		if (compare_result > 0)
		{
			runner->right = AVLInsertNode(runner->right, new_node, compare_func);
			
		}
		else
		{
			runner->left = AVLInsertNode(runner->left, new_node, compare_func);
			
		}
	}
	else
	{
		runner = new_node;
	}
	
	/* update current node height with the max of the two sub-trees */
	runner->height = UpdateNodeHeight(runner);
	/* update current node balance */
	current_balance = CalculateBalance(runner);
	/* perform rotations if needed */
	runner = AVLFixBalance(runner, current_balance);
	
	return runner;
}



static size_t UpdateNodeHeight(avl_node_t *node)
{
	return 1 + MaxHeightOfTwoNodes(node->left, node->right);
}


static int CalculateBalance(avl_node_t *node)
{
	assert(node);
	
	return (AVLGetNodeHeight(node->left) - AVLGetNodeHeight(node->right));
}




static avl_node_t *AVLFixBalance(avl_node_t *root, int balance)
{
	if (balance >= -1 && balance <= 1)
	{
		return root;
	}
	
	
	if (balance >= 2)
	{
		if (CalculateBalance(root->left) >= 0)
		{
			/* left left scenario */
			root = AVLRotateRight(root);
		}
		else
		{
			/* left right scenario */
			root->left = AVLRotateLeft(root->left);
			root = AVLRotateRight(root);
		}
	}
	else
	{
		if (CalculateBalance(root->right) <= 0)
		{
			/* right right scenario */
			root = AVLRotateLeft(root);
		}
		else
		{
			/* right left scenario */
			root->right = AVLRotateRight(root->right);
			root = AVLRotateLeft(root);
		}
	}
	
	return root;
}




static avl_node_t *AVLRotateRight(avl_node_t *node)
{
	avl_node_t *root = node;
	avl_node_t *pivot = root->left;
	
	root->left = pivot->right;
	pivot->right = root;
	
	root->height = UpdateNodeHeight(root);
	pivot->height = UpdateNodeHeight(pivot);
	
	return pivot;
}




static avl_node_t *AVLRotateLeft(avl_node_t *node)
{
	avl_node_t *root = node;
	avl_node_t *pivot = root->right;
	
	root->right = pivot->left;
	pivot->left = root;
	
	root->height = UpdateNodeHeight(root);
	pivot->height = UpdateNodeHeight(pivot);
	
	return pivot;
}



static avl_node_t *AVLCreateNode(void *data)
{
	avl_node_t *node = malloc(sizeof(struct avl_node_t));
	if (NULL == node)
	{
		return NULL;
	}
	
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	
	return node;
}


/* post-order traversal and freeing */
static void AVLDestroyHelper(avl_node_t *node)
{
	if (node == NULL)
	{
		return;
	}
	
	AVLDestroyHelper(node->left);
	AVLDestroyHelper(node->right);
	free(node);
}




static avl_node_t *AVLRemoveHelper(avl_node_t *node_to_remove, avl_compare_func_t compare_func, void *data)
{
	int compare_result = 0;
	int current_balance = 0;
	
	if (NULL == node_to_remove)
	{
		return NULL;
	}
	
	compare_result = (compare_func)(data, node_to_remove->data);
	
	if (compare_result > 0)
	{
		node_to_remove->right = AVLRemoveHelper(node_to_remove->right, compare_func, data);
		
	}
	else if (compare_result < 0)
	{
		node_to_remove->left = AVLRemoveHelper(node_to_remove->left, compare_func, data);
	}
	else	/* found the node to remove */
	{
		node_to_remove = AVLRemoveNode(node_to_remove, compare_func);
	}
	
	
	if (NULL != node_to_remove)	/* if current node is not NULL, update height and balance, then fix balance (if needed) */
	{
		node_to_remove->height = UpdateNodeHeight(node_to_remove);
		current_balance = AVLGetNodeHeight(node_to_remove->left) - AVLGetNodeHeight(node_to_remove->right);	
		node_to_remove = AVLFixBalance(node_to_remove, current_balance);
	}
	
	return node_to_remove;


}




static avl_node_t *AVLRemoveNode(avl_node_t *node_to_remove, avl_compare_func_t compare_func)
{
	/* if the node to remove is a leaf */
	if(node_to_remove->left == NULL && node_to_remove->right == NULL)
	{
		free(node_to_remove);
		return NULL;
	}
	/* if the node to remove only has a left child */
	else if (NULL == node_to_remove->right)
	{
		avl_node_t *left_sub_tree = node_to_remove->left;
		free(node_to_remove);
		return left_sub_tree;
	}
	/* if the node to remove only has a right child */
	else if (NULL == node_to_remove->left)
	{
		avl_node_t *right_sub_tree = node_to_remove->right;
		free(node_to_remove);
		return right_sub_tree;
	}
	else	/* node to remove has 2 children */
	{
		/* swap node_to_remove with successor */
		avl_node_t *new_root = FindInOrderSuccessor(node_to_remove);
		node_to_remove->data = new_root->data;
		/* remove the duplicated node (that is located in the right sub-tree) */
		node_to_remove->right = AVLRemoveHelper(node_to_remove->right, compare_func, new_root->data); 
		return node_to_remove;
	}
}



/* in order traversal */
static int AVLForEachHelper(avl_node_t *node, avl_action_func_t action_func, void *params)
{
	int action_status = 0;
	
	if (NULL == node)
	{
		return 0;
	}
	
	AVLForEachHelper(node->left, action_func, params);
	
	action_status = action_func(node->data, params);
	
	AVLForEachHelper(node->right, action_func, params);
	
	return action_status;
}



static void *AVLFindNode(avl_node_t *node, avl_compare_func_t compare_func, void *data)
{
	int compare_result = 0;
	
	if (NULL == node)
	{
		return NULL;
	}
	
	if (node->data == data)
	{
		return node->data;
	}
	
	compare_result = (compare_func)(data, node->data);
	
	if (compare_result > 0)
	{
		return AVLFindNode(node->right, compare_func, data);
	}
	else
	{
		return AVLFindNode(node->left, compare_func, data);
	}
}


/* pre-order traversal */
static size_t AVLCountNodes(avl_node_t *node)
{
	if (NULL == node)
	{
		return 0;
	}
	
	return 1 + AVLCountNodes(node->left) + AVLCountNodes(node->right);
}



static size_t MaxHeightOfTwoNodes(avl_node_t *first, avl_node_t *second)
{
	size_t a = AVLGetNodeHeight(first);
	size_t b = AVLGetNodeHeight(second);
	
	return (a > b? a : b);
}



static size_t AVLGetNodeHeight(const avl_node_t *node)
{
	if (NULL == node)
	{
		return 0;
	}
	
	return node->height;
}



static avl_node_t *FindInOrderSuccessor(avl_node_t *node_to_remove)
{
	avl_node_t *runner = node_to_remove->right;
	
	/* recursive function that returns the left-most leaf of runner */
	return FindInOrderSuccessorRecursive(runner);
}


static avl_node_t *FindInOrderSuccessorRecursive(avl_node_t *runner)
{
	if (NULL == runner->left)
	{
		return runner;
	}
	
	return FindInOrderSuccessorRecursive(runner->left);
}

