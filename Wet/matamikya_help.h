#ifndef MATAMIKYA_HELP_H_
#define MATAMIKYA_HELP_H_

#include "matamikya.h"
#include "amount_set.h"
#include "set.h"
#include <stdlib.h>

typedef struct product_t
{
    char* name;
    unsigned int id;
    MtmProductData product_data;
    MtmCopyData product_data_copy_function;
    MtmFreeData product_data_free_function;
    MtmGetProductPrice product_data_get_price;
    MatamikyaAmountType amount_type;
}*Product;

typedef struct order_t
{
    unsigned int id;
    AmountSet products_ids;
}*Order;


ASElement createProduct(const char* name, const unsigned int id, 
                        MtmProductData product_data, MatamikyaAmountType amount_type,
                        MtmCopyData copy_function, MtmFreeData free_function,
                        MtmGetProductPrice price_function);
ASElement copyProduct(ASElement product);
void freeProduct(ASElement product);
int compareProduct(ASElement product1, ASElement product2);
MatamikyaAmountType getProductAmountType(ASElement product);
ASElement copyProductId(ASElement product_id);
void freeProductId(ASElement product_id);
int compareProductId(ASElement product_id1, ASElement product_id2);
SetElement createOrder(const unsigned int id, CopyASElement product_id_copy_function, 
                    FreeASElement product_id_free_function, CompareASElements product_id_compare_function);
SetElement copyOrder(SetElement order);
void freeOrder(SetElement order);
int compareOrder(SetElement order1, SetElement order2);


#define RETURN_IF_NULL(pointer, return_value)\
        if(pointer == NULL){\
            return return_value;\
        }\

#endif /* MATAMIKYA_HELP_H_ */