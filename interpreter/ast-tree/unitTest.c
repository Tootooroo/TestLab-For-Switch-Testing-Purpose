/* unitTest.c */

#include "test.h"

#include "variable.h"
#include "expression.h"
#include "statement.h"
#include "func.h"
#include "parameter.h"
#include "module.h"

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(paramTest),
        cmocka_unit_test(exprTest),
        cmocka_unit_test(primitiveTest),
        cmocka_unit_test(stmtTest),
        cmocka_unit_test(moduleTesting)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
