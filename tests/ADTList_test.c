////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file ADTList_test.c
/// @author Constantinos Georgiou
/// @brief Unit tests for implementation of ADT List.
/// @version 1.0
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ADTList.h"

#include "acutest.h"

/// @brief Compares two int*.
///
/// @return < 0, if a < b, or, > 0, if b < a, or, 0, if a and b are equivalent
///
static int compare_ints(const void* a, const void* b) { return *(int*)a - *(int*)b; }

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

void test_create(void) {
    List list = list_create(NULL);
    DestroyFunc destroy = list_set_destroy_value(list, NULL);

    TEST_CHECK(list != NULL);
    TEST_CHECK(list_size(list) == 0);
    TEST_CHECK(destroy == NULL);

    list_destroy(list);
}

void test_insert(void) {
    int N = 10;

    // Insert at the beginning.
    List list = list_create(free);
    for (int i = 0; i < N; i++) {
        ListNode inserted = list_insert_next(list, LIST_BOF, create_int(i));

        if (TEST_CHECK(inserted != NULL)) {
            ListNode first = list_first(list);
            int* value = list_node_value(list, inserted);
            TEST_CHECK(inserted == first);
            TEST_CHECK(*value == i);
            TEST_CHECK(list_size(list) == (i + 1));
        }
    }

    // Check if elements were inserted in descending order:
    ListNode node = list_first(list);
    for (int i = N - 1; i >= 0; i--) {
        int* value = list_node_value(list, node);
        TEST_CHECK(*value == i);
        node = list_next(list, node);
    }

    // Check last element:
    TEST_CHECK(*(int*)list_node_value(list, list_last(list)) == 0);

    // Destroy to test insertion at the end:
    list_destroy(list);

    // Insert at the end.
    list = list_create(free);
    for (int i = 0; i < N; i++) {
        ListNode inserted = list_insert_next(list, list_last(list), create_int(i));

        if (TEST_CHECK(inserted != NULL)) {
            ListNode last = list_last(list);
            int* value = list_node_value(list, inserted);

            TEST_CHECK(inserted == last);
            TEST_CHECK(*value == i);
            TEST_CHECK(list_size(list) == (i + 1));
        }
    }

    // Check if elements were inserted in ascending order:
    node = list_first(list);
    for (int i = 0; i < N; i++) {
        TEST_CHECK(*(int*)list_node_value(list, node) == i);
        node = list_next(list, node);
    }

    // Check last element:
    TEST_CHECK(*(int*)list_node_value(list, list_last(list)) == (N - 1));

    // Check in-between insertion by inserting a NULL node after the first node.
    ListNode first_node = list_first(list);
    list_insert_next(list, first_node, NULL);
    TEST_CHECK(list_node_value(list, list_next(list, first_node)) == NULL);

    list_destroy(list);
}

void test_remove_next(void) {
    List list = list_create(free);

    int N = 10;

    // Insert at the beginning. Remove from the beginning.
    for (int i = 0; i < N; i++) {
        list_insert_next(list, LIST_BOF, create_int(i));
    }
    for (int i = N - 1; i >= 0; i--) {
        TEST_CHECK(*(int*)list_node_value(list, list_first(list)) == i);
        list_remove_next(list, LIST_BOF);
    }
    TEST_CHECK(list_size(list) == 0);

    // Insert at the end. Remove from the beginning.
    for (int i = 0; i < N; i++) {
        list_insert_next(list, list_last(list), create_int(i));
    }
    for (int i = 0; i < N; i++) {
        TEST_CHECK(*(int*)list_node_value(list, list_first(list)) == i);
        list_remove_next(list, LIST_BOF);
    }
    TEST_CHECK(list_size(list) == 0);

    // Remove from the middle.
    for (int i = 0; i < N; i++) {
        list_insert_next(list, list_last(list), create_int(i));
    }
    ListNode middle = list_first(list);
    for (int i = 0; i < N / 2; i++) {
        middle = list_next(list, middle);
    }
    list_remove_next(list, middle);
    TEST_CHECK(list_size(list) == N - 1);

    list_destroy(list);
}

void test_find() {
    int N = 1000;
    int array[N];

    List list = list_create(NULL);

    for (int i = 0; i < N; i++) {
        array[i] = i;
        list_insert_next(list, LIST_BOF, &array[i]);
    }

    // Find all elements:
    for (int i = 0; i < N; i++) {
        int* value = list_find(list, &i, compare_ints);
        TEST_CHECK(value == &array[i]);
    }

    // Find a value that is not part of the list:
    int not_exists = -1;
    TEST_CHECK(list_find(list, &not_exists, compare_ints) == NULL);

    list_destroy(list);
}

void test_find_node() {
    int N = 1000;
    int array[N];

    List list = list_create(NULL);

    for (int i = 0; i < N; i++) {
        array[i] = i;
        list_insert_next(list, LIST_BOF, &array[i]);
    }

    ListNode node = list_first(list);

    for (int i = N - 1; i >= 0; i--) {
        ListNode found_node = list_find_node(list, &i, compare_ints);

        TEST_CHECK(found_node == node);
        TEST_CHECK(list_node_value(list, found_node) == &array[i]);

        node = list_next(list, node);
    }

    list_destroy(list);
}

void test_concatenate(void) {
    int N = 10;
    int array[N];

    List a = list_create(NULL);
    List b = list_create(NULL);

    for (int i = 0; i < N / 2; i++) {
        array[i] = i;
        list_insert_next(a, list_last(a), &array[i]);
    }
    for (int i = N / 2; i < N; i++) {
        array[i] = i;
        list_insert_next(b, list_last(b), &array[i]);
    }

    List concatenated = list_concatenate(a, b);

    TEST_CHECK(concatenated != NULL);

    // Traverse list:
    ListNode node = list_first(concatenated);
    for (int i = 0; i < N; i++) {
        int* value = list_node_value(concatenated, node);

        TEST_CHECK(value == &array[i]);
        TEST_CHECK(*value == array[i]);

        node = list_next(concatenated, node);
    }

    list_destroy(concatenated);
}

void test_traverse(void) {}
void test_get_at(void) {}

TEST_LIST = {
    {"create", test_create},
    {"insert", test_insert},
    {"remove", test_remove_next},
    {"find", test_find},
    {"find_node", test_find_node},
    {"concatenate", test_concatenate},
    {"traverse", test_traverse},
    {"get_at", test_get_at},
    {NULL, NULL}  // End of tests.
};