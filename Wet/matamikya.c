#include "amount_set.h"
#include "matamikya.h"
#include "set.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define CAPITAl_A 'A'
#define CAPITAL_Z 'Z'
#define SMALL_A 'a'
#define SMALL_Z 'z'
#define SMALLEST_DIGIT '0'
#define BIGGEST_DIGIT '9'
#define INTEGER_AMOUNT_DEVIATION 0.001
#define RETURN_IF_NULL(pointer, return_value)\
        if(pointer == NULL){\
            return return_value;\
        }\

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

ASElement createProduct(const char* name, const unsigned int id, 
                        MtmProductData product_data, MatamikyaAmountType amount_type,
                        MtmCopyData copy_function, MtmFreeData free_function,
                        MtmGetProductPrice price_function)
{
    Product product = malloc(sizeof(*product));
    RETURN_IF_NULL(product, NULL);
    product->name = malloc(sizeof(*name));
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
    product_copy->name = malloc(sizeof(*(((Product)product)->name)));
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

static void freeProduct(ASElement product)
{
    free(((Product)product)->name);
    ((Product)product)->product_data_free_function(((Product)product)->product_data);
    free(product);
}

static int compareProduct(ASElement product1, ASElement product2)
{
    return ((Product)product1)->id - ((Product)product2)->id;
}

typedef struct order_t
{
    unsigned int id;
    AmountSet products_ids;
}*Order;

static ASElement copyProductId(ASElement product_id)
{
    unsigned int *product_id = malloc(sizeof(product_id));
    RETURN_IF_NULL(product_id, NULL);
    return (ASElement)product_id;
}
static void freeProductId(ASElement product_id)
{
    free(product_id);
}
static int compareProductId(ASElement product_id1, ASElement product_id2)
{
    return ((unsigned int*)product_id1) - ((unsigned int*)product_id2);
}

static SetElement createOrder(const unsigned int id, CopyASElement product_id_copy_function, 
                        FreeASElement product_id_free_function, CompareASElements product_id_compare_function)
{
    Order order = malloc(sizeof(*order));
    RETURN_IF_NULL(order, NULL);
    order->id = id;
    order->products_ids = asCreate(product_id_copy_function, product_id_free_function,
                                    product_id_compare_function);
    return (SetElement)order;
}

static SetElement copyOrder(SetElement order)
{
    Order order_copy = malloc(sizeof(*order_copy));
    RETURN_IF_NULL(order_copy, NULL);
    order_copy->id = ((Order)order)->id;
    order_copy->products_ids = ((Order)order)->products_ids;
    if(order_copy->products_ids == NULL){
        free(order_copy);
        return NULL;
    }
    return (SetElement)order_copy;
}

static void freeOrder(SetElement order)
{
    asDestroy((SetElement)((Order)order)->products_ids);
    free(order);
}

static int compareOrder(SetElement order1, SetElement order2)
{
    return ((Order)order1)->id - ((Order)order2)->id;
}

static bool isNameValid(char* name)
{
    RETURN_IF_NULL(name, false);
    char first_letter = name[0];
    return((SMALLEST_DIGIT < first_letter && first_letter < BIGGEST_DIGIT)||
        (SMALL_A < first_letter && first_letter < SMALL_Z) ||
        (CAPITAl_A < first_letter && first_letter < CAPITAL_Z));
}

static double getAbs(double number)
{
    return number > 0 ? number : -number;
}

static int roundToClosestInteger(double number)
{
    return (int)(number + 0.5);
}

static roundToClosestHalfInteger(double number)
{
    number *= 2.0;
    number = (double)roundToClosestInteger(number);
    return number / 2.0;
}

static bool isAmountConsistent(double amount, MatamikyaAmountType amountType)
{
    if(amountType == MATAMIKYA_INTEGER_AMOUNT){
        return getAbs(roundToClosestInteger(amount) - amount) <= INTEGER_AMOUNT_DEVIATION;
    }
    else if(amountType == MATAMIKYA_HALF_INTEGER_AMOUNT){
        return (getAbs(roundToClosestInteger(amount) - amount) <= INTEGER_AMOUNT_DEVIATION) ||
                (getAbs(roundToClosestHalfInteger(amount) - amount) <= INTEGER_AMOUNT_DEVIATION);
    }
    return true;
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
    RETURN_IF_NULL(matamikya, NULL);
    matamikya->storage = NULL;
    matamikya->orders = NULL;
    matamikya->profit = NULL;
    matamikya->next_order_id = 1;
    return matamikya;
}

void matamikyaDestroy(Matamikya matamikya)
{
    RETURN_IF_NULL(matamikya, NULL);
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
    if(matamikya == NULL || name == NULL || customData == NULL || 
        copyData == NULL || freeData == NULL || prodPrice == NULL){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if(!isNameValid(name)){
        return MATAMIKYA_INVALID_NAME;
    }
    if(amount < 0 || !isAmountConsistent(amount, amountType)){
        return MATAMIKYA_INVALID_AMOUNT;
    }
    if(matamikya->storage == NULL){
        matamikya->storage = asCreate(copyProduct, freeProduct, compareProduct);
        matamikya->profit = asCreate(copyProductId, freeProductId, compareProductId);
        RETURN_IF_NULL(matamikya->storage, MATAMIKYA_OUT_OF_MEMORY);
        RETURN_IF_NULL(matamikya->profit, MATAMIKYA_OUT_OF_MEMORY);
    }
    if(isProductInStorage(matamikya, id)){
        return MATAMIKYA_PRODUCT_ALREADY_EXIST;
    }
    ASElement product_to_add = createProduct(name, id, customData, amountType, copyData, freeData, prodPrice);
    RETURN_IF_NULL(product_to_add, MATAMIKYA_OUT_OF_MEMORY);
    AmountSetResult storage_register_result = asRegister(matamikya->storage, product_to_add);
    AmountSetResult profit_register_result = asRegister(matamikya->profit, (ASElement)(((Product)product_to_add)->id));
    assert(storage_register_result == AS_SUCCESS && profit_register_result == AS_SUCCESS);
    return MATAMIKYA_SUCCESS;
}

unsigned int mtmCreateNewOrder(Matamikya matamikya)
{
    if(matamikya->orders == NULL){
        matamikya->orders = setCreate(copyOrder, freeOrder, compareOrder);
    }
    Order new_order = createOrder(matamikya->next_order_id, copyProductId, freeProductId, compareProductId);
    matamikya->next_order_id++;
    setAdd(matamikya->orders, (SetElement)new_order);
    return new_order->id;
}

static bool isOrderExists(Matamikya matamikya, const unsigned int orderId)
{
    Order tmp = createOrder(orderId, copyProductId, freeProductId, compareProductId);
    if(tmp == NULL){
        return false;
    }
    bool is_in = setIsIn(matamikya->orders, tmp);
    freeOrder(tmp);
    return is_in;
}
static bool isProductInStorage(Matamikya matamikya, const unsigned int productId)
{
    RETURN_IF_NULL(matamikya, false);
    Product tmp = copyProduct(asGetFirst(matamikya->storage));
    RETURN_IF_NULL(tmp, false);
    tmp->id = productId;
    bool is_in = asContains(matamikya->storage, tmp);
    freeProduct(tmp);
    return is_in;
}
MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, const unsigned int orderId,
                                     const unsigned int productId, const double amount)
{
    RETURN_IF_NULL(matamikya, MATAMIKYA_NULL_ARGUMENT);
    if(!isOrderExists(matamikya, orderId)){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    if(!isProductInStorage(matamikya, productId)){
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
}

