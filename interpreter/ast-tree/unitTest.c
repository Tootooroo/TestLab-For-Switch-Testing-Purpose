/* unitTest.c */

#include "test.h"

#include "variable.h"
#include "expression.h"
#include "statement.h"
#include "func.h"
#include "parameter.h"
#include "module.h"
#include "misc.h"
#include "array.h"
#include "variable_ops.h"

int main(void) {
    moduleInit();
    variableOpsInit();

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(paramTest),
        cmocka_unit_test(exprTest),
        cmocka_unit_test(primitiveTest),
        cmocka_unit_test(stmtTest),
        cmocka_unit_test(moduleTesting),
        cmocka_unit_test(miscTest),
        cmocka_unit_test(arrayTesting)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
