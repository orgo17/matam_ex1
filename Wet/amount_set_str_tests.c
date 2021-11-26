#include "amount_set_str.h"
#include "amount_set_str_tests.h"
#include <string.h>
#include <stdlib.h>
#define NAMES 3
#define DEMO_AMOUNT 10
#define NULL_POINTER -1

static AmountSet amountSetCreateDemo()
{
    AmountSet demo = asCreate();
    if(demo == NULL){
        return NULL;
    }
    char *names[NAMES] = {"rice","mlik","bread"};
    for(int i = 0; i < NAMES; i++)
    {
        asRegister(demo, names[i]);
        asChangeAmount(demo, names[i], DEMO_AMOUNT);
    }
    return demo;
}

bool testAmountSetCreateAndDestory()
{
    AmountSet created_set = asCreate();
    if(created_set == NULL){
        return false;
    }
    asDestroy(created_set);
    return true;
}

bool testAsCopy(AmountSet set)
{
    AmountSet demo = asCreate();
    AmountSet copy = asCopy(demo);
    bool are_equal = false;
    asCompare(demo, copy, &are_equal);
    asDestroy(demo);
    asDestroy(copy);
    return are_equal;
}
bool testAsCopyEmptySet(AmountSet set)
{
    AmountSet demo = amountSetCreateDemo();
    AmountSet copy = asCopy(demo);
    bool are_equal = false;
    asCompare(demo, copy, &are_equal);
    asDestroy(demo);
    asDestroy(copy);
    return are_equal;
}
bool testAsCopyNull()
{
    AmountSet demo = NULL;
    AmountSet copy = listCopy(demo);
    return copy == NULL;
}

bool testAsGetSize()
{
    AmountSet demo = asCreate();
    int size = asGetSize(demo);
    asDestroy(demo);
    return size == NAMES;
}
bool testAsGetSizeEmpty()
{
    AmountSet demo = asCreate();
    int size = asGetSize(demo);
    asDestroy(demo);
    return size == 0;
}
bool testAsGetSizeNull()
{
    AmountSet demo = NULL;
    int size = asGetSize(demo);
    return size == NULL_POINTER;
}

bool testAsContains()
{
    AmountSet demo = amountSetCreateDemo();
    char* name = "milk";
    bool contains = asContains(demo, name);
    asDestroy(demo);
    return contains;
}
bool testAsContainsEmpty()
{
    AmountSet demo = asCreate();
    char* name = "milk";
    bool contains = asContains(demo, name);
    asDestroy(demo);
    return !contains;
}
bool testAsContainsNull()
{
    AmountSet demo = NULL;
    char* name = "milk";
    bool contains = asContains(demo, name);
    return !contains;
}

bool testAsGetAmount()
{
    AmountSet demo = amountSetCreateDemo();
    char* name = "milk";
    double amount = 0;
    if(asGetAmount(demo, name, &amount) != AS_SUCCESS){
        return false;
    }
    bool result = (amount == DEMO_AMOUNT);
    asDestroy(demo);
    return result;
}