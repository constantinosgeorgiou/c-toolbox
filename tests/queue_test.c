#include "acutest.h"

void test_create(void) {}
void test_enqueue(void) {}
void test_dequeue(void) {}

TEST_LIST = {
    {"queue_create", test_create},
    {"queue_enqueue", test_enqueue},
    {"queue_dequeue", test_dequeue},

    {NULL, NULL}  // End of tests.
};