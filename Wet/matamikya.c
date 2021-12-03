#include "matamikya.h"
#include "amount_set.h"
#include "matamikya_help.h"
#include "set.h"
#include "matamikya_print.h"
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

static bool isNameValid(const char* name)
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
    return (int)(number < 0 ? (number - 0.5) : (number + 0.5));
}

static double roundToClosestHalfInteger(double number)
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

static bool isProductInStorage(Matamikya matamikya, const unsigned int productId)
{
    RETURN_IF_NULL(matamikya, false);
    RETURN_IF_NULL(matamikya->storage, false);
    RETURN_IF_NULL(asGetFirst(matamikya->storage), false);
    Product tmp = copyProduct(asGetFirst(matamikya->storage));
    RETURN_IF_NULL(tmp, false);
    tmp->id = productId;
    bool is_in = asContains(matamikya->storage, tmp);
    freeProduct(tmp);
    return is_in;
}

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
    if(matamikya == NULL){
        return;
    }
    asDestroy(matamikya->storage);
    setDestroy(matamikya->orders);
    asDestroy(matamikya->profit);
    free(matamikya);
}

static ASElement returnProductById(Matamikya matamikya, AmountSet product_as, const unsigned int productId)
{
    RETURN_IF_NULL(product_as, NULL);
    AS_FOREACH(ASElement, iterator, product_as){
        if((((Product)iterator)->id) - productId == 0){
            return iterator;
        }
    }
    return NULL;
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
    AmountSetResult profit_register_result = 
                    asRegister(matamikya->profit, (ASElement)(&((Product)product_to_add)->id));
    assert(storage_register_result == AS_SUCCESS && profit_register_result == AS_SUCCESS);
    AmountSetResult storage_amount_result = asChangeAmount(matamikya->storage, product_to_add, amount);
    assert(storage_amount_result == AS_SUCCESS);
    freeProduct(product_to_add);
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount)
{
    RETURN_IF_NULL(matamikya, MATAMIKYA_NULL_ARGUMENT);
    if(!isProductInStorage(matamikya, id)){
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
    if(!isAmountConsistent(amount, getProductAmountType(returnProductById(matamikya, matamikya->storage, id)))){
        return MATAMIKYA_INVALID_AMOUNT;
    }
    AmountSetResult change_amount_in_storage_result = 
                    asChangeAmount(matamikya->storage, returnProductById(matamikya, matamikya->storage, id), amount);
    if(change_amount_in_storage_result == AS_NULL_ARGUMENT){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if(change_amount_in_storage_result == AS_INSUFFICIENT_AMOUNT){
        return MATAMIKYA_INSUFFICIENT_AMOUNT;
    }
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id)
{
    RETURN_IF_NULL(matamikya, MATAMIKYA_NULL_ARGUMENT);
    AmountSetResult delete_product_result = 
                    asDelete(matamikya->storage, returnProductById(matamikya, matamikya->storage ,id));
    if(delete_product_result == AS_NULL_ARGUMENT){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if(delete_product_result == AS_ITEM_DOES_NOT_EXIST){
        return MATAMIKYA_PRODUCT_NOT_EXIST;
    }
    AmountSetResult delete_product_profit_result = asDelete(matamikya->profit, (ASElement)(&id));
    if(delete_product_profit_result == AS_NULL_ARGUMENT){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    return MATAMIKYA_SUCCESS;
}

unsigned int mtmCreateNewOrder(Matamikya matamikya)
{
    if(matamikya->orders == NULL){
        matamikya->orders = setCreate(copyOrder, freeOrder, compareOrder);
    }
    SetElement new_order = createOrder(matamikya->next_order_id, copyProductId, freeProductId, compareProductId);
    RETURN_IF_NULL(new_order, 0);
    matamikya->next_order_id++;
    setAdd(matamikya->orders, (SetElement)new_order);
    freeOrder(new_order);
    return (matamikya->next_order_id)-1;
}

static bool isOrderExists(Matamikya matamikya, const unsigned int orderId)
{
    RETURN_IF_NULL(matamikya, false);
    SetElement tmp = createOrder(orderId, copyProductId, freeProductId, compareProductId);
    RETURN_IF_NULL(tmp, false);
    bool is_in = setIsIn(matamikya->orders, tmp);
    freeOrder(tmp);
    return is_in;
}

static SetElement returnOrderById(Matamikya matamikya, const unsigned int orderId)
{
    RETURN_IF_NULL(matamikya, NULL);
    SET_FOREACH(SetElement, iterator, matamikya->orders){
        if((((Order)iterator)->id) - orderId == 0){
            return iterator;
        }
    }
    return NULL;
}

static ASElement returnAsElementIdById(AmountSet as_product_ids, const unsigned int productId)
{
    RETURN_IF_NULL(as_product_ids, NULL);
    AS_FOREACH(ASElement, iterator, as_product_ids){
        int iterator_id = *(int*)iterator;
        if(iterator_id - productId == 0){
            return iterator;
        }
    }
    return NULL;
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
    ASElement product_to_change = returnProductById(matamikya,matamikya->storage, productId);
    assert(product_to_change);
    if(!isAmountConsistent(amount, ((Product)product_to_change)->amount_type)){
        return MATAMIKYA_INVALID_AMOUNT;
    }
    AmountSet products_in_order = ((Order)(returnOrderById(matamikya, orderId)))->products_ids;
    ASElement product_id_to_change_amount = returnAsElementIdById(products_in_order, productId);
    AmountSetResult as_change_amount_result = asChangeAmount(products_in_order, product_id_to_change_amount, amount);
    //check if product in order
    if(as_change_amount_result == AS_NULL_ARGUMENT && amount>0){
        asRegister(products_in_order, (ASElement)(&productId));
        product_id_to_change_amount = returnAsElementIdById(products_in_order, productId);
        asChangeAmount(products_in_order, product_id_to_change_amount, amount);
    }
    //check if product amount <= 0 and remove him from order
    if(as_change_amount_result == AS_INSUFFICIENT_AMOUNT){
        asDelete(products_in_order, product_id_to_change_amount);
    }
    return MATAMIKYA_SUCCESS;
}

static bool isEnoughInStorage(Matamikya matamikya, SetElement order)
{
    AS_FOREACH(ASElement, iterator, ((Order)order)->products_ids){
        double amount_in_order = 0, amount_in_storage = 0;
        asGetAmount(((Order)order)->products_ids ,iterator, &amount_in_order); 
        asGetAmount(matamikya->storage,
                    returnProductById(matamikya, matamikya->storage, *(int*)iterator), 
                    &amount_in_storage);
        if(amount_in_order > amount_in_storage){
           return false;
        }
    }
    return true;
}

MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int orderId)
{
    RETURN_IF_NULL(matamikya, MATAMIKYA_NULL_ARGUMENT);
    SetElement order = returnOrderById(matamikya, orderId);
    if(!setIsIn(matamikya->orders, order)){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    if(!isEnoughInStorage(matamikya, order)){
        return MATAMIKYA_INSUFFICIENT_AMOUNT;
    }
    AS_FOREACH(ASElement, iterator, ((Order)order)->products_ids){
        int product_id = *(int*)iterator;
        double amount_in_order;
        asGetAmount(((Order)order)->products_ids, iterator, &amount_in_order);
        mtmChangeProductAmount(matamikya, product_id, -amount_in_order);
        Product product_to_sell = (Product)returnProductById(matamikya, matamikya->storage, product_id);
        double profit = product_to_sell->product_data_get_price(product_to_sell->product_data, amount_in_order);
        asChangeAmount(matamikya->profit, returnAsElementIdById(matamikya->profit, product_id), profit);
    }
    setRemove(matamikya->orders, order);
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int orderId)
{
    RETURN_IF_NULL(matamikya, MATAMIKYA_NULL_ARGUMENT);
    SetResult set_remove_result = setRemove(matamikya->orders, returnOrderById(matamikya, orderId));
    if(set_remove_result == SET_NULL_ARGUMENT){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    if(set_remove_result == SET_ITEM_DOES_NOT_EXIST){
        return MATAMIKYA_ORDER_NOT_EXIST;
    }
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE *output)
{
    if(matamikya == NULL || output == NULL){
        return MATAMIKYA_NULL_ARGUMENT;
    }
    fprintf(output, "Inventory Status:\n");
    AS_FOREACH(ASElement, iterator, matamikya->storage){
        char *product_name = ((Product)iterator)->name;
        unsigned int product_id = ((Product)iterator)->id;
        double product_amount = 0;
        asGetAmount(matamikya->storage, iterator, &product_amount);
        const double product_price = 
                    ((Product)iterator)->product_data_get_price(((Product)iterator)->product_data,1.0);
        mtmPrintProductDetails(product_name, product_id, product_amount, product_price, output);
    }
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, FILE *output)
{
    RETURN_IF_NULL(matamikya, MATAMIKYA_NULL_ARGUMENT);
    RETURN_IF_NULL(output, MATAMIKYA_NULL_ARGUMENT);
    SetElement order_to_print = returnOrderById(matamikya, orderId);
    RETURN_IF_NULL(order_to_print, MATAMIKYA_ORDER_NOT_EXIST);
    mtmPrintOrderHeading(orderId, output);
    double total_price = 0;
    AS_FOREACH(ASElement, iterator, ((Order)order_to_print)->products_ids){
        ASElement product_to_print = returnProductById(matamikya, matamikya->storage, *(int*)iterator);
        char *product_name = ((Product)product_to_print)->name;
        unsigned int product_id = ((Product)product_to_print)->id;
        double product_amount_in_order;
        asGetAmount(((Order)order_to_print)->products_ids, iterator, &product_amount_in_order);
        const double product_price_in_order = 
                    ((Product)product_to_print)->product_data_get_price(((Product)product_to_print)->product_data,
                     product_amount_in_order);
        mtmPrintProductDetails(product_name, product_id, product_amount_in_order, product_price_in_order, output);
        total_price += product_price_in_order;
    }
    mtmPrintOrderSummary(total_price, output);
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output)
{
    RETURN_IF_NULL(matamikya, MATAMIKYA_NULL_ARGUMENT);
    RETURN_IF_NULL(output, MATAMIKYA_NULL_ARGUMENT);
    fprintf(output, "Best Selling Product:\n");
    double max_profit = 0, product_profit = 0;
    AS_FOREACH(ASElement, iterator, matamikya->profit){
        asGetAmount(matamikya->profit, iterator, &product_profit);
        if(product_profit > max_profit){
            max_profit = product_profit;
        }
    }
    if(max_profit == 0){
        fprintf(output, "none\n");
        return MATAMIKYA_SUCCESS;
    }
    AS_FOREACH(ASElement, iterator, matamikya->profit){
        asGetAmount(matamikya->profit, iterator, &product_profit);
        if(product_profit == max_profit){
            Product product_to_print = (Product)returnProductById(matamikya, matamikya->storage, *(int*)iterator);
            char *product_name = (product_to_print)->name;
            unsigned int product_id = (product_to_print)->id;
            mtmPrintIncomeLine(product_name, product_id, max_profit, output);
        }
    }
    return MATAMIKYA_SUCCESS;
}

