/*
date: 
reviewer: 
status: 
*/

#include <stdlib.h>	/* malloc */
#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */

#include "avl.h"


struct avl
{
    struct avl_node_t *root;
    avl_compare_func_t compare_func;
};


struct avl_node_t
{
    void *data;
    struct avl_node_t *left;
    struct avl_node_t *right;
    size_t height;
};


static size_t AVLCountNodes(avl_node_t *node);
static void *AVLFindNode(avl_node_t *node, avl_compare_func_t compare_func, void *data);
static int AVLForEachHelper(avl_node_t *node, avl_action_func_t action_func, void *params);

avl_t *AVLBSTCreate(avl_bst_compare_func_t cmp_func)
{
	bst_t *bst = NULL;
	
	assert(NULL != cmp_func);
	
	avl = (avl_bst_t *)malloc(sizeof(avl_bst_t));
	if (NULL == avl)
	{
		return NULL;
	}
	
	avl->root = NULL;

	bst->compare_func = cmp_func;
	
	return bst;
}



void AVLDestroy(avl_t *tree)
{
	if (tree->root == NULL)
	{
		return;
	}
	
	if (tree->root->left == NULL)
	{
		
	}
	
	AVLDestroy(tree->root->left);
	AVLDestroy(tree->root->right);
	
	struct bst_node_t *runner = NULL;
	
	
}


static avl_node_t *AVLInsertNode(avl_node_t *node, avl_compare_func_t compare_func, void *data)
{
	int compare_result = 0;
	
	if (NULL == node)
	{
		return node;
	}
	
	compare_result = (compare_func)(node->data, data);
	
	if (compare_result > 0)
	{
		++(node->height);
		return AVLInsertNode(node->right, compare_func, data);
	}
	else
	{
		++(node->height);
		return AVLInsertNode(node->left, compare_func, data);
	}
}

int AVLInsert(avl_t *tree, void *data)
{
	struct avl_node_t *new_node = NULL;
	struct avl_node_t *location_of_new_node = NULL;
	
	assert(tree);
	
	new_node = BSTCreateNode(data);
	if (NULL == new_node)
	{
		return 1;
	}

 	location_of_new_node = AVLInsertNode(tree->root, tree->compare_func, data);
 
	location_of_new_node = new_node;


	return 0;
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
	return AVLFindNode(tree->node, tree->compare_func, data);
}


static void *AVLFindNode(avl_node_t *node, avl_compare_func_t compare_func, void *data)
{
	int compare_result = 0;
	
	if (NULL == node || node->data == data)
	{
		return node->data;
	}
	
	compare_result = (compare_func)(node->data, data);
	
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
		return;
	}
	
	AVLForEachHelper(node->left, action_func, params);
	
	action_status = action_func(node->data, params);
	
	AVLForEachHelper(node->right, action_func, params);
	
	return action_status;
}







