#include "amount_set_str.h"
#include "list_str.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define SIZE_FOR_NULL -1

struct AmountSet_t
{
    List items;
    List iterator;
};

AmountSet asCreate()
{
    AmountSet set = malloc(sizeof(*set));
    if(set == NULL){
        return NULL;
    }
    set->items = listCreate();
    if(set->items == NULL){
        return NULL;
    }
    set->iterator = set->items;
    return set;
}

void asDestroy(AmountSet set)
{
    listDestroy(set->items);
    free(set);   
}

static bool isAmountSetEmpty(AmountSet set)
{
    if(!set){
        return true;
    }
    if(listGetNext(set->items) == NULL){
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
    amount_set_copy->items = listCopy(set->items);
    return amount_set_copy;
}

int asGetSize(AmountSet set)
{
    if(!set){
        return SIZE_FOR_NULL;
    }
    return listGetSize(set->items);
}

bool asContains(AmountSet set, const char* element)
{
    if(!set){
        return false;
    }
    List iterator_value_before = set->iterator;
    AS_FOREACH(char*, i, set){
        if(strcmp(element, i) == 0){
            set->iterator = iterator_value_before;
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
    List iterator_value_before = set->iterator;
    AS_FOREACH(char*, i, set){
        if(strcmp(element, i) == 0){
            *outAmount = listReturnAmountOfElement(set->iterator);
            set->iterator = iterator_value_before;
            return AS_SUCCESS;
        }
    }
}

char* asGetFirst(AmountSet set)
{
    if(!set || isAmountSetEmpty(set)){
        return NULL;
    }
    set->iterator = listGetNext(set->items);
    return listReturnNameOfElement(set->iterator);
}

char* asGetNext(AmountSet set)
{
    if(!set){
        return NULL;
    }
    set->iterator = listGetNext(set->iterator);
    if(!(set->iterator)){
        return NULL;
    }
    return listGetName(set->iterator);
}

AmountSetResult asRegister(AmountSet set, const char* element)
{
    if(!set || !element || !set->items){
        return AS_NULL_ARGUMENT;
    }
    if(asContains(set,element)){
        return AS_ITEM_ALREADY_EXISTS;
    }
    ListResult insert_result = listInsertLexicographic(set->items, element);
    assert(insert_result != LIST_OUT_OF_MEMORY);
    return AS_SUCCESS;
}

AmountSetResult asChangeAmount(AmountSet set, const char* element, double amount)
{
    if(set == NULL || set->items == NULL){
        return AS_NULL_ARGUMENT;
    }
    if(!asContains(set,element)){
        return AS_ITEM_DOES_NOT_EXIST;
    }
    double item_amount;
    asGetAmount(set, element, &item_amount);
    if(item_amount + amount < 0){
        return AS_INSUFFICIENT_AMOUNT;
    }
    listSetAmountOfElement(set->items, item_amount + amount, element);
    return AS_SUCCESS;
}

AmountSetResult asCompare(AmountSet set1, AmountSet set2, bool *result)
{
    if(set1 == NULL || set2 == NULL){
        return AS_NULL_ARGUMENT;
    }
    if(listCompare(set1->items, set2->items, result) == LIST_NULL_ARGUMENT){
        return AS_NULL_ARGUMENT;
    }
    if(listCompare(set1->iterator, set2->iterator, result) == LIST_NULL_ARGUMENT){
        return AS_NULL_ARGUMENT;
    }
    return AS_SUCCESS;
}


