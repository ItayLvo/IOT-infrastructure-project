#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <stddef.h> /* size_t */

typedef struct linked_list linked_list_t;
typedef struct node *iterator_t;

typedef int (*action_func_t)(void*);
typedef int (*comp_func_t)(void*, void*);

/*
Description: 
Params: (void) ,
Return Value: 
Time Complexity: O(1)
Space: O(n)
*/
linked_list_t *SLListCreate(void);

/*
Description: frees the list and all of its nodes.
Params: (list_t*) list
Return Value: void
Time Complexity: O(n)
Space: O(0)
*/
void SLListDestroy(linked_list_t *list);

/*
Description: Creates a new node and adds it to the LList
Params: (list_t) list, (void *) data to add to list
Return Value: (iterator_t) iterator pointing at last inserted node
Errors: new node malloc failed, 
Time Complexity: O(1)
Space: O(n)
Notes: this functionality allows convenient linking of several nodes
*/
iterator_t SLListInsert(linked_list_t *list, void *data);

/*
Description: 
Params: (iterator) ,
Return Value: 
Time Complexity: O(1)
Space: O()
*/
void SLListRemove(iterator_t iterator);

/*
Description: 
Params: (linked list) ,
Return Value: 
Time Complexity: O(n)
Space: O(1)
*/
size_t SLListCount(const linked_list_t *linked_list);

/*
Description: 
Params: (iterator and data) ,
Return Value: 
Time Complexity: O(1)
Space: O()
*/
void SLListSetData(iterator_t iterator, void* data);

/*
Description: 
Params: (iterator) ,
Return Value: 
Time Complexity: O(1)
Space: O(1)
*/
void *SLListGetData(const iterator_t iterator);

/*
Description: 
Params: (start and end iterators and ) ,
Return Value: 
Time Complexity: O(n)
Space: O(1)
*/
iterator_t SLListFind(iterator_t iter_start, iterator_t iter_end, const void* data, const comp_func func);

/*
Description: 
Params: () ,
Return Value: 
Time Complexity: O()
Space: O()
*/
iterator_t SLListNext(iterator_t iter);

/*
Description: 
Params: (list) ,
Return Value: 
Time Complexity: O()
Space: O()
*/
int SLListIsEmpty(const linked_list_t* list);

/*
Description: 
Params: () ,
Return Value: 
Time Complexity: O()
Space: O()
*/
iterator_t SLListGetBegin(linked_list_t* list);

/*
Description: 
Params: () ,
Return Value: 
Time Complexity: O()
Space: O()
*/
iterator_t SLListGetEnd(linked_list_t* list);

/*
Description: 
Params: () ,
Return Value: 
Time Complexity: O()
Space: O()
*/
void SLListForEach(iterator_t iter_start, iterator_t iter_end, action_func func);

/*
Description: 
Params: () ,
Return Value: 
Time Complexity: O()
Space: O()
*/
int SLListIsEqual(iterator_t it1, iterator_t it2);

#endif /* __LINKED_LIST_H__ */
