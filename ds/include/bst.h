#ifndef __BST_H__
#define __BST_H__

/******** typedefs: ********/ 

typedef struct binary_search_tree_t bst_t;
typedef struct  bst_node_t *bst_iterator_t;

/*
Description: typedef to funcion pointer that compares two elements.
Params: (void *, void *) two elements to compare.
Return Value:
            positive if first argument is larger,
            zero if equal,
            negative if the second argument is larger.
*/
typedef int (*bst_compare_func_t)(const void *, const void *);

/*
Description: typedef to funcion pointer. The function performs an action on dest with the data of param.
Params: (void *, void *) first param - pointer to data in an element, second param - pointer to data sent by the user
Return Value: int that represent status. 0 for success , non 0 for failure
*/
typedef int (*action_func_t)(void *dest, void *param);

/******** struct definitions (in src file!): ********/

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
}

/******** function forward declerations: ********/ 


/*
description: Create a new binary search tree
input: Pointer to compare function
return: Pointer to new BST. NULL if failed.
time complexity: O(1)
space complexity: O(1)
*/
bst_t *BSTCreate(bst_compare_func_t cmp_func);


/*
Description: Frees the tree and all of its elements.
Params: bst_t pointer.
Return Value: void
Time Complexity: O(n)
Space: O(1)
*/
void BSTDestroy(bst_t *tree);


/*
Description: Creates a new node and adds it to the tree. The node will be added to its appropriate location in a binary search tree.
Params: (bst_t *) tree pointer, (void *) pointer to data to be inserted.
Return Value: (bst_iterator_t) iterator of inserted node
Errors: failed to create node, returns invalid iterator.
Time Complexity: O(n) worst case, O(logn) average case.
Space: O(1)
*/
bst_iterator_t BSTInsert(bst_t *tree, void *data);


/*
Description: Removes the element that is referenced by the iterator
Params: (iterator) element to remove
Return Value: Data of removed element
Time Complexity: O(n) worst case, O(logn) avg case
Space: O(1)
*/
bst_iterator_t BSTRemove(bst_iterator_t iterator);  /* return value? */


/*
Description: Find iterator to desired element in tree
Params: tree pointer, data
Return Value: On success returns desired element. On failure returns root iterator.
Time Complexity: O(n) worst case, O(logn) average case
Space: O(1)
*/
bst_iterator_t BSTFind(const bst_t *tree, void *data);


/*
Description: Get the data from specifc element 
Params: bst_t iterator
Return Value: (void *) data
Time Complexity: O(1)
Space: O(1)
*/
void *BSTGetData(bst_iterator_t iterator);


/*
Description: chekc is tree is empty or not.
Params: pointer to tree.
Return Value: non-0 if empty, 0 if empty.
Time Complexity: O(1)
Space: O(1)
*/
int BSTIsEmpty(const bst_t *tree);


/*
Description: Get the number of elements in the ree. 
Params: bst_t pointer to tree.
Return Value: size_t number of lements.
Time Complexity: O(n)
Space: O(1)
*/
size_t BSTSize(const bst_t *tree);


/*
Description: Compare two elements of the tree.
Params: two bst_t iterators to be compared.
Return Value: 1 if equal, 0 if not (boolean int)
Time Complexity: O(1)
Space: O(1)
*/
int BSTIsSameIter(bst_iterator_t iter1, bst_iterator_t iter2);


/*
Description: Apply action_func_t func for each elemnet from starting iterator to
the end iterator
Params: start iterator, end iterator,action_func_t is the function
	to be executed on each element
Return Value: exit status of the action function
Time Complexity: O(n)
Space: O(1)
*/
int BSTForEach(bst_t *tree, action_func_t func, void *data_input);


/*
Description: Get the next iterator in an in-order traversal.
Params: bst_t iterator
Return Value: bst_t iterator
Time Complexity: O(n) worst case, O(logn) avg case
Space: O(1)
*/
bst_t iterator *BSTNext(const bst_iterator_t iterator);


/*
Description: Get the previous iterator in an in-order traversal.
Params: bst_t iterator
Return Value: bst_t iterator
Time Complexity:O(n)
Space: O(1)
*/
bst_t iterator *BSTPrev(const bst_iterator_t iterator);


/*
Description: Get the first iterator of an in-order traversal.
מה זה?
Params: bst_t iterator
Return Value: bst_t iterator
Time Complexity: O(logn) avg case, O(n) worst case
Space: O(1)
*/
bst_t iterator *BSTFBegin(const bst_iterator_t iterator);


/*
Description: Get the last iterator of an in-order traversal.
איבר שאין אחריו אף איבר. סוג של דאמי אחרי האיבר הגדול ביותר
Params: bst_t iterator
Return Value: bst_t iterator
Time Complexity: Time Complexity: O(1). save it somewhere
Space: O(1)
*/
bst_t iterator *BSTEnd(const bst_iterator_t iterator);

#endif  /* __BST_H__


