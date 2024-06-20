/*
date: 
reviewer: 
status: 
*/

#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */
#include <stdio.h>	/* printf */ 	/**********************/

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


static size_t AVLCountNodes(avl_node_t *node);
static void *AVLFindNode(avl_node_t *node, avl_compare_func_t compare_func, void *data);
static int AVLForEachHelper(avl_node_t *node, avl_action_func_t action_func, void *params);
static avl_node_t *AVLCreateNode(void *data);
static size_t AVLGetNodeHeight(const avl_node_t *node);

static void AVLDestroyHelper(avl_node_t *node, avl_compare_func_t compare_func);
static avl_node_t *AVLRemoveHelper(avl_node_t *node_to_remove, avl_compare_func_t compare_func, void *data);
static avl_node_t *AVLRemoveNode(avl_node_t *node_to_remove);
static avl_node_t *FindInOrderSuccessor(avl_node_t *node_to_remove);

static avl_node_t *AVLInsertNode(avl_node_t *runner, avl_node_t *new_node, avl_compare_func_t);
static size_t MaxHeightOfTwoNodes(avl_node_t *first, avl_node_t *second);
static avl_node_t *AVLRotateLeft(avl_node_t *node);
static avl_node_t *AVLRotateRight(avl_node_t *node);
static avl_node_t *AVLFixBalance(avl_node_t *root, int balance);
static int CalculateBalance(avl_node_t *node);


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
	
	AVLDestroyHelper(tree->root, tree->compare_func);
	
	free(tree);
}



static void AVLDestroyHelper(avl_node_t *node, avl_compare_func_t compare_func)
{
	if (node == NULL)
	{
		return;
	}
	
	printf("destroying: %d\n", *(int *)node->data);
	AVLDestroyHelper(node->left, compare_func);
	AVLDestroyHelper(node->right, compare_func);
	
	AVLRemoveHelper(node, compare_func, node->data);
}

void AVLRemove(avl_t *tree, void *data)	/* TODO */
{
	assert(tree);
	
	tree->root = AVLRemoveHelper(tree->root, tree->compare_func, data);
}

static avl_node_t *AVLRemoveHelper(avl_node_t *node_to_remove, avl_compare_func_t compare_func, void *data)
{
	int compare_result = 0;
	int current_balance = 0;
	
	if (node_to_remove == NULL)
	{
		return;
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
	else /* found */
	{
		node_to_remove = AVLRemoveNode(node_to_remove);
	}
	
	
	if (NULL != node_to_remove)
	{
		node_to_remove->height = 1 + MaxHeightOfTwoNodes(node_to_remove->left, node_to_remove->right);
		current_balance = AVLGetNodeHeight(node_to_remove->left) - AVLGetNodeHeight(node_to_remove->right);	
		node_to_remove = AVLFixBalance(node_to_remove, current_balance);
	}
	
	return node_to_remove;


}



static avl_node_t *AVLRemoveNode(avl_node_t *node_to_remove)
{
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
	else if (NULL == node_to_remove->left)
	{
		avl_node_t *right_sub_tree = node_to_remove->right;
		free(node_to_remove);
		return right_sub_tree;
	}
	else	/* node to remove has 2 children */
	{
		avl_node_t *new_root = FindInOrderSuccessor(node_to_remove);
		new_root->right = node_to_remove->right;
		new_root->left = node_to_remove->left;
		new_root->data = node_to_remove->data;
		free(node_to_remove);
		return new_root;
	}
}


static avl_node_t *FindInOrderSuccessor(avl_node_t *node_to_remove)
{
	avl_node_t *runner = node_to_remove->right;
	
	while (NULL != runner->left)
	{
		runner = runner->left;
	}
	
	return runner;
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
	
	if (NULL == tree->root)
	{
		tree->root = new_node;
	}
	else
	{
 		tree->root = AVLInsertNode(tree->root, new_node, tree->compare_func);
	}
	return 0;
}


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
	
	
	runner->height = 1 + MaxHeightOfTwoNodes(runner->left, runner->right);
	
	current_balance = CalculateBalance(runner);
	
	runner = AVLFixBalance(runner, current_balance);
	
	return runner;
}

static int CalculateBalance(avl_node_t *node)
{
	if (node == NULL)
	{
		printf("CalculateBalance error");	/* shouldnt ever happen, debug only, delete this */
	}
	
	return (AVLGetNodeHeight(node->left) - AVLGetNodeHeight(node->right));
}

static avl_node_t *AVLFixBalance(avl_node_t *root, int balance)
{
	if (balance >= -1 && balance <= 1)
	{
		return;
	}
	
	
	if (balance >= 2)
	{
		if (CalculateBalance(root->left) >= 1)			/* if (root->left->left != NULL) */	
		{
			/* left left */
			root = AVLRotateRight(root);
			printf("\nleft left\n");
		}
		else
		{
			/* left right*/
			root->left = AVLRotateLeft(root->left);
			root = AVLRotateRight(root);
			printf("\nleft right\n");
		}
	}
	else
	{
		if (CalculateBalance(root->right) <= -1)		/*  if (root->right->right != NULL) */
		{
			/* right right */
			root = AVLRotateLeft(root);
			printf("\nright right\n");
		}
		else
		{
			/* right left */
			root->right = AVLRotateRight(root->right);
			root = AVLRotateLeft(root);
			printf("\nright left\n");
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
	
	root->height = 1 + MaxHeightOfTwoNodes(root->left, root->right);
	pivot->height = 1 + MaxHeightOfTwoNodes(pivot->left, pivot->right);
	
	return pivot;
}


static avl_node_t *AVLRotateLeft(avl_node_t *node)
{
	avl_node_t *root = node;
	avl_node_t *pivot = root->right;
	
	root->right = pivot->left;
	pivot->left = root;
	
	root->height = 1 + MaxHeightOfTwoNodes(root->left, root->right);
	pivot->height = 1 + MaxHeightOfTwoNodes(pivot->left, pivot->right);
	
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

size_t AVLHeight(const avl_t *tree)
{
	assert(tree);
	
	if (NULL == tree->root)
	{
		return 0;
	}
	
	return tree->root->height;
}


static size_t AVLGetNodeHeight(const avl_node_t *node)
{
	if (NULL == node)
	{
		return 0;
	}
	
	return node->height;
}


size_t AVLCount(const avl_t *tree)
{
	return AVLCountNodes(tree->root);
}

static size_t AVLCountNodes(avl_node_t *node)
{
	if (NULL == node)
	{
		return 0;
	}
	
	return 1 + AVLCountNodes(node->left) + AVLCountNodes(node->right);
}



void *AVLFind(const avl_t *tree, const void *data)
{
	return AVLFindNode(tree->root, tree->compare_func, (void *)data);
}


static void *AVLFindNode(avl_node_t *node, avl_compare_func_t compare_func, void *data)
{
	int compare_result = 0;
	
	if (NULL == node || node->data == data)
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


int AVLIsEmpty(const avl_t *tree)
{
	return (tree->root == NULL);
}


int AVLForEach(avl_t *tree, avl_action_func_t action_func, void* params)
{
	return AVLForEachHelper(tree->root, action_func, params);
}


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







static size_t MaxHeightOfTwoNodes(avl_node_t *first, avl_node_t *second)
{
	size_t a = AVLGetNodeHeight(first);
	size_t b = AVLGetNodeHeight(second);
	
	return (a > b? a : b);
}


