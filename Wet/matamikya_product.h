#ifndef MATAMIKYA_PRODUCT_H_
#define MATAMIKYA_PRODUCT_H_

#include "matamikya.h"
#include "amount_set.h"

/** Type for representing a product object */
typedef struct product_t *Product;

/**
 * @brief Create a Product object
 * 
 * @param name - product's name
 * @param id - product's id
 * @param product_data - user supplied product data such as price per amount type
 * @param amount_type - product's amount type
 * @param copy_function - user supplied function to copy product_data
 * @param free_function - user supplied function to free product_data
 * @param price_function - user supplied function to return the price according to product_data
 * @return new product object 
 */
ASElement createProduct(const char* name, const unsigned int id, 
                        MtmProductData product_data, MatamikyaAmountType amount_type,
                        MtmCopyData copy_function, MtmFreeData free_function,
                        MtmGetProductPrice price_function);

/**
 * @brief Copies a given product object
 * 
 * @param product - product to be copied
 * @return Copy of the given product 
 */
ASElement copyProduct(ASElement product);

/**
 * @brief Deallocates memory that was allocated to a product object
 * 
 * @param product - product to be free'd 
 */
void freeProduct(ASElement product);

/**
 * @brief Compares between two products using their ids
 * 
 * @param product1 - first product to compare 
 * @param product2 - second product to compare 
 * @return If product1 id is bigger than product2 id return 1
 * If product1 id is smaller than product2 id return -1
 * If both products ids are equal return 0 
 */
int compareProduct(ASElement product1, ASElement product2);

unsigned int getProductId(ASElement product);
void setProductId(ASElement product, unsigned int id);
char* getProductName(ASElement product);
MatamikyaAmountType getProductAmountType(ASElement product);

/**
 * @brief Calculates the profit made from selling the given product at the given amount
 * 
 * @param product - product to sell
 * @param amount - amount of product
 * @return Profit made from the sell
 */
double calculateProductProfit(ASElement product, double amount);

#define RETURN_IF_NULL(pointer, return_value)\
        if(pointer == NULL){\
            return return_value;\
        }\

#endif /* MATAMIKYA_PRODUCT_H_ */