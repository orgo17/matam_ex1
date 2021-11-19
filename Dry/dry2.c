#include <stdbool.h>
#include <stdlib.h>

typedef struct node_t { 
    int x; 
    struct node_t *next; 
} *Node; 
 
typedef enum { 
    SUCCESS=0, 
    MEMORY_ERROR, 
    EMPTY_LIST, 
    UNSORTED_LIST, 
    NULL_ARGUMENT, 
} ErrorCode; 
 
int getListLength(Node list); 
bool isListSorted(Node list); 
ErrorCode mergeSortedLists(Node list1, Node list2, Node *mergedOut);

bool isList1Bigger(Node num_list1, Node num_list2);
ErrorCode createNext(Node tmp);

//Function implementation:
ErrorCode mergeSortedLists(Node list1, Node list2, Node *mergedOut)
{   
    if(!list1 || !list2){
        mergedOut = NULL;
        return EMPTY_LIST;
    }
    if(!isListSorted(list1) || !isListSorted(list2))
    {
        mergedOut = NULL;
        return UNSORTED_LIST;
    }
    Node tmp = malloc(sizeof(Node));
    if(!tmp){
        mergedOut = NULL;
        return MEMORY_ERROR;
    }
    mergedOut = &tmp;
    while(list1->next && list2->next)
    {
        if(isList1Bigger(list1, list2)){
            tmp->x = list1->x;
        }
        else{
            tmp->x = list2->x;
        }
        if(createNext(tmp) == MEMORY_ERROR){
            mergedOut = NULL;
            return MEMORY_ERROR;
        }
    }
    while(list1->next){
        tmp->x = list1->x;
        if(createNext(tmp) == MEMORY_ERROR){
            mergedOut = NULL;
            return MEMORY_ERROR;
        }
    }
    while(list2->next){
        tmp->x = list2->x;
        if(createNext(tmp) == MEMORY_ERROR){
            mergedOut = NULL;
            return MEMORY_ERROR;
        }
    }
    return SUCCESS;
}

bool isList1Bigger(Node num_list1, Node num_list2)
{
    return num_list1->x > num_list2->x;
}

ErrorCode createNext(Node tmp)
{
    tmp->next = malloc(sizeof(Node));
    if(!(tmp->next)){
        return MEMORY_ERROR;
    }
    else{
        return SUCCESS;
    }
}