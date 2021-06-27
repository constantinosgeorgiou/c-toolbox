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

void test_split(void) {
    OrderedSet oset = oset_create(compare_ints, free, free);

    int N = 1000;

    // Create key and value arrays.
    int** key_array = create_array(N, 0);
    shuffle(key_array, N);  // Shuffle key array for uniform value insertion.
    int** value_array = create_array(N, 0);

    // Insert (key, value) pairs.
    for (int i = 0; i < N; i++) {
        oset_insert(oset, key_array[i], value_array[i]);
    }

    // Split in the middle.
    int split_key = N / 2;
    OrderedSet split = oset_split(oset, &split_key);

    // Check values of Ordered Sets.
    OrderedSetNode node = oset_first(oset);
    for (int i = 0; i < N / 2; i++) {
        int* key = oset_node_key(oset, node);
        TEST_CHECK(*key == i);
        node = oset_next(oset, node);
    }
    node = oset_first(split);
    for (int i = N / 2; i < N; i++) {
        int* key = oset_node_key(split, node);
        TEST_CHECK(*key == i);
        node = oset_next(split, node);
    }
    oset_destroy(split);

    // Split with split_key < 0 (first key)
    split_key = -1;
    OrderedSet empty = oset_split(oset, &split_key);
    TEST_CHECK(oset_size(empty) == 0);
    oset_destroy(empty);

    // Split with split_key > N (last key)
    split_key = N + 1;
    OrderedSet empty = oset_split(oset, &split_key);
    TEST_CHECK(oset_size(empty) == 0);

    // Split empty Ordered Set.
    OrderedSet error = oset_split(empty, &split_key);
    TEST_CHECK(error == OSET_ERROR);
    oset_destroy(empty);

    oset_destroy(oset);

    free(key_array);
    free(value_array);
}

void test_merge(void) {
    OrderedSet alpha = oset_create(compare_ints, free, free);
    OrderedSet beta = oset_create(compare_ints, free, free);

    int N = 1000;

    // Create key and value arrays for odd number Ordered Set.
    int** odd_key_array = malloc(N * sizeof(*odd_key_array));
    int** odd_value_array = malloc(N * sizeof(*odd_value_array));
    int number = 1;
    for (int i = 0; i < N; i++) {
        odd_key_array[i] = create_int(number);
        odd_value_array[i] = create_int(number);
        number += 2;
    }
    shuffle(odd_key_array, N);  // Shuffle key array for uniform value insertion.

    // Create key and value arrays for even number Ordered Set.
    int** even_key_array = malloc(N * sizeof(*even_key_array));
    int** even_value_array = malloc(N * sizeof(*even_value_array));
    number = 0;
    for (int i = 0; i < N; i++) {
        even_key_array[i] = create_int(number);
        even_value_array[i] = create_int(number);
        number += 2;
    }
    shuffle(even_key_array, N);  // Shuffle key array for uniform value insertion.

    // Insert (key, value) pairs into Ordered Sets.
    for (int i = 0; i < N; i++) {
        oset_insert(alpha, odd_key_array[i], odd_value_array[i]);
        oset_insert(beta, even_key_array[i], even_value_array[i]);
    }

    oset_merge(alpha, beta);

    OrderedSetNode node = oset_first(alpha);
    for (int i = 0; i < (2 * N); i++) {
        int* key = oset_node_key(alpha, node);
        int* value = oset_node_value(alpha, node);

        TEST_CHECK(*key == i);
        TEST_CHECK(*value == i);

        node = oset_next(alpha, node);
    }

    oset_destroy(alpha);
    oset_destroy(beta);

    free(odd_key_array);
    free(odd_value_array);
    free(even_key_array);
    free(even_value_array);
}

void test_concat(void) {
    OrderedSet a = oset_create(compare_ints, free, free);

    int N = 1000;

    // Create key and value arrays.
    int** key_array = create_array(N, 0);
    shuffle(key_array, N);  // Shuffle key array for uniform value insertion.
    int** value_array = create_array(N, 0);

    // Insert (key, value) pairs.
    for (int i = 0; i < N; i++) {
        oset_insert(a, key_array[i], value_array[i]);
    }

    // Split in the middle.
    int split_key = N / 2;
    OrderedSet b = oset_split(a, &split_key);

    oset_concat(a, b);

    OrderedSetNode node = oset_first(a);
    for (int i = 0; i < N; i++) {
        int* key = oset_node_key(a, node);
        TEST_CHECK(*key == i);
        node = oset_next(a, node);
    }

    oset_destroy(a);

    free(key_array);
    free(value_array);
}

TEST_LIST = {
    {"oset_create", test_create},
    {"oset_insert", test_insert},
    {"oset_remove", test_remove},
    {"oset_traversal", test_traversal},
    {"oset_find", test_find},
    {"oset_get_at", test_get_at},
    {"oset_remove_at", test_remove_at},
    {"oset_split", test_split},
    {"oset_merge", test_merge},
    {"oset_concat", test_concat},

    {NULL, NULL}  // End of tests
}