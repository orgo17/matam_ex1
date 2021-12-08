#include "matamikya_help.h"
#include <string.h>

ASElement createProduct(const char* name, const unsigned int id, 
                        MtmProductData product_data, MatamikyaAmountType amount_type,
                        MtmCopyData copy_function, MtmFreeData free_function,
                        MtmGetProductPrice price_function)
{
    Product product = malloc(sizeof(*product));
    RETURN_IF_NULL(product, NULL);
    product->name = malloc(strlen(name)+1);
    if(product->name == NULL){
        free(product);
        return NULL;
    }
    strcpy(product->name, name);
    product->id = id;
    product->product_data = copy_function(product_data);
    if(product->product_data == NULL){
        free(product->name);
        free(product);
        return NULL;
    }
    product->product_data_copy_function = copy_function;
    product->product_data_free_function = free_function;
    product->product_data_get_price = price_function;
    product->amount_type = amount_type;
    return (ASElement)product;
}

ASElement copyProduct(ASElement product)
{
    Product product_copy = malloc(sizeof(*product_copy));
    RETURN_IF_NULL(product_copy, NULL);
    product_copy->name = malloc(strlen(((Product)product)->name)+1);
    if(product_copy->name == NULL){
        free(product);
        return NULL;
    }
    strcpy(product_copy->name, ((Product)product)->name);
    product_copy->id = ((Product)product)->id;
    product_copy->product_data = ((Product)product)->
                                product_data_copy_function(((Product)product)->product_data);
    if(product_copy->product_data == NULL){
        free(product_copy->name);
        free(product_copy);
        return NULL;
    }
    product_copy->product_data_copy_function = ((Product)product)->product_data_copy_function;
    product_copy->product_data_free_function = ((Product)product)->product_data_free_function;
    product_copy->product_data_get_price = ((Product)product)->product_data_get_price;
    product_copy->amount_type = ((Product)product)->amount_type;
    return (ASElement)product_copy;
}

void freeProduct(ASElement product)
{
    free(((Product)product)->name);
    ((Product)product)->product_data_free_function(((Product)product)->product_data);
    free(product);
}

int compareProduct(ASElement product1, ASElement product2)
{
    return ((Product)product1)->id - ((Product)product2)->id;
}

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