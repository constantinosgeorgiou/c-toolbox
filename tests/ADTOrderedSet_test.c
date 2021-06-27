#include "ADTOrderedSet.h"

#include <stdlib.h>

#include "acutest.h"

/// @brief Compares two int*.
///
/// @return < 0, if a < b, or, > 0, if b < a, or, 0, if a and b are equivalent
///
static int compare_ints(const void* a, const void* b) { return *(int*)a - *(int*)b; }

void test_create(void) {
    OrderedSet oset = oset_create(compare_ints, free, free);

    TEST_CHECK(oset != OSET_ERROR);
    TEST_CHECK(oset_size(oset) == 0);

    DestroyFunc destroy_key = oset_set_destroy_key(oset, NULL);
    DestroyFunc destroy_value = oset_set_destroy_value(oset, NULL);

    TEST_CHECK(destroy_key == free);
    TEST_CHECK(destroy_value == free);

    // TODO: Create oset with vector containing values.

    oset_destroy(oset);
}

/// @brief Shuffles the values of an array.
///
void shuffle(int* array[], int size) {
    for (int i = 0; i < size; i++) {
        int j = i + rand() / (RAND_MAX / (size - i) + 1);
        int* t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

/// @brief Inserts (key, value) pair to oset and test for correct insertion.
///
void insert_and_test(OrderedSet oset, void* key, void* value) {
    oset_insert(oset, key, value);
    TEST_CHECK(oset_find(oset, key) == value);
}

void test_insert(void) {
    OrderedSet oset = oset_create(compare_ints, free, free);

    int N = 1000;

    // Create, initialize, and suffle key array.
    int** key_array = malloc(N * sizeof(*key_array));
    for (int i = 0; i < N; i++) {
        key_array[i] = create_int(i);
    }
    shuffle(key_array, N);  // Shuffle key array for uniform value insertion.

    // Create value array.
    int** value_array = malloc(N * sizeof(*value_array));

    for (int i = 0; i < N; i++) {
        value_array[i] = create_int(i);

        insert_and_test(oset, key_array[i], value_array[i]);

        TEST_CHECK(oset_size(oset) == (i + 1));
    }

    // Insert key equivalent to the first key, and check if the (key, value) pair was inserted
    // correctly.
    int* duplicate_key = create_int(*key_array[0]);
    int* value = create_int(N + 1);  // N + 1, to create a number definitely not in the Ordered Set.
    int size = oset_size(oset);
    insert_and_test(oset, duplicate_key, value);
    TEST_CHECK(oset_size(oset) == (size + 1));

    // Check if duplicate key was inserted before the original key.
    OrderedSetNode dup = oset_find_node(oset, duplicate_key);
    int* next_key = oset_node_key(oset, oset_next(oset, dup));
    int* next_value = oset_node_value(oset, oset_next(oset, dup));
    TEST_CHECK(compare_ints(next_key, duplicate_key) == 0);  // Check if key are the same.
    TEST_CHECK(next_key != duplicate_key);                   // Check if addresses are different.
    TEST_CHECK(compare_ints(next_value, value) != 0);        // Check if values are different.

    // Check if values are sorted.
    OrderedSetNode node = oset_first(oset);
    for (int i = 0; i < N; i++) {
        int* value = oset_node_value(oset, node);

        TEST_CHECK(*value == i);

        node = oset_next(oset, node);
    }

    oset_destroy(oset);
}

void test_remove(void) {
    OrderedSet oset = oset_create(compare_ints, NULL, NULL);

    int N = 1000;

    // Create, initialize, and suffle key array.
    int** key_array = malloc(N * sizeof(*key_array));
    for (int i = 0; i < N; i++) {
        key_array[i] = create_int(i);
    }
    shuffle(key_array, N);  // Shuffle key array for uniform value insertion.

    // Create value array.
    int** value_array = malloc(N * sizeof(*value_array));

    // Insert (key, value) pairs.
    for (int i = 0; i < N; i++) {
        value_array[i] = create_int(i);
        oset_insert(oset, key_array[i], value_array[i]);
    }

    // Remove first key.
    int* key = create_int(0);
    TEST_CHECK(oset_remove(oset, key));
    TEST_CHECK(*((int*)oset_node_key(oset, oset_first(oset))) == 1);  // Check first key is 1.

    // Remove last key.
    *key = N - 1;
    TEST_CHECK(oset_remove(oset, key));
    TEST_CHECK(*((int*)oset_node_key(oset, oset_last(oset))) == (N - 2));  // Check last key is N-2.

    // Remove already removed key.
    TEST_CHECK(oset_remove(oset, key) == false);

    // Remove not existent key.
    *key = N + N;  // N + N, guarantees a large key not in Ordered Set.
    TEST_CHECK(oset_remove(oset, key) == false);

    // Insert duplicate key.
    *key = N / 2;                    // N / 2, guarantees already existent key.
    int* value = create_int(N + N);  // Value can be whatever, doesn't matter.
    oset_insert(oset, key, value);

    // Remove duplicate key and original key.
    TEST_CHECK(oset_remove(oset, key));
    TEST_CHECK(oset_remove(oset, key));
    TEST_CHECK(oset_find(oset, key) == NULL);

    // Remove keys.
    int size = oset_size(oset);
    for (int i = 0; i < N; i++) {
        TEST_CHECK(oset_remove(oset, key_array[i]));
        TEST_CHECK(oset_size(oset) == (size--));
        TEST_CHECK(oset_find(oset, key_array[i]) == NULL);
    }

    // Change DestroyFuncs to see if removal of (key, value) pairs frees the allocated memory.
    oset_set_destroy_key(oset, free);
    oset_set_destroy_value(oset, free);

    for (int i = 0; i < N; i++) {
        oset_insert(oset, key_array[i], value_array[i]);
    }

    // Remove keys.
    int size = oset_size(oset);
    for (int i = 0; i < N; i++) {
        TEST_CHECK(oset_remove(oset, key_array[i]));
        TEST_CHECK(oset_size(oset) == (size--));
        TEST_CHECK(oset_find(oset, key_array[i]) == NULL);
    }

    free(key_array);
    free(value_array);
}

TEST_LIST = {
    {"oset_create", test_create},
    {"oset_insert", test_insert},
    {"oset_remove", test_remove},
    {NULL, NULL}  // End of tests
}