#include <stdio.h>
#include <stdarg.h>
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

int getListLength(Node list);
bool isListSorted(Node list);
ErrorCode mergeSortedLists(Node list1, Node list2, Node *mergedOut);

// ========================= enter you functions here =========================

bool isList1Bigger(Node list1, Node list2)
{
    return list1->x > list2->x;
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

// ========================= enter you functions here =========================


bool isListSorted(Node list){
    while (list->next){
        if (list->x > list->next->x){
            return false;
        }
        list = list->next;
    }
    return true;
}

Node createList(int count, ...){
    va_list v_list;
    Node list = malloc(sizeof(*list));
    Node temp = list;
    va_start(v_list,count);
    temp->x = va_arg(v_list,int);
    temp->next = NULL;
    for (int i = 1; i < count; ++i)
    {
        temp->next = malloc(sizeof(*temp));
        temp = temp->next;
        temp->x = va_arg(v_list,int);
        temp->next = NULL;
    }
    va_end(v_list);

    return list;
}

void printList(Node list){
    while(list){
        printf("%d ", list->x);
        list = list->next;
    }
    puts("");
}

int main()
{
    // test example 1
    Node list1 = createList(3,1,3,5);
    Node list2 = createList(3,2,4,6);
    Node merged;
    ErrorCode error = mergeSortedLists(list1,list2, &merged);
    printList(list1);
    printList(list2);
    printList(merged);
    printf("%d\n",error);

    // test example 2
    list1 = createList(3,3,8,3);
    list2 = createList(2,2,2);
    error = mergeSortedLists(list1,list2, &merged);
    printList(list1);
    printList(list2);
    printList(merged);
    printf("%d\n",error);

    return 0;
}