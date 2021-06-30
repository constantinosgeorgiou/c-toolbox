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

void test_insert(void) {
    Vector vec = vector_create(0, NULL);
    int N = 1000;
    int* array = malloc(N * sizeof(*array));

    // Insert 1000 elements to cause multiple resizes.
    for (int i = 0; i < 1000; i++) {
        vector_insert_last(vec, &array[i]);
        TEST_CHECK(vector_size(vec) == i + 1);
        TEST_CHECK(vector_get_at(vec, i) == &array[i]);
    }

    for (int i = 0; i < 1000; i++)
        TEST_CHECK(vector_get_at(vec, i) == &array[i]);

    vector_destroy(vec);
    free(array);
}

TEST_LIST = {
    {"vector_create", test_create},
    {"vector_insert", test_insert},

    {NULL, NULL}  // End of tests.
};