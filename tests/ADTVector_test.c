#include "ADTVector.h"

#include <stdlib.h>

#include "acutest.h"

void test_create(void) {
    Vector vec1 = vector_create(0, free);
    Vector vec2 = vector_create(10, free);

    TEST_CHECK(vector_size(vec1) == 0);
    TEST_CHECK(vector_size(vec2) == 10);

    DestroyFunc destroy_value1 = vector_set_destroy_value(vec1, NULL);
    DestroyFunc destroy_value2 = vector_set_destroy_value(vec2, NULL);

    TEST_CHECK(destroy_value1 == free);
    TEST_CHECK(destroy_value2 == free);

    vector_destroy(vec1);
    vector_destroy(vec2);
}

TEST_LIST = {
    {"vector_create", test_create},

    {NULL, NULL}  // End of tests.
};