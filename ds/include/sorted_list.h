#ifndef __SORTED_LIST_H__
#define __SORTED_LIST_H__

#include <stddef.h> /* size_t */
#include "dllist.h"

typedef struct sorted_list slist_t;

struct iterator
{
    dll_iterator_t iter;
#ifdef _DEBUG
    sorted_list *sorted_list;
#endif /* _DEBUG */
};

typedef struct iterator slist_iterator_t;

/*
Description: typedef of a pointer to function. The function performs an action on the passed data of list element  
Params: (void *, void *) first param - pointer to data in an element, second param - pointer to data sent by the user
Return Value: int that represent status. 0 for success , non 0 for failure
*/
typedef int (*slist_action_func_t)(void*,void*);

/*
Description: typedef to pointer function that compare the two argument
Params: (void *, void *) two elements to compare
Return Value: int that represent the status of the matching:
    negetive if first argument is larger,
    zero if they equal
    positive if the second argument is larger.
*/
typedef int (*slist_compare_func_t)(const void*, const void*);

/*
Description: typedef to pointer function that checks if the two arguments are a match
Params: (void *, void *) two elements to compare
Return Value: int that represent the status of the matching, 1 for a match, 0 for not match
*/
typedef int (*slist_match_func_t)(const void*, const void*);


/*
Description: creates new sorted linked list
Params: compare function pointer - compares two elements
Return Value: pointer to new instance of sorted doubly linked list
Time Complexity: O(1)
Space: O(1)
*/
slist_t *SListCreate(slist_compare_func_t cmp_func);

/*
Description: frees the sorted list and all of its elements.
Params: (list_t*) list
Return Value: void
Time Complexity: O(n)
Space: O(1)
*/
void SListDestroy(slist_t *list);

/*
Description: Creates a new node and adds it to the DLList. The node will be added to its appropriate location in sorted order.
Params: (list_t *) list, (void *) pointer to data to be inserted
Return Value: (dll_iterator_t) iterator pointing to last inserted node
Errors: new node failed, return the iterator to the end of the list  
Time Complexity: O(1)
Space: O(1)
*/
slist_iterator_t SListInsert(slist_t *list,  void *data);

/*
Description: Removes the element that is references by the iterator
Params: (iterator) which element to remove
Return Value: the iterator after the removed iterator. 
Time Complexity: O(1)
Space: O(1)
can be invalid
*/
slist_iterator_t SListRemove(slist_iterator_t iter);

/*
Description: Return the count of all the elements in the list
Params: (linked list) ,
Return Value: size_t represent the number of elements in the list
Time Complexity: O(n)
Space: O(1)
*/
size_t SListCount(const slist_t *list);

/*
Description: Get the data from specifc element 
Params: (iterator) 
Return Value: (void *)
Time Complexity: O(1)
Space: O(1)
*/
void *SListGetData(slist_iterator_t iter);
/*
Description: Remove the first iterator.
Params: dll_t *list
Return Value: void * to data popped 
Time Complexity: O(1)
Space: O(1)
*/
void *SListPopFront(slist_t *list);

/*
Description: Remove the last iterator.
Params: dll_t *list
Return Value: void * to data popped 
Time Complexity: O(1)
Space: O(1)
*/
void *SListPopBack(slist_t *list);
/*
Description: Find iterator to desired element from starting iterator to end iterator 
Params: list pointer, start iterator, end iterators , void *data - for comparison)
Return Value: On success the first desired element iterator. On failure iter_end (invalid)
Time Complexity: O(n)
Space: O(1)
*/
slist_iterator_t SListFind(slist_iterator_t *list, slist_iterator_t start_iter, 
                           slist_iterator_t end_iter, void* data);

/*
Description: Find iterator to desired element from starting iterator to end iterator 
Params: (start iterator, end iterators , void *data - for comparison, match_func_t - comparsion function)
Return Value: On success the first desired element iterator. On failure iter_end (invalid)
Time Complexity: O(n)
Space: O(1)
*/
slist_iterator_t SListFindIf(slist_iterator_t start_iter, slist_iterator_t end_iter, void* data, slist_match_func_t func);

/*
Description: Get the iterator for the next element in the list. 
Params: (dll_iterator_t iter) 
Return Value: dll_iterator_t
Time Complexity: O(1)
Space: O(1)
*/
slist_iterator_t SListNext(slist_iterator_t iter);

/*
Description: Get the iterator for the previous element in the list
Params: (dll_iterator_t iter) 
Return Value: dll_iterator_t
Time Complexity: O(1)
Space: O(1)
*/
slist_iterator_t SListPrev(slist_iterator_t iter);

/*
Description: indication if list is empty 
Params: (list) ,
Return Value: boolean 1 for empty 0 for not
Time Complexity: O(1)
Space: O(1)
*/
int SListIsEmpty(const slist_t* list);

/*
Description: Get the first element iterator of the list
Params: (sdll_t* list) ,
Return Value: iterator for the first element. If list is empty, returns end of list.
Time Complexity: O(1)
Space: O(1)
*/
slist_iterator_t SListBegin(const slist_t* list);

/*
Description: Get invalid iterator, that follows the last valid element of the list.
Params: (sdll_t* list) ,
Return Value: iterator for invalid iterator. Represent the end of the list
Time Complexity: O(1)
Space: O(1)
*/
slist_iterator_t SListEnd(const slist_t* list);

/*
Description: Apply action_func_t func for each element from starting iterator to
the end iterator
Params: (sorted_list_t iter_start  iter_end) ,action_func_t the function
to preform on each element
Return Value: return iterator to the end when the function succeeded and iterator to where it failed if it failed
Time Complexity: O(n)
Space: O(1)
Note: passed func must keep the sorting algorithm
*/
slist_iterator_t SDLListForeach(slist_iterator_t start_iterator, slist_iterator_t end_iterator, void* data, slist_action_func_t func);

/*
Description: Checks if two iterators point to the same element
Params: sorted_list_t it1, sorted_list_t it2
Return Value: boolean value: 0 for not equal, !0 for equal
Time Complexity: O(1)
Space: O(1)
*/
int SListIsEqualIter(slist_iterator_t it1, slist_iterator_t it2);

/*
Description: Merges 2 sorted lists 
Params: 2 lists to merge. ...
Return Value: slist_t* pointer to the merged list (dest_list)
Time Complexity: O(n)
Space: O(1)
*/
slist_t* SDLListMerge(slist_t *dest_list, slist_t *src_list);

#endif /* __SORTED_LIST_H__ */
