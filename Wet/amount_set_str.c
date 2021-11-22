#include "amount_set_str.h"
#include <stdlib.h>
#include <string.h>
#define SIZE_FOR_NULL -1

struct AmountSet_t
{
    char *name;
    double amount;
    struct AmountSet_t *next;
    char *iterator;
};

AmountSet asCreate()
{
    AmountSet created_amount_set = malloc(sizeof(AmountSet));
    if(created_amount_set == NULL){
        return NULL;
    }
    created_amount_set->name = NULL;
    created_amount_set->amount = 0;
    created_amount_set->next = NULL;
    created_amount_set->iterator = NULL;
    return created_amount_set;
}

void asDestroy(AmountSet set)
{
    while (set)
    {
        AmountSet next = set->next;
        free(set);
        set = next;
    }
}

static bool isAmountSetEmpty(AmountSet set)
{
    if(!set){
        return true;
    }
    if(set->name == NULL){
        return true;
    }
    return false;
}

// AmountSet asCopy(AmountSet set)
// {
//     AmountSet copy = asCreate();
//     if(copy == NULL || set == NULL){
//         asDestroy(copy);
//         return NULL;
//     }
//     while (set)
//     {
        
//     }
    
// }

// static AmountSet copyNode(AmountSet set)
// {
//     AmountSet copy = asCreate();
//     if(copy == NULL || set == NULL){
//         asDestroy(copy);
//         return NULL;
//     }
//     copy->name = set->name;
//     copy->amount = set->amount;
//     return copy;
// }

int asGetSize(AmountSet set)
{
    if(!set){
        return SIZE_FOR_NULL;
    }
    int size = 0;
    char* iterator_value_before = set->iterator;
    AS_FOREACH(set->iterator, set)
    {
        size++;
    }
    set->iterator = iterator_value_before;
    return size;
}

bool asContains(AmountSet set, const char* element)
{
    if(!set){
        return false;
    }
    char* iterator_value_before = set->iterator;
    AS_FOREACH(set->iterator, set)
    {
        if(strcmp(element, set->iterator) == 0){
            return true;
        }
    }
    set->iterator = iterator_value_before;
    return false;
}

AmountSetResult asGetAmount(AmountSet set, const char* element, double* outAmount)
{
    if(!set){
        return AS_NULL_ARGUMENT;
    }
    if(!asContains(set,element)){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    AmountSet pointer_to_start = set;
    while(pointer_to_start && strcmp(pointer_to_start->name, element) != 0){
        pointer_to_start = pointer_to_start->next;
    }
    *outAmount = pointer_to_start->amount;
    return AS_SUCCESS;
}

char* asGetFirst(AmountSet set)
{
    if(!set || isAmountSetEmpty(set)){
        return NULL;
    }
    set->iterator = set->name;
    return set->iterator;
}

char* asGetNext(AmountSet set)
{
    if(!set){
        return NULL;
    }
    set->iterator = set->next->name;
    if(!(set->iterator)){
        return NULL;
    }
    return set->iterator;
}

AmountSetResult asRegister(AmountSet set, const char* element)
{
    if(!set){
        return AS_NULL_ARGUMENT;
    }
    if(asContains(set,element)){
        return AS_ITEM_ALREADY_EXISTS;
    }
    AmountSet element_to_add = asCreate();
    element_to_add->name = element;
    AmountSet pointer_to_start = set;
    while(pointer_to_start->next && strcmp(pointer_to_start->next->name, element)<0)
    {
        pointer_to_start = pointer_to_start->next;
    }
}

