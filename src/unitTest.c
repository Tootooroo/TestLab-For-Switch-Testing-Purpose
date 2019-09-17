/* unitTest.c */

#include "test.h"

// Queue testing
#include "queue.h"
// List testing
#include "list.h"
// HashMap testing
#include "hashMap.h"
// Pair testing
#include "pair.h"
// Wrapper testing
#include "wrapper.h"

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(queue_Basic),
        cmocka_unit_test(list_Basic),
        cmocka_unit_test(hashMap_Basic),
        cmocka_unit_test(pairTest),
        cmocka_unit_test(wrapper_test)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
