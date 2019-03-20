/* unitTest.c */

#include "test.h"

// Queue testing
#include "queue.h"


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(queue_Basic) 
    }; 
    return cmocka_run_group_tests(tests, NULL, NULL);
}
