/*
Date: 
Status: 
Reviewer: 
*/

/* TODO: fix backtracking bug where 35x3 results in 35, 32, 33 but correct bit path */
/* TODO: check if max path reached and return trie full */

#include <stddef.h>	/* size_t */
#include <assert.h>	/* assert */
#include <stdlib.h>	/* malloc, free */
#include <stdio.h>	/* printf */

#include "trie.h"	/* trie function declerations */

#define NUMBER_OF_CHILDREN 2

#define NOT_FULL 0
#define FULL 1

#define LEFT 0
#define RIGHT 1



typedef struct trie_node_t
{
	struct trie_node_t *parent;
	struct trie_node_t *children[NUMBER_OF_CHILDREN];
	int is_full;
} trie_node_t;


struct trie
{
	trie_node_t *root;
	size_t max_height;
};



/* static functions forward declerations */
static trie_node_t *TrieCreateNode(trie_node_t *parent);
static trie_node_t *GetChild(trie_node_t *parent, int side);
static int GetBitAtIndex(unsigned int key, size_t index);
static e_trie_status TrieInsertHelper(trie_node_t *runner, unsigned int requested_key, unsigned int *result_key, size_t current_level);
static void UpdateNodeIsFull(trie_node_t *node);
static trie_node_t *GetChild(trie_node_t *parent, int side);
static void TrieDestroyHelper(trie_node_t *node);
static void TrieRemoveHelper(trie_node_t *runner, unsigned int key_to_remove, size_t current_level);
static size_t TrieSizeHelper(trie_node_t *node, size_t current_level);


/**** API functions ****/

trie_t *TrieCreate(size_t trie_height)
{
	trie_t *trie = NULL;
	
	assert(0 != trie_height);
	
	trie = (trie_t *)malloc(sizeof(trie_t));
	if (NULL == trie)
	{
		return NULL;
	}
	
	trie->root = TrieCreateNode(NULL);
	if (NULL == trie->root)
	{
		free(trie);
		return NULL;
	}

	trie->max_height = trie_height;
	
	return trie;
}


e_trie_status TrieInsert(trie_t *trie, const unsigned int requested_key, unsigned int *result_key)
{
	e_trie_status insert_status = SUCCESS;
	
	assert(trie);
	assert (result_key);
	
	*result_key = 0;
	insert_status = TrieInsertHelper(trie->root, requested_key, result_key, trie->max_height - 1);
	
	
	return insert_status;
}



void TrieRemove(trie_t *trie, unsigned int key_to_remove)
{
	assert(trie);	
	
	TrieRemoveHelper(trie->root, key_to_remove, trie->max_height - 1);
}



void TrieDestroy(trie_t *trie)
{
	assert(trie);
	
	TrieDestroyHelper(trie->root);
	
	free(trie);
}


/* counts the amount of full paths from root to leaf, where the leaf is full */
size_t TrieSize(const trie_t *trie)
{
	assert (trie);
	
	return TrieSizeHelper(trie->root, trie->max_height - 1);
}



/**** static helper functions ****/


static size_t TrieSizeHelper(trie_node_t *node, size_t current_level)
{
	if (NULL == node)
	{
		return 0;
	}
	
	if (node->is_full == FULL)
	{
		return 1;
	}
	
	return TrieSizeHelper(GetChild(node, LEFT), current_level - 1) +
			TrieSizeHelper(GetChild(node, RIGHT), current_level - 1);
	
}


static void TrieRemoveHelper(trie_node_t *runner, unsigned int key_to_remove, size_t current_level)
{
	unsigned int current_bit = 0;
	trie_node_t *child = NULL;
	
	current_bit = GetBitAtIndex(key_to_remove, current_level);
	child = GetChild(runner, current_bit);
	
	if (NULL == child)
	{
		return;
	}
	else
	{
		if (0 == current_level)
		{
			child->is_full = NOT_FULL;
			UpdateNodeIsFull(runner);
		}
		else
		{
			TrieRemoveHelper(child, key_to_remove, current_level - 1);
		}
	}
}


static e_trie_status TrieInsertHelper(trie_node_t *runner, unsigned int requested_key, unsigned int *result_key, size_t current_level)
{
	e_trie_status return_status = SUCCESS;
	unsigned int current_bit = 0;
	trie_node_t *child = NULL;
	
	
	/* set current_bit to 0 or 1 with bitwise operations on the requested_key, according to current height */
	current_bit = GetBitAtIndex(requested_key, current_level);
	child = GetChild(runner, current_bit);

	printf("current bit: %d\n", current_bit);
	
	/* if current node wasn't created yet - create node */
	if (NULL == child)
	{
		child = TrieCreateNode(runner);
		if (NULL == child)
		{
			return MEMORY_FALIURE;
		}
		
		runner->children[current_bit] = child;
		
		*result_key <<= 1;
		*result_key += current_bit;
		
		if (0 == current_level)
		{
			child->is_full = FULL;
			UpdateNodeIsFull(runner);
			
			return SUCCESS;
		}
		
		return_status = TrieInsertHelper(child, requested_key, result_key, current_level - 1);
	}
	
	/* if child was already created, and is not full */
	else if (child->is_full == NOT_FULL)
	{
		*result_key <<= 1;
		*result_key += current_bit;
		
		if (0 == current_level)
		{
			child->is_full = FULL;
			UpdateNodeIsFull(runner);
			
			return SUCCESS;
		}
		else
		{
			return_status = TrieInsertHelper(child, requested_key, result_key, current_level - 1);		
		}
	}
	
	/* if child is full */
	else
	{
		if (1 == current_bit) 	/* current bit is 0, but is full => revert last bit and backtrack */
		{
			/* TODO check if max size reached! */
			/* return TRIE_FULL;*/
			*result_key >>= 1;
			return_status = TrieInsertHelper(runner->parent, requested_key + (1 << current_level), result_key, current_level + 1);
			
		}
		else	/* current bit is 0, but is full => try right brother node */
		{
			return_status = TrieInsertHelper(runner, requested_key + (1 << current_level), result_key, current_level);
		}
		
	}
	
	return return_status;
}


static void TrieDestroyHelper(trie_node_t *node)
{
	if (NULL == node)
	{
		return;
	}
	
	TrieDestroyHelper(GetChild(node, LEFT));
	TrieDestroyHelper(GetChild(node, RIGHT));
	
	free(node);
}


static void UpdateNodeIsFull(trie_node_t *node)
{
	int is_full = FULL;
	size_t i = 0;
	
	for (i = 0; i < NUMBER_OF_CHILDREN && is_full == FULL; ++i)
	{
		if (NULL == node->children[i] || NOT_FULL == node->children[i]->is_full)
		{
			is_full = 0;
		}
	}
	
	node->is_full = is_full; 
}




static int GetBitAtIndex(unsigned int key, size_t index)
{
	return ((key >> index) & 1);
}


static trie_node_t *TrieCreateNode(trie_node_t *parent)
{
	trie_node_t *node = malloc(sizeof(trie_node_t));
	size_t i = 0;
	
	if (NULL == node)
	{
		return NULL;
	}
	
	node->parent = parent;
	node->is_full = NOT_FULL;
	
	for (i = 0; i < NUMBER_OF_CHILDREN; ++i)
	{
		node->children[i] = NULL;
	}
	
	return node;
}


static trie_node_t *GetChild(trie_node_t *parent, int side)
{
	return parent->children[side];
}

