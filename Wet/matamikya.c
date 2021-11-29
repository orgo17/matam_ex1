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
    double next_order_id;
};

Matamikya matamikyaCreate()
{
    Matamikya matamikya = malloc(sizeof(*matamikya));
    if(matamikya == NULL){
        return NULL;
    }
    matamikya->storage = NULL;
    matamikya->orders = NULL;
    matamikya->next_order_id = 1;
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

MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice);

