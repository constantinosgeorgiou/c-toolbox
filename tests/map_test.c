////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file ADTList_test.c
/// @author Constantinos Georgiou
/// @brief Unit tests for implementation of ADT Map.
/// @version 1.0
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "map.h"

#include <stdbool.h>
#include <stdlib.h>

#include "acutest.h"

/// @brief Compares two int*.
///
/// @return < 0, if a < b, or, > 0, if b < a, or, 0, if a and b are equivalent
///
static int compare_ints(const void* a, const void* b) { return *(int*)a - *(int*)b; }

void test_create(void) {
    Map map = map_create(compare_ints, NULL, NULL);
    map_set_hash_function(map, hash_int);
    map_set_destroy_key(map, NULL);
    map_set_destroy_value(map, NULL);

    TEST_CHECK(map != NULL);
    TEST_CHECK(map_size(map) == 0);

    map_destroy(map);
}

/// @brief Allocates memory for an integer with given value.
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

/// @brief Inserts key with value to map and test for correct insertion.
///
void insert_and_test(Map map, void* key, void* value) {
    map_insert(map, key, value);
    TEST_CHECK(map_find(map, key) == value);
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

void test_insert(void) {
    Map map = map_create(compare_ints, free, free);
    map_set_hash_function(map, hash_int);

    int N = 1000;
    int** key_array = malloc(N * sizeof(*key_array));
    int** value_array = malloc(N * sizeof(*value_array));

    for (int i = 0; i < N; i++) {
        key_array[i] = create_int(i);
    }

    // Shuffle key array for uniform value insertion.
    shuffle(key_array, N);

    for (int i = 0; i < N; i++) {
        value_array[i] = create_int(i);

        insert_and_test(map, key_array[i], value_array[i]);

        TEST_CHECK(map_size(map) == (i + 1));
    }

    // Insert a key equivalent to the first key, and check if the (key, value) pair was overwritten.
    int* new_key = create_int(*key_array[0]);
    int* new_value = create_int(99);
    insert_and_test(map, new_key, new_value);

    map_destroy(map);

    // Check if insert/replace works correctly without destroying (key, value) pairs.
    Map map2 = map_create(compare_ints, NULL, NULL);
    map_set_hash_function(map2, hash_int);

    int key1 = 0, value1 = 0;
    int key2 = 0, value2 = 0;

    insert_and_test(map2, &key1, &value1);
    insert_and_test(map2, &key1, &value2);
    insert_and_test(map2, &key2, &value2);

    map_destroy(map2);
    free(key_array);
    free(value_array);
}

void test_remove(void) {
    Map map = map_create(compare_ints, free, free);
    map_set_hash_function(map, hash_int);

    int N = 1000;
    int** key_array = malloc(N * sizeof(*key_array));
    int** value_array = malloc(N * sizeof(*value_array));

    for (int i = 0; i < N; i++) {
        key_array[i] = create_int(i);
        value_array[i] = create_int(i);

        map_insert(map, key_array[i], value_array[i]);

        // Periodically remove the newly inserted key.
        if (i % (N / 20) == 0) {
            TEST_CHECK(map_remove(map, key_array[i]));
        }
    }

    // Remove not existent key.
    int not_exists = N * 2;
    TEST_CHECK(!map_remove(map, &not_exists));

    // Remove all nodes.
    for (int i = 0; i < N; i++) {
        // Remove the remaining nodes.
        if (i % (N / 20) != 0) {
            TEST_CHECK(map_remove(map, key_array[i]));
        }
    }

    // Remove an already removed key.
    int removed = N / 2;
    TEST_CHECK(!map_remove(map, &removed));

    map_destroy(map);
    free(key_array);
    free(value_array);
}

void test_find() {
    Map map = map_create(compare_ints, free, free);
    map_set_hash_function(map, hash_int);

    int N = 1000;
    int** key_array = malloc(N * sizeof(*key_array));
    int** value_array = malloc(N * sizeof(*value_array));

    for (int i = 0; i < N; i++) {
        key_array[i] = create_int(i);
        value_array[i] = create_int(i);

        map_insert(map, key_array[i], value_array[i]);
        MapNode found = map_find_node(map, key_array[i]);
        void* found_key = map_node_key(map, found);
        void* found_val = map_node_value(map, found);

        TEST_CHECK(found != MAP_EOF);
        TEST_CHECK(found_key == key_array[i]);
        TEST_CHECK(found_val == value_array[i]);
    }

    // Search for non-existent key.
    int not_exists = 2000;
    TEST_CHECK(map_find_node(map, &not_exists) == MAP_EOF);
    TEST_CHECK(map_find(map, &not_exists) == NULL);

    map_destroy(map);
    free(key_array);
    free(value_array);
}

void test_iterate() {
    Map map = map_create(compare_ints, free, free);
    map_set_hash_function(map, hash_int);

    // Test for empty map.
    TEST_CHECK(map_first(map) == MAP_EOF);

    int N = 1000;
    for (int i = 0; i < N; i++) {
        // value is 2*i to guarantee that it is different from key in a predictable way.
        map_insert(map, create_int(i), create_int(2 * i));
    }

    // While traversing the map, check that a key is seen only once.
    bool seen[N];
    for (int i = 0; i < N; i++) {
        seen[i] = false;
    }

    for (MapNode node = map_first(map); node != MAP_EOF; node = map_next(map, node)) {
        int* key = map_node_key(map, node);
        int* value = map_node_value(map, node);

        TEST_CHECK(*key >= 0 && *key < N && !seen[*key]);
        TEST_CHECK(*value == 2 * *key);

        seen[*key] = true;
    }

    map_destroy(map);
}

TEST_LIST = {
    {"map_create", test_create},
    {"map_insert", test_insert},
    {"map_remove", test_remove},
    {"map_find", test_find},
    {"map_iterate", test_iterate},

    {NULL, NULL}  // End of tests.
};