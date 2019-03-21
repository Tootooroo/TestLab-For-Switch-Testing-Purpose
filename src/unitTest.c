/* unitTest.c */

#include "test.h"

// Queue testing
#include "queue.h"
// List testing
#include "list.h"

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(queue_Basic),
        cmocka_unit_test(list_Basic)
    }; 
    return cmocka_run_group_tests(tests, NULL, NULL);
}
