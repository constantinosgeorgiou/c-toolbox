//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT BList.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "ADTBidirectionalList.h"

#include "acutest.h"  // Απλή βιβλιοθήκη για unit testing

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

TEST_LIST = {
    {"create", test_create},
    {"insert", test_insert},
    {"remove", test_remove},

    {NULL, NULL}  // End of tests.
};