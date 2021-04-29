//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT BList.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "ADTBidirectionalList.h"

#include "acutest.h"  // Απλή βιβλιοθήκη για unit testing

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
    BList blist = blist_create(NULL);

    TEST_CHECK(blist != NULL);
    TEST_CHECK(blist_size(blist) == 0);

    blist_destroy(blist);
}

void test_insert(void) {
    int N = 1000;

    // Insert at the beginning.
    BList blist = blist_create(free);
    for (int i = 0; i < N; i++) {
        BListNode inserted = blist_insert(blist, BLIST_BOF, create_int(i));

        BListNode first = blist_first(blist);
        int* value = blist_node_value(blist, first);

        TEST_CHECK(inserted == first);
        TEST_CHECK(*value == i);

        TEST_CHECK(blist_size(blist) == (i + 1));
    }
    blist_destroy(blist);

    // Insert at the end.
    blist = blist_create(free);
    for (int i = 0; i < N; i++) {
        BListNode inserted = blist_insert(blist, blist_last(blist), create_int(i));

        BListNode last = blist_last(blist);
        int* value = blist_node_value(blist, last);

        TEST_CHECK(inserted == last);
        TEST_CHECK(*value == i);

        TEST_CHECK(blist_size(blist) == (i + 1));
    }
    blist_destroy(blist);
}

void test_remove(void) {
    BList blist = blist_create(free);

    int N = 10;

    // Insert at the beginning. Remove from the beginning.
    for (int i = 0; i < N; i++) {
        blist_insert(blist, BLIST_BOF, create_int(i));
    }
    for (int i = N - 1; i >= 0; i--) {
        TEST_CHECK(*(int*)blist_node_value(blist, blist_first(blist)) == i);
        blist_remove(blist, blist_first(blist));
    }
    TEST_CHECK(blist_size(blist) == 0);

    // Insert at the beginning. Remove from the end.
    for (int i = 0; i < N; i++) {
        blist_insert(blist, BLIST_BOF, create_int(i));
    }
    for (int i = 0; i < N; i++) {
        TEST_CHECK(*(int*)blist_node_value(blist, blist_last(blist)) == i);
        blist_remove(blist, blist_last(blist));
    }
    TEST_CHECK(blist_size(blist) == 0);

    // Insert at the end. Remove from the beginning.
    for (int i = 0; i < N; i++) {
        blist_insert(blist, blist_last(blist), create_int(i));
    }
    for (int i = 0; i < N; i++) {
        TEST_CHECK(*(int*)blist_node_value(blist, blist_first(blist)) == i);
        blist_remove(blist, blist_first(blist));
    }
    TEST_CHECK(blist_size(blist) == 0);

    // Insert at the end. Remove from the end.
    for (int i = 0; i < N; i++) {
        blist_insert(blist, blist_last(blist), create_int(i));
    }
    for (int i = N - 1; i >= 0; i--) {
        TEST_CHECK(*(int*)blist_node_value(blist, blist_last(blist)) == i);
        blist_remove(blist, blist_last(blist));
    }
    TEST_CHECK(blist_size(blist) == 0);

    // Remove from the middle.
    for (int i = 0; i < N; i++) {
        blist_insert(blist, blist_last(blist), create_int(i));
    }
    BListNode middle = blist_first(blist);
    for (int i = 0; i < N / 2; i++) {
        middle = blist_next(blist, middle);
    }
    blist_remove(blist, middle);
    TEST_CHECK(blist_size(blist) == N - 1);

    blist_destroy(blist);
}

void test_find() {
    BList blist = blist_create(NULL);
    int N = 1000;
    int array[N];

    for (int i = 0; i < N; i++) {
        array[i] = i;
        blist_insert(blist, BLIST_BOF, &array[i]);
    }

    // Find all elements:
    for (int i = 0; i < N; i++) {
        int* value = blist_find(blist, &i, compare_ints);
        TEST_CHECK(value == &array[i]);
    }

    // Find a value that is not part of the bidirectional list:
    int not_exists = -1;
    TEST_CHECK(blist_find(blist, &not_exists, compare_ints) == NULL);

    blist_destroy(blist);
}

void test_find_node() {
    BList blist = blist_create(NULL);

    int N = 1000;
    int array[N];

    for (int i = 0; i < N; i++) {
        array[i] = i;
        blist_insert(blist, BLIST_BOF, &array[i]);
    }

    BListNode node = blist_first(blist);

    for (int i = N - 1; i >= 0; i--) {
        BListNode found_node = blist_find_node(blist, &i, compare_ints);

        TEST_CHECK(found_node == node);
        TEST_CHECK(blist_node_value(blist, found_node) == &array[i]);

        node = blist_next(blist, node);
    }

    blist_destroy(blist);
}

void test_concatenate(void) {
    int N = 10;
    int array[N];

    BList a = blist_create(NULL);
    BList b = blist_create(NULL);

    for (int i = 0; i < N / 2; i++) {
        array[i] = i;
        blist_insert(a, blist_last(a), &array[i]);
    }
    for (int i = N / 2; i < N; i++) {
        array[i] = i;
        blist_insert(b, blist_last(b), &array[i]);
    }

    BList concatenated = blist_concatenate(a, b);

    TEST_CHECK(concatenated != NULL);

    BListNode node = blist_first(concatenated);
    for (int i = 0; i < N; i++) {
        int* value = blist_node_value(concatenated, node);

        TEST_CHECK(value == &array[i]);
        TEST_CHECK(*value == array[i]);

        node = blist_next(concatenated, node);
    }

    blist_destroy(concatenated);
}

TEST_LIST = {
    {"create", test_create},
    {"insert", test_insert},
    {"remove", test_remove},
    {"find", test_find},
    {"find_node", test_find_node},
    {"concatenate", test_concatenate},

    {NULL, NULL}  // End of tests.
};