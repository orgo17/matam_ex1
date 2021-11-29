#include "amount_set.h"
#include "matamikya.h"
#include "set.h"
#include <stdlib.h>
#include <string.h>

typedef struct product_t
{
    char* name;
    unsigned int id;
    MtmProductData product_data;
    MatamikyaAmountType amount_type;
}*Product;

static Product createProduct(const char* name, const unsigned int id, 
                            MtmProductData product_data, MatamikyaAmountType amount_type,
                            MtmCopyData copy_function)
{
    Product product = malloc(sizeof(*product));
    if(product == NULL){
        return NULL;
    }
    strcpy(product->name, name);
    product->id = id;
    product->product_data = copy_function(product_data);
    product->amount_type = amount_type;
    return product;
}

static Product copyProduct(Product product, MtmCopyData copy_function)
{
    Product product_copy = malloc(sizeof(*product_copy));
    if(product_copy == NULL){
        return NULL;
    }
    product_copy->name = malloc(sizeof(*(product->name)));
    if(product_copy->name == NULL){
        return NULL;
    }
    strcpy(product_copy->name, product->name);
    product_copy->id = product->id;
    product_copy->product_data = copy_function(product->product_data);
    if(product_copy->product_data == NULL){
        return NULL;
    }
    product_copy->amount_type = product->amount_type;
}

static void freeProduct(Product product, MtmFreeData free_function)
{
    free(product->name);
    free_function(product->product_data);
    free(product);
}

static int compareProduct(Product product1, Product product2)
{
    return product1->id - product2->id;
}

typedef Product (*ProductCopyData)(Product, MtmCopyData);
typedef void (*ProductFreeData)(Product, MtmFreeData);
typedef int (*ProductCompareData)(Product, Product);

typedef struct order_t
{
    unsigned int id;
    AmountSet products;
}*Order;

static Order createOrder(const unsigned int id, ProductCopyData product_copy_function, 
                        ProductFreeData product_free_function, ProductCompareData product_compare_function)
{
    Order order = malloc(sizeof(*order));
    if(order == NULL){
        return NULL;
    }
    order->id = id;
    order->products = asCreate(product_copy_function, product_free_function, product_compare_function);
    return order;
}

static Order copyOrder(Order order)
{
    Order order_copy = malloc(sizeof(*order_copy));
    if(order_copy == NULL){
        return NULL;
    } 
    order_copy->id = order->id;
    order_copy->products = asCopy(order->products);
    if(order->products == NULL){
        return NULL;
    }
    return order_copy;
}

static void freeOrder(Order order)
{
    asDestroy(order->products);
    free(order);
}

static int compareOrder(Order order1, Order order2)
{
    return order1->id - order2->id;
}

struct Matamikya_t
{
    AmountSet storage;
    Set orders;
    AmountSet profit;
    unsigned int next_order_id;
};

Matamikya matamikyaCreate()
{
    Matamikya matamikya = malloc(sizeof(*matamikya));
    if(matamikya == NULL){
        return NULL;
    }
    matamikya->storage = NULL;
    matamikya->orders = NULL;
    matamikya->profit = NULL;
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
    asDestroy(matamikya->profit);
    free(matamikya);
}

MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
    if(matamikya == NULL){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if(matamikya->storage == NULL){
        matamikya->storage = asCreate()
    }
}


unsigned int mtmCreateNewOrder(Matamikya matamikya)
{
    
}