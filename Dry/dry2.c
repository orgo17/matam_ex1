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

//Originl functions 
int getListLength(Node list); 
bool isListSorted(Node list); 
ErrorCode mergeSortedLists(Node list1, Node list2, Node *mergedOut);

//New helper functions
/*
    Input: list1 - sorted linked list
    list2 - sorted linked list
    Output: returns True if list1 value is bigger than list2 value, and False otherwise
*/
bool isList1Bigger(Node list1, Node list2);
/*
    Input: tmp - temporary Node used to sbuild mergedOut
    mergedOut - sorted linked list
    Output: appropriate error code
*/
ErrorCode createNextNode(Node tmp, Node *mergedOut);
/*
    Input: list - pointer to a linked list
    Function frees the memory allocated to the list
*/
void destroyList(Node *list);

//Function implementation:
ErrorCode mergeSortedLists(Node list1, Node list2, Node *mergedOut)
{   
    mergedOut = NULL;
    if(!list1 || !list2){
        return EMPTY_LIST;
    }
    if(!isListSorted(list1) || !isListSorted(list2))
    {
        return UNSORTED_LIST;
    }
    Node tmp = malloc(sizeof(Node));
    if(!tmp){
        return MEMORY_ERROR;
    }
    mergedOut = &tmp;
    while(list1->next && list2->next)//add numbers to mergedOut
    {
        if(isList1Bigger(list1, list2)){
            tmp->x = list1->x;
            list1 = list1->next;
        }
        else{
            tmp->x = list2->x;
            list2 = list2->next;
        }
        if(createNextNode(tmp,mergedOut) == MEMORY_ERROR){
            return MEMORY_ERROR;
        }
    }
    while(list1->next){//add remaining numbers from list1 to mergedOut
        tmp->x = list1->x;
        if(createNextNode(tmp,mergedOut) == MEMORY_ERROR){
            return MEMORY_ERROR;
        }
        list1 = list1->next;
    }
    while(list2->next){//add remaining numbers from list2 to mergedOut
        tmp->x = list2->x;
        if(createNextNode(tmp,mergedOut) == MEMORY_ERROR){
            return MEMORY_ERROR;
        }
        list2 = list2->next;
    }
    return SUCCESS;
}

void destroyList(Node *list)
{
    while(list)
    {
        Node to_delete = (*list);
        (*list) = (*list)->next;
        free(to_delete);
    }
    list = NULL;
}

bool isList1Bigger(Node list1, Node list2)
{
    return list1->x > list2->x;
}

ErrorCode createNextNode(Node tmp, Node *mergedOut)
{
    tmp->next = malloc(sizeof(Node));
    if(!(tmp->next)){
        destroyList(mergedOut);
        return MEMORY_ERROR;
    }
    else{
        return SUCCESS;
    }
}