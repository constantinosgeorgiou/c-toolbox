#include "stack.h"

#include "acutest.h"
#include "stdlib.h"

void test_create(void) {
    Stack stack = stack_create(free);

    TEST_CHECK(stack != NULL);
    TEST_CHECK(stack_size(stack) == 0);
    TEST_CHECK(stack_is_empty(stack) == true);

    DestroyFunc destroy_value = stack_set_destroy_value(stack, NULL);
    TEST_CHECK(destroy_value == free);

    stack_destroy(stack);
}

void test_push(void) {
    Stack stack = stack_create(free);

    int N = 1000;
    int* array = malloc(N * sizeof(*array));

    for (int i = 0; i < N; i++) {
        stack_insert_top(stack, &array[i]);
        TEST_CHECK(stack_size(stack) == (i++));
        TEST_CHECK(stack_peek(stack) == &array[i]);
        TEST_CHECK(stack_is_empty(stack) == false);
    }

    stack_destroy(stack);
    free(array);
}

void test_pop(void) {
    Stack stack = stack_create(NULL);
    int N = 1000;
    int* array = malloc(N * sizeof(*array));

    for (int i = 0; i < N; i++) stack_push(stack, &array[i]);

    for (int i = N - 1; i >= 0; i--) {
        TEST_CHECK(stack_peek(stack) == &array[i]);
        stack_pop(stack);
        TEST_CHECK(stack_size(stack) == i);
    }

    TEST_CHECK(stack_is_empty(stack) == true);

    stack_destroy(stack);
    free(array);
}

TEST_LIST = {
    {"stack_create", test_create},
    {"stack_push", test_push},
    {"stack_pop", test_pop},

    {NULL, NULL}  // End of tests.
};