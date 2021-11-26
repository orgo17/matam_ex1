#include "amount_set_str.h"
#include "amount_set_str_tests.h"

int main()
{
    RUN_TEST(testAmountSetCreateAndDestory);
    RUN_TEST(testAsCopy);
    RUN_TEST(testAsCopyEmptySet);
    RUN_TEST(testListGetSize);
    RUN_TEST(testListGetSizeEmpty);
    RUN_TEST(testListGetSizeNull);
    RUN_TEST(testAsContains);
    RUN_TEST(testAsContainsEmpty);
    RUN_TEST(testAsContainsNull);
    RUN_TEST(testAsGetAmount);
}