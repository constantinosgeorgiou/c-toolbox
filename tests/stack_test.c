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

TEST_LIST = {
    {"stack_create", test_create},

    {NULL, NULL}  // End of tests.
};