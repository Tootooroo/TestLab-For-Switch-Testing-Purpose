/* unitTest.c */

#include "test.h"

#include "variable.h"
#include "expression.h"

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(varTest),
        cmocka_unit_test(primitiveTest),
        cmocka_unit_test(exprTest)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
