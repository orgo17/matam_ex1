#ifndef LIST_STR_H_
#define LIST_STR_H_

#include <stdbool.h>

/** Type for defining the list */
typedef struct List_t *List;

/** Type used for returning error codes from list functions */
typedef enum ListResult_t {
	LIST_SUCCESS,
	LIST_NULL_ARGUMENT,
	LIST_OUT_OF_MEMORY,
} ListResult;

/**
* Allocates a new List.
*
* Creates a new empty list.
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new List in case of success.
*/
List listCreate();

/**
* listDestroy: Deallocates an existing list. Clears all elements by using the
* stored free function.
*
* @param list Target list to be deallocated. If list is NULL nothing will be
* done
*/
void listDestroy(List list);

/**
* Creates a copy of target list.
*
* The new copy will contain all the elements from the source list in the same
* order.
*
* @param list The target list to copy
* @return
* NULL if a NULL was sent or a memory allocation failed.
* A List containing the same elements with same order as list otherwise.
*/
List listCopy(List list);

/**
* Returns the number of elements in a list
*
* @param list The target list which size is requested.
* @return
* -1 if a NULL pointer was sent.
* Otherwise the number of elements in the list.
*/
int listGetSize(List list);

/**
* Adds a new element to the list with amount 0, the new element will inserted in a way that keeps
* the list sorted in a lexicographic order.
*
* @param list The list for which to add an element in its start
* @param element The element to insert. A copy of the element will be inserted
* @return
* LIST_NULL_ARGUMENT if a NULL was sent as list
* LIST_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* an element failed)
* LIST_SUCCESS the element has been inserted successfully
*/
ListResult listInsertLexicographic(List list, char* element);

/**
* Removes the given element from the list
*
* @param list The list for which the current element will be removed
* @return
* LIST_NULL_ARGUMENT if list is NULL
* LIST_SUCCESS the current element was removed successfully
*/
ListResult listRemoveElement(List list, const char* element);

/**
* Removes all elements from target list.
*
* All of the elements are deallocated.
* @param list Target list to remove all element from
* @return
* LIST_NULL_ARGUMENT - if a NULL pointer was sent.
* LIST_SUCCESS - Otherwise.
*/
ListResult listClear(List list);

#endif /* LIST_STR_H_ */
