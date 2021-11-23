#include "list_str.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define NULL_POINTER -1

struct List_t
{
    char *name;
    double amount;
    struct List_t *next;
};

List listCreate()
{
	List new_list = malloc(sizeof(*new_list));
	if(!new_list) {
		return NULL;
	}
	new_list->name = NULL;
	new_list->amount = 0;
	return new_list;
}

void listDestroy(List list)
{
	while(list) {
		List to_delete = list;
		list = list->next;
		free(to_delete);
	}
}

static List copyElement(List element)
{
    List copy = listCreate();
    if(copy == NULL || element == NULL){
        return NULL;
    }
    copy->name = element->name;
    copy->amount = element->amount;
    return copy;
}

List listCopy(List list)
{
    if(list == NULL){
        return NULL;
    }
    List list_copy = copyElement(list);
    if(list_copy == NULL){
        return NULL;
    }
    List list_tmp = list;
    List list_copy_tmp = list_copy;
    while(list_tmp){
        list_copy_tmp->next = copyElement(list_tmp->next);
        if(list_copy_tmp->next == NULL){
            listDestroy(list_copy);
            return NULL;
        }
        list_copy_tmp = list_copy_tmp->next;
        list_tmp = list_tmp->next;
    }
    return list_copy;
}

int listGetSize(List list)
{
    if(!list){
        return NULL_POINTER;
    }
    int size = 0;
    List copy = list;
    while(copy){
        size++;
        copy = copy->next;
    }
    return size;
}


ListResult listRemoveElement(List list, const char* element)
{
    if(!list){
        return LIST_NULL_ARGUMENT;
    }
    List copy = list, prev;
    while(!copy && strcmp(copy->name, element)!=0){
        prev = copy;
        copy = copy->next;
    }
    prev->next = copy->next;
    free(copy);
    return LIST_SUCCESS;
}

ListResult listInsertLexicographic(List list, char* element)
{
    if(!list){
        return LIST_NULL_ARGUMENT;
    }
    List new_element = listCreate();
    if(!new_element){
        return LIST_OUT_OF_MEMORY;
    }
    new_element->name = element;
    List copy = list, prev;
    while(!copy && strcmp(copy->name, element)<0){
        prev = copy;
        copy = copy->next;
    }
    prev->next = new_element;
    new_element->next = copy->next;
    return LIST_SUCCESS;
}

ListResult listClear(List list)
{
    if(list == NULL){
        return LIST_NULL_ARGUMENT;
    }
    listDestroy(list->next);
    list->name = NULL;
    list->amount = 0;
    list->next = NULL;
    return LIST_SUCCESS;
}
