#ifndef MATAMIKYA_ORDER_H_
#define MATAMIKYA_ORDER_H_

#include "matamikya.h"
#include "amount_set.h"
#include "set.h"
#include <stdlib.h>

/** Type for representing a order object */
typedef struct order_t *Order;

/**
 * @brief Copies a given product_id element
 * 
 * @param product_id - the element to be copied
 * @return Copy of the given product_id element
 */
ASElement copyProductId(ASElement product_id);

/**
 * @brief Deallocates memory that was allocated to a product_id element
 * 
 * @param product_id - product_id element to be free'd 
 */
void freeProductId(ASElement product_id);

/**
 * @brief Compares between two product ids
 * 
 * @param product_id1 first product id
 * @param product_id2 second product id
 * @return If product_id1 is bigger than product_id2 return 1
 * If product_id1 id is smaller than product_id2 id return -1
 * If both ids are equal return 0 
 */
int compareProductId(ASElement product_id1, ASElement product_id2);

/**
 * @brief Create a Order object
 * 
 * @param id - Order's id
 * @param product_id_copy_function - copy function for product ids in the order
 * @param product_id_free_function - free function for product ids in the order
 * @param product_id_compare_function - compare function for product ids in the order
 * @return A new order Object 
 */
SetElement createOrder(const unsigned int id, CopyASElement product_id_copy_function, 
                    FreeASElement product_id_free_function, CompareASElements product_id_compare_function);

/**
 * @brief Copies an order object
 * 
 * @param order - Order to be copied
 * @return A copy of the given order
 */
SetElement copyOrder(SetElement order);

/**
 * @brief Deallocates memory that was allocated to an order object
 * 
 * @param order - Order to be free'd
 */
void freeOrder(SetElement order);

/**
 * @brief Compares between to orders, using their ids
 * 
 * @param order1 - first order
 * @param order2 - second order
 * @return If order1 id is bigger than order2 id return 1
 * If order1 id is smaller than order2 id return -1
 * If both ids are equal return 0 
 */
int compareOrder(SetElement order1, SetElement order2);

unsigned int getOrderId(SetElement order);
AmountSet getOrderProductIds(SetElement order);

#define RETURN_IF_NULL(pointer, return_value)\
        if(pointer == NULL){\
            return return_value;\
        }\
        
#endif /* MATAMIKYA_ORDER_H_ */