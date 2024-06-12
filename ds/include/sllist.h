#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <stddef.h> /* size_t */

typedef struct linked_list linked_list_t;
typedef struct node *iterator_t;

typedef int (*action_func_t)(void*, void*);
typedef int (*comp_func_t)(void*, void*);

/*
Description: creates new singly linked list
Params: (void) ,
Return Value: pointer to new instance of singly linked list
Time Complexity: O(1)
Space: O(1)
*/
linked_list_t *SLListCreate(void);

/*
Description: frees the list and all of its nodes.
Params: (list_t*) list
Return Value: void
Time Complexity: O(n)
Space: O(1)
*/
void SLListDestroy(linked_list_t *list);

/*
Description: Creates a new node and adds it to the LList
Params: (list_t *) list, (iterator_t) where to insert the new element,
(void *) data to add to list
Return Value: (iterator_t) iterator pointing at last inserted node
Errors: new node malloc failed, return the iterator to the end of the list  
Time Complexity: O(1)
Space: O(n)
Notes: this functionality allows convenient linking of several nodes
*/
iterator_t SLListInsert(linked_list_t *list, void *data, iterator_t it);

/*
Description: Remove element in specfic iterator
Params: (iterator) which element to remove
Return Value: void 
Time Complexity: O(1)
Space: O(1)
*/
void SLListRemove(iterator_t iterator);

/*
Description: Return the count of all the elements in the list
Params: (linked list) ,
Return Value: 
Time Complexity: O(n)
Space: O(1)
*/
size_t SLListCount(const linked_list_t *linked_list);

/*
Description: Set the data for specfic element in the list
Params: (iterator - where to set the new data and  new data) ,
Return Value: void 
Time Complexity: O(1)
Space: O(1)
*/
void SLListSetData(iterator_t iterator, void* data);

/*
Description: Get the element from specifc element 
Params: (iterator) ,
Return Value: (void *)
Time Complexity: O(1)
Space: O(1)
*/
void *SLListGetData(const iterator_t iterator);

/*
Description: Find iterator to desired element from starting iterator
to end iterator 
Params: (start and end iterators and ), void *data - for copmarsion,
comp_func_t - comparsion function
Return Value: On succsses the desired element iterator On failure NULL
Time Complexity: O(n)
Space: O(1)
*/
iterator_t SLListFind(iterator_t iter_start, iterator_t iter_end,
					  void* data, comp_func_t func);

/*
Description: Get the iterator for the next element in the list
Params: (iterator_t iter) ,
Return Value: iterator_t
Time Complexity: O(1)
Space: O(1)
*/
iterator_t SLListNext(iterator_t iter);

/*
Description: indication if list is empty 
Params: (list) ,
Return Value: boolean 1 for empty 0 for not
Time Complexity: O(1)
Space: O(1)
*/
int SLListIsEmpty(const linked_list_t* list);

/*
Description: Get the first element iterator of the list
Params: (linked_list_t* list) ,
Return Value: iterator for the first element
Time Complexity: O(1)
Space: O(1)
*/
iterator_t SLListGetBegin(linked_list_t* list);

/*
Description: Get the last element iterator of the list
Params: (linked_list_t* list) ,
Return Value: iterator for the first element
Time Complexity: O(1)
Space: O(1)
*/
iterator_t SLListGetEnd(linked_list_t* list);

/*
Description: Apply action_func_t func for each elemnet from starting iterator to
the end iterator
Params: start iterator, end iterator,action_func_t is the function
	to be executed on each element
Return Value: exit status of the action function
Time Complexity: O(n)
Space: O(1)
*/
int SLListForEach(iterator_t start, iterator_t end,
 				   action_func_t func, void *data_input);

/*
Description: Checks if two iterators point to the same element
Params: (iterator_t it1, iterator_t it2) ,
Return Value: boolean value 1 for equal 0 for not
Time Complexity: O(1)
Space: O(1)
*/
int SLListIsEqual(iterator_t it1, iterator_t it2);

/*
Description: Append list2 to the end of list1 and empty list2 
Params: (linked_list_t* list1, linked_list_t* list2) ,
Return Value: iterator to the starting element of list1 
Time Complexity: O(1)
Space: O(1)
*/
int SLListAppendLists(linked_list_t* list1, linked_list_t* list2);

#endif /* __LINKED_LIST_H__ */

