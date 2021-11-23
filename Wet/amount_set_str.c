#include "amount_set_str.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define SIZE_FOR_NULL -1

struct AmountSet_t
{
    char *name;
    double amount;
    struct AmountSet_t *next;
    AmountSet iterator;
};

AmountSet asCreate()
{
    AmountSet created_amount_set = malloc(sizeof(*created_amount_set));
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

AmountSet asCopy(AmountSet set)
{
    AmountSet amount_set_copy = asCreate();
    if(amount_set_copy == NULL || set == NULL){
        return NULL;
    }
    amount_set_copy = copyElement(set);
    AmountSet p = amount_set_copy;
    asGetFirst(set);
    while(p)
    {
        AmountSet copy = copyElement(set->iterator);
        if(copy == NULL){
            asDestory(copy);
            return NULL;
        }
        asGetNext(set);
        p->next = copy;
        p = copy;
    }
    return amount_set_copy;
}

static AmountSet copyElement(AmountSet element)
{
    AmountSet copy = asCreate();
    if(copy == NULL || element == NULL){
        return NULL;
    }
    copy->name = element->name;
    copy->amount = element->amount;
    return copy;
}

int asGetSize(AmountSet set)
{
    if(!set){
        return SIZE_FOR_NULL;
    }
    int size = 0;
    AmountSet iterator_value_before = set->iterator;
    for(char* iterator = asGetFirst(set) ;iterator ;iterator = asGetNext(set))
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
    AmountSet iterator_value_before = set->iterator;
    for(char* iterator = asGetFirst(set) ;iterator ;iterator = asGetNext(set))
    {
        if(strcmp(element, set->iterator->name) == 0){
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
    set->iterator = set;
    return set->iterator->name;
}

char* asGetNext(AmountSet set)
{
    if(!set){
        return NULL;
    }
    set->iterator = set->next;
    if(!(set->iterator)){
        return NULL;
    }
    return set->iterator->name;
}

AmountSetResult asRegister(AmountSet set, const char* element)
{
    if(!set || !element){
        return AS_NULL_ARGUMENT;
    }
    if(asContains(set,element)){
        return AS_ITEM_ALREADY_EXISTS;
    }
    AmountSet element_to_add = asCreate();
    assert(element_to_add == NULL);
    element_to_add->name = element;
    AmountSet pointer_to_current = set;
    if(strcmp(pointer_to_current->name, element)<0){
        element_to_add->next = set;
        set = element_to_add;
        return AS_SUCCESS;
    }
    while(pointer_to_current->next && strcmp(pointer_to_current->next->name, element)<0)
    {
        pointer_to_current = pointer_to_current->next;
    }
    element_to_add->next = pointer_to_current->next;
    pointer_to_current->next = element_to_add;
    return AS_SUCCESS;
}

