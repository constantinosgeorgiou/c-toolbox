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

void test_remove(void) {
    Vector vec = vector_create(1000, NULL);
    int N = 1000;
    int* array = malloc(N * sizeof(*array));

    // Replace elements in vector.
    for (int i = 0; i < 1000; i++)
        vector_set_at(vec, i, &array[i]);

    // Remove elements to cause resizes.
    for (int i = 999; i >= 0; i--) {
        TEST_CHECK(vector_get_at(vec, i) == &array[i]);
        vector_remove_last(vec);
        TEST_CHECK(vector_size(vec) == i);
    }

    vector_destroy(vec);
    free(array);
}

void test_get_set_at(void) {
    int N = 1000;
    Vector vec = vector_create(N / 2, NULL);  // N/2, to test the size.
    TEST_CHECK(vector_size(vec) == N / 2);

    // Insert additional N/2 NULLs.
    for (int i = 0; i < N / 2; i++)
        vector_insert_last(vec, NULL);

    int* array = malloc(N * sizeof(*array));
    for (int i = 0; i < N; i++) {
        TEST_CHECK(vector_get_at(vec, i) == NULL);
        vector_set_at(vec, i, &array[i]);
        TEST_CHECK(vector_get_at(vec, i) == &array[i]);
    }

    vector_destroy(vec);
    free(array);
}

void test_iterate(void) {
    Vector vec = vector_create(0, NULL);
    int N = 1000;
    int* array = malloc(N * sizeof(*array));

    TEST_CHECK(vector_first(vec) == VECTOR_BOF);
    TEST_CHECK(vector_last(vec) == VECTOR_EOF);

    // Insert elements.
    for (int i = 0; i < 1000; i++)
        vector_insert_last(vec, &array[i]);

    int i = 0;
    for (VectorNode node = vector_first(vec); node != VECTOR_EOF; node = vector_next(vec, node))
        TEST_CHECK(vector_node_value(vec, node) == &array[i++]);
    TEST_CHECK(i == N);

    for (VectorNode node = vector_last(vec); node != VECTOR_BOF; node = vector_previous(vec, node))
        TEST_CHECK(vector_node_value(vec, node) == &array[--i]);
    TEST_CHECK(i == 0);

    vector_destroy(vec);
    free(array);
}

TEST_LIST = {
    {"vector_create", test_create},
    {"vector_insert", test_insert},
    {"vector_remove", test_remove},
    {"vector_get_set_at", test_get_set_at},
    {"vector_iterate", test_iterate},

    {NULL, NULL}  // End of tests.
};