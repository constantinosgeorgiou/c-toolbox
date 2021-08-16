#include "pqueue.h"

#include <stdbool.h>

#include "ADTVector.h"
#include "acutest.h"

/// Compares two int*.
///
/// @return < 0, if a < b, or, > 0, if b < a, or, 0, if a and b are equivalent
///
static int compare_ints(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

/// Allocates memory for an integer with given value.
///
/// @return Newly created pointer to integer.
///
static int* create_int(int value) {
    int* pointer = malloc(sizeof(int));
    if (pointer == NULL) {
        return NULL;
    }
    *pointer = value;
    return pointer;
}

void test_create(void) {
    PQueue pqueue = pqueue_create(compare_ints, free, NULL);

    TEST_CHECK(pqueue != NULL);
    TEST_CHECK(pqueue_size(pqueue) == 0);
    TEST_CHECK(pqueue_is_empty(pqueue) == true);

    DestroyFunc destroy_element = pqueue_set_destroy_value(pqueue, NULL);
    TEST_CHECK(destroy_element = free);

    pqueue_destroy(pqueue);

    // Create with initial elements.

    int N = 5;

    Vector elements = vector_create(0, NULL);
    for (int i = 0; i < N; i++) {
        vector_insert_last(elements, create_int(i));
    }

    pqueue = pqueue_create(compare_ints, free, elements);

    TEST_CHECK(pqueue != NULL);
    TEST_CHECK(pqueue_size(pqueue) == N);
    TEST_CHECK(pqueue_is_empty(pqueue) == false);

    TEST_CHECK(*(int*)pqueue_peek(pqueue) == N - 1);
    pqueue_pull(pqueue);
    TEST_CHECK(*(int*)pqueue_peek(pqueue) == N - 1);

    vector_destroy(elements);
    pqueue_destroy(pqueue);
}

void test_insert(void) {}

void test_pull(void) {}

TEST_LIST = {
    {"pqueue_create", test_create},
    {"pqueue_insert", test_insert},
    {"pqueue_pull", test_pull},

    {NULL, NULL}  // End of tests.
};