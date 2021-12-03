#include <stdbool.h>
#include <stdlib.h>
#define RETURN_IF_NULL(pointer, return_value)\
        if(pointer == NULL){\
            return return_value;\
        }\

#define CREATE_NEXT_AND_ADVANCE\
        if(createNextNode(tmp,mergedOut) == MEMORY_ERROR){\
            return MEMORY_ERROR;\
        }\
        tmp = tmp->next;\

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

//Original functions 
int getListLength(Node list); 
bool isListSorted(Node list); 
ErrorCode mergeSortedLists(Node list1, Node list2, Node *mergedOut);

//New helper functions
/**
 * check if Node list1 integer is bigger then Node list2 integer
 * 
 * @param list1 - first list to compare
 * @param list2 - second list to compare
 * @return
 * true if Node list1 integer is bigger
 * false if Node list2 integer is bigger
 */
bool isList1Bigger(Node list1, Node list2);

/**
 * @brief Create a Next Node for mergedOut
 * 
 * @param tmp - Last node in mergedOut
 * @param mergedOut - List that will contain merge of two sorted lists
 * @return 
 * MEMORY_ERROR in case of malloc error
 * SUCCESS if the process was successful
 */
ErrorCode createNextNode(Node tmp, Node *mergedOut);

/**
 * @brief Frees the data allocated to list
 * 
 * @param list - list to be destroyed 
 */
void destroyList(Node *list);

/**
 * Adds the remaining numbers in list to merged 
 * 
 * @param list to add Nodes from
 * @param tmp Node we want to insert the number into
 * @param mergedOut The list that will contain the two merged lists
 * @return
 * MEMORY_ERROR in case of malloc error
 * SUCCESS if the process was successful
 */
ErrorCode addRemainingToMerged(Node list, Node tmp, Node *mergedOut);

//Function implementation:
ErrorCode mergeSortedLists(Node list1, Node list2, Node *mergedOut)
{   
    if(!list1 || !list2){
        return EMPTY_LIST;
    }
    if(!isListSorted(list1) || !isListSorted(list2))
    {
        return UNSORTED_LIST;
    }
    *mergedOut = malloc(sizeof(Node));
    RETURN_IF_NULL(*mergedOut, MEMORY_ERROR);
    Node tmp = *mergedOut;
    while(list1 && list2)//add numbers to mergedOut
    {
        if(!isList1Bigger(list1, list2)){
            tmp->x = list1->x;
            list1 = list1->next;
        }
        else{
            tmp->x = list2->x;
            list2 = list2->next;
        }
        CREATE_NEXT_AND_ADVANCE;
    }
    if(addRemainingToMerged(list1, tmp, mergedOut) == MEMORY_ERROR || 
        addRemainingToMerged(list2, tmp, mergedOut) == MEMORY_ERROR){
        return MEMORY_ERROR;
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

ErrorCode addRemainingToMerged(Node list, Node tmp, Node *mergedOut)
{
    while(list){ 
        tmp->x = list->x;
        if((list->next) != NULL)
        {
            CREATE_NEXT_AND_ADVANCE;
        }
        list = list->next;
    }
    return SUCCESS;
}