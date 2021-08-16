#include "acutest.h"

void test_create(void) {}

void test_insert(void) {}

void test_pull(void) {}

TEST_LIST = {
    {"pqueue_create", test_create},
    {"pqueue_insert", test_insert},
    {"pqueue_pull", test_pull},

    {NULL, NULL}  // End of tests.
};