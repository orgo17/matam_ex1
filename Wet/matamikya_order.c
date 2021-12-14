#include "matamikya_order.h"
#include <stdlib.h>
#include <string.h>

struct order_t
{
    unsigned int id;
    AmountSet products_ids;
};

ASElement copyProductId(ASElement product_id)
{
    int *product_id_copy = malloc(sizeof(*product_id_copy));
    RETURN_IF_NULL(product_id_copy, NULL);
    *product_id_copy = *(int*)product_id;
    return (ASElement)product_id_copy;
}

void freeProductId(ASElement product_id)
{
    free(product_id);
}

int compareProductId(ASElement product_id1, ASElement product_id2)
{
    return (*(unsigned int*)product_id1) - (*(unsigned int*)product_id2);
}

SetElement createOrder(const unsigned int id, CopyASElement product_id_copy_function, 
                        FreeASElement product_id_free_function, CompareASElements product_id_compare_function)
{
    Order order = malloc(sizeof(*order));
    RETURN_IF_NULL(order, NULL);
    order->id = id;
    order->products_ids = asCreate(product_id_copy_function, product_id_free_function,
                                    product_id_compare_function);
    if(!order->products_ids){
        free(order);
        return NULL;
    }
    return (SetElement)order;
}

SetElement copyOrder(SetElement order)
{
    Order order_copy = malloc(sizeof(*order_copy));
    RETURN_IF_NULL(order_copy, NULL);
    order_copy->id = ((Order)order)->id;
    order_copy->products_ids = asCopy(((Order)order)->products_ids);
    if(order_copy->products_ids == NULL){
        free(order_copy);
        return NULL;
    }
    return (SetElement)order_copy;
}

void freeOrder(SetElement order)
{
    if(order != NULL){
        asDestroy(((Order)order)->products_ids);
    }
    free(order);
}

int compareOrder(SetElement order1, SetElement order2)
{
    return ((Order)order1)->id - ((Order)order2)->id;
}

unsigned int getOrderId(SetElement order)
{
    return ((Order)order)->id;
}

AmountSet getOrderProductIds(SetElement order)
{
    return ((Order)order)->products_ids;
}
