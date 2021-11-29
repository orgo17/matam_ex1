#include "amount_set.h"
#include "matamikya.h"
#include "set.h"
#include <stdlib.h>
#include <string.h>

typedef struct product_t
{
    const char* name;
    const unsigned int id;
    double profit;
    MtmProductData product_data;
    MatamikyaAmountType amount_type;
}Product;

typedef struct order_t
{
    const unsigned int id;
    AmountSet products;
}Order;

struct Matamikya_t
{
    AmountSet storage;
    Set orders;
};

Matamikya matamikyaCreate()
{
    Matamikya matamikya = malloc(sizeof(*matamikya));
    if(matamikya == NULL){
        return NULL;
    }
    matamikya->storage = NULL;
    matamikya->orders = NULL;
    return matamikya;
}

void matamikyaDestroy(Matamikya matamikya)
{
    if(matamikya == NULL){
        return;
    }
    asDestroy(matamikya->storage);
    setDestroy(matamikya->orders);
    free(matamikya);
}

