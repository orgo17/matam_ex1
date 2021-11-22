#include "amount_set_str.h"

int main()
{
    AmountSet set = asCreate();
    asDestroy(set);
    return 0;
}