#ifndef __DLINKED_LIST_H__
#define __DLINKED_LIST_H__

#include <stddef.h> /* size_t */


typedef struct dll dll_t;
typedef struct node *dll_iterator_t;

/*
Description: typedef to pointer function that preform action on the list elmenet data 
Params: (void *, void *) the first param data from the list element, the second param data sended by the user
Return Value: int represent status. 0 for success , non 0 for failure
*/
typedef int (*dll_action_func_t)(void *,void *);

/*
Description: typedef to pointer function that find the mactching data
Params: (void *, void *) two elements to check matching
Return Value: (does it match?) int that represent the status of the matching, 0 for match.
*/
typedef int (*dll_match_func_t)(const void *, const void *);

/*
Description: creates new doubly linked list
Params: (void)
Return Value: pointer to new instance of doubly linked list
Time Complexity: O(1)
Space: O(1)
*/
dll_t *DLListCreate(void);

/*
Description: frees the list and all of its nodes.
Params: (list_t*) list
Return Value: void
Time Complexity: O(n)
Space: O(1)
*/
void DLListDestroy(dll_t *list);

/*
Description: Creates a new node and adds it to the DLList.
allows consecutive linking of several nodes.
Params: (list_t *) list, (dll_iterator_t) where to insert the new element,
(void *) data to add to list
Return Value: (dll_iterator_t) iterator pointing at last inserted node
Errors: new node failed, return the iterator to the end of the list.
Time Complexity: O(1)
Space: O(1)

*/
dll_iterator_t DLListInsert(dll_t *list, dll_iterator_t iterator, void *data);

/*
Description: Removes the element that is references by the iterator
Params: (iterator) which element to remove
Return Value: the iterator after the removed iterator. 
Time Complexity: O(1)
Space: O(1)
can be invalid
*/
dll_iterator_t DLListRemove(dll_iterator_t iterator);

/*
Description: Return the count of all the elements in the list
Params: (linked list) ,
Return Value: size_t represent the number of elements in the list
Time Complexity: O(n)
Space: O(1)
*/
size_t  DLListCount(const dll_t *linked_list);

/*
Description: Set the data for specfic element in the list
Params: (iterator - where to set the new data and  new data) ,
Return Value: void 
Time Complexity: O(1)
Space: O(1)
*/
void DLListSetData(dll_iterator_t iterator, void* data);

/*
Description: Get the element from specifc element 
Params: (iterator) ,
Return Value: (void *)
Time Complexity: O(1)
Space: O(1)
*/
void *DLListGetData(dll_iterator_t iterator);

/*
Description: Find iterator to desired element from starting iterator
to end iterator 
Params: (start and end iterators and ), void *data - for copmarsion,
match_func_t - comparsion function
Return Value: On success the desired element iterator On failure iter_end
Time Complexity: O(n)
Space: O(1)
*/
dll_iterator_t DLListFind(dll_iterator_t iter_start, dll_iterator_t iter_end, void *data, dll_match_func_t func);

/*
Description: Get the iterator for the next element in the list. 
Params: (dll_iterator_t iter) ,
Return Value: dll_iterator_t
Time Complexity: O(1)
Space: O(1)
*/
dll_iterator_t DLListNext(dll_iterator_t iter);

/*
Description: Get the iterator for the previous element in the list
Params: (dll_iterator_t iter) ,
Return Value: dll_iterator_t
Time Complexity: O(1)
Space: O(1)
*/
dll_iterator_t DLListPrev(dll_iterator_t iter);

/*
Description: indication if list is empty 
Params: (list) ,
Return Value: (int) 1 for "true" (list is empty), 0 for "false".
Time Complexity: O(1)
Space: O(1)
*/
int DLListIsEmpty(const dll_t* list);

/*
Description: Get the first element iterator of the list
Params: (dll_t* list) ,
Return Value: iterator for the first element. If list is empty, returns end of list.
Time Complexity: O(1)
Space: O(1)
*/
dll_iterator_t DLListBegin(const dll_t* list);

/*
Description: Get invalid iterator, that follows the last valid element of the list.
Params: (dll_t* list) ,
Return Value: iterator for invalid iterator. Represent the end of the list
Time Complexity: O(1)
Space: O(1)
*/
dll_iterator_t DLListEnd(const dll_t* list);

/*
Description: Apply action_func_t func for each elemnet from starting iterator to
the end iterator
Params: (dll_iterator_t iter_start  iter_end) ,action_func_t the function
to preform on each element
Return Value: return end if the function succeeded. if failed, return iterator where failed.
Time Complexity: O(n)
Space: O(1)
*/
dll_iterator_t DLListForeach(dll_iterator_t iter_start, dll_iterator_t iter_end, void *data, dll_action_func_t func);

/*
Description: Checks if two iterators point to the same element
Params: dll_iterator_t it1, dll_iterator_t it2
Return Value: boolean value: 0 for not equal, !0 for equal
Time Complexity: O(1)
Space: O(1)
*/
int DLListIsEqualIter(dll_iterator_t it1, dll_iterator_t it2);

/*
Description: Remove the first iterator.
Params: dll_t *list
Return Value: void * to data popped 
Time Complexity: O(1)
Space: O(1)
*/
void *DLListPopFront(dll_t *list);

/*
Description: Remove the last iterator.
Params: dll_t *list
Return Value: void * to data popped 
Time Complexity: O(1)
Space: O(1)
*/
void *DLListPopBack(dll_t *list);

/*
Description: add element to the begin of the list.
Params: dll_t *list
Return Value:return iteratur to the element (if failure returns list end)
Time Complexity: O(1)
Space: O(1)
*/
dll_iterator_t DLListPushFront(dll_t *list, void *data);

/*
Description: add element to the end of the list.
Params: dll_t *list
Return Value:`return iteratur to the element (if failure returns list end)
Time Complexity: O(1)
Space: O(1)
*/
dll_iterator_t DLListPushBack(dll_t *list, void *data);

/*
Description: transfers a segment of elements (iterators) in a range (from->to (non inclusive)) a list container to another list container before a certain position. removes the element from the source list.Params: destination iterator, source iterator (dll_iterator_t).
Params: destination iterator, source iterator (dll_iterator_t).
Return Value: destination iterator (dll_iterator_t).
Time Complexity: O(1)
Space: O(1)
*/
dll_iterator_t DLListSplice(dll_iterator_t start_iterator ,dll_iterator_t end_iterator, dll_iterator_t dest_iterator);

/*
Description: insert the data in matching elements inside the range to list.
Params: list used to copy matching elements, iter_start iter_end - the range, data for the matching function, match_func_t matching function
Return Value: number of elements found. -1 if failed.
Time Complexity: O(n)
Space: O(1)
*/
int DLListMultiFind(dll_t *dest_list ,dll_iterator_t iter_start, dll_iterator_t iter_end, void* data, dll_match_func_t func);


#endif /* __LINKED_LIST_H__ */
