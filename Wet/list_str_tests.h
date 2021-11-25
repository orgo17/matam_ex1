#ifndef LIST_STR_TESTS_H_
#define LIST_STR_TESTS_H_
#include <stdio.h>
#include <stdbool.h>

/**
 * Tests if it was possible to create an empty list correctly.  
 * 
 * @return true if creation was succesful.
 * @return false if memory allocation failed during creation.
 */
bool testCreateAndDestory();

/**
 * Tests if the size of a non-empty list is returned correctly
 * 
 * @return true if the size that was calculated
 * @return false 
 */
bool testListGetSize();

/**
 * tests size of empty list 
 * 
 * @return true 
 * @return false 
 */
bool testListGetSizeEmpty();

/**
 *  tests size of null 
 * 
 * @return true 
 * @return false 
 */
bool testListGetSizeNull();

bool testListCopy();
bool testListCopyEmpty();
bool testListCopyNull();
bool testCompareList();
bool testCompareEmptyLists();
bool testCompareDifferentLists();
bool testCompareNullLists();
bool testListClear();
bool testListRemoveElement();
bool testListRemoveInsert();
bool testListSetAmountOfElement();
bool testListSetAmountOfElementEmptey();


/**
 * Macro to run a test function and print its result.
 * 
 * @param test A test function that returns true if the test passed
 * and false if it failed
 */
#define RUN_TEST(test) \
do { \
  printf("Running " #test "... "); \
  if (test()) { \
    printf("[OK]"); \
  } \
  printf("\n"); \
} while(0)

#endif /* LIST_STR_TESTS_H_ */

