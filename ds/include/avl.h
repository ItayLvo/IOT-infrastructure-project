
#ifndef __AVL_H__
#define __AVL_H__

#include <stddef.h>	/* size_t */

typedef struct avl avl_t;

/*
Description: typedef to pointer function that compare the two argument
Params: (void *, void *) two elements to compare
Return Value: int that represent the status of the matching:
   positive if the first argument is bigger,
   zero if they are equal
   negative if the second argument is bigger.
*/
typedef int (*avl_compare_func_t)(const void *data, const void *new_data);

/*
Description: typedef of a pointer to function. The function performs an action on the passed data of list element  
Params: (void *, void *) first param - pointer to data in an element, second param - pointer to data sent by the user
Return Value: int that represent status. 0 for success , non 0 for failure
*/
typedef int (*avl_action_func_t)(void *data, void *params);


/*
AVLCreate
description: create a binary search tree
input: a pointer to a compare function 
return: Pointer to binary tree. NULL if failed
time complexity: O(1)
space complexity: O(1)
*/
avl_t *AVLCreate(avl_cmp_func_t compare);

/*
AVLDestroy
description: destroy all the elements in the tree (Post-Ordered)
input: binary tree
return: nothing 
time complexity: O(n)
space complexity: O(1)
*/
void AVLDestroy(avl_t *tree);

/*
AVLSize
description: return the number of elements in the binary search tree (in pre ordered traverse) 
input:  binary tree
return: number of elements in the binary search tree
time complexity: O(n)
space complexity: O(1)
*/
size_t AVLCount(const avl_t *tree);

/*
AVLHeight
description: return the maximum depth of any leaf from the root
input:  binary tree pointer
return: maximum depth if any leaf from the root
time complexity: O(1)
space complexity: O(1)
*/
size_t AVLHeight(const avl_t *tree);

/*
AVLIsEmpty
description: check if AVL is empty
input:  binary tree pointer
return: non-0 if empty, 0 if not empty
time complexity: O(1)
space complexity: O(1)
*/
int AVLIsEmpty(const avl_t *tree);

/*
AVLInsert
description: insert a new element to the AVL
input: a binary tree and new data
return: 0 for success, non-0 for failure
time complexity: O(log n)
space complexity: O(1)
*/
int AVLInsert(avl_t *tree, void *data);

/*
AVLRemove
description: remove an element from the tree
input: a binary tree and new data
return: nothing
time complexity: O(log n)
space complexity: O(1)
*/
void AVLRemove(avl_t *tree, void *data);

/*
AVLFind
description: searches the tree for a certain element. 
input: a binary tree and data to search for
return: a generic pointer to the data if found, NULL otherwise
time complexity: O(log n)
space complexity: O(1)
*/
void *AVLFind(const avl_t *tree, const void *data);

/* 
AVLForEach
description: does a certain action on all the tree (do not change the key it self) In-order
input: binary tree, an action func and a parameter
return: the last status the action function returned
time complexity: O(n)
space complexity: O(1)
*/
int AVLForEach(avl_t *tree,
				 avl_action_func_t action,
				  void* params);


#endif /*__AVL_RECURSIVE_H__*/


