#ifndef AMOUNT_SET_STR_TESTS_H_
#define AMOUNT_SET_STR_TESTS_H_
#include <stdbool.h>

bool testAmountSetCreateAndDestory();
bool testAsCopy();
bool testAsCopyEmptySet();
bool testListGetSize();
bool testListGetSizeEmpty();
bool testListGetSizeNull();
bool testAsContains();
bool testAsContainsEmpty();
bool testAsContainsNull();
bool testAsGetAmount();

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

#endif /* AMOUNT_SET_STR_TESTS_H_ */