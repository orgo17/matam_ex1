#include "matamikya_product.h"
#include <stdlib.h>
#include <string.h>

struct product_t
{
    char* name;
    unsigned int id;
    MtmProductData product_data;
    MtmCopyData product_data_copy_function;
    MtmFreeData product_data_free_function;
    MtmGetProductPrice product_data_get_price;
    MatamikyaAmountType amount_type;
};

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

unsigned int getProductId(ASElement product)
{
    return ((Product)product)->id;
}

void setProductId(ASElement product, unsigned int id)
{
    ((Product)product)->id = id;
}

char* getProductName(ASElement product)
{
    return ((Product)product)->name;
}

MatamikyaAmountType getProductAmountType(ASElement product)
{
    return ((Product)product)->amount_type;
}

double calculateProductProfit(ASElement product, double amount)
{
    return ((Product)product)->product_data_get_price(((Product)product)->product_data, amount);
}