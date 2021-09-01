#include "slist.h"

#include "acutest.h"

#include "test_companion.h"

void test_create(void) {
  SList slist = slist_create(free);
  DestroyFunc destroy_value = slist_set_destroy_value(slist, NULL);

  TEST_CHECK(slist != NULL);
  TEST_CHECK(slist_size(slist) == 0);
  TEST_CHECK(destroy_value == free);

  slist_destroy(slist);
}

void test_insert(void) {
  int N = 10;

  SList slist = slist_create(free);

  for (int i = 0; i < N; ++i) {
    // Insert at the beginning.
    slist_insert_next(slist, SLIST_BOF, create_int(i));

    int *value = slist_node_value(slist, slist_first(slist));

    TEST_CHECK(*value == i);
    TEST_CHECK(slist_size(slist) == (i + 1));
  }

  // Check if elements were inserted in descending order:
  SListNode node = slist_first(slist);
  for (int i = N - 1; i >= 0; i--) {
    int *value = slist_node_value(slist, node);
    TEST_CHECK(*value == i);
    node = slist_next(slist, node);
  }

  // Destroy to test insertion at the end:
  slist_destroy(slist);

  slist = slist_create(free);

  for (int i = 0; i < N; i++) {
    // Insert at the end.
    slist_insert_next(slist, slist_last(slist), create_int(i));

    int *value = slist_node_value(slist, slist_last(slist));

    TEST_CHECK(*value == i);
    TEST_CHECK(slist_size(slist) == (i + 1));
  }

  // Check if elements were inserted in ascending order:
  node = slist_first(slist);
  for (int i = 0; i < N; i++) {
    TEST_CHECK(*(int *)slist_node_value(slist, node) == i);
    node = slist_next(slist, node);
  }

  // Check in-between insertion by inserting a NULL node after the first node.
  SListNode first_node = slist_first(slist);
  slist_insert_next(slist, first_node, NULL);
  TEST_CHECK(slist_node_value(slist, slist_next(slist, first_node)) == NULL);

  slist_destroy(slist);
}

void test_remove_next(void) {
  int N = 10;

  SList slist = slist_create(free);

  // Insert at the beginning. Remove from the beginning.
  for (int i = 0; i < N; i++) {
    slist_insert_next(slist, SLIST_BOF, create_int(i));
  }
  for (int i = N - 1; i >= 0; i--) {
    TEST_CHECK(*(int *)slist_node_value(slist, slist_first(slist)) == i);
    slist_remove_next(slist, SLIST_BOF);
  }
  TEST_CHECK(slist_size(slist) == 0);

  // Insert at the end. Remove from the beginning.
  for (int i = 0; i < N; i++) {
    slist_insert_next(slist, slist_last(slist), create_int(i));
  }
  for (int i = 0; i < N; i++) {
    TEST_CHECK(*(int *)slist_node_value(slist, slist_first(slist)) == i);
    slist_remove_next(slist, SLIST_BOF);
  }
  TEST_CHECK(slist_size(slist) == 0);

  // Remove from the middle.
  for (int i = 0; i < N; i++) {
    slist_insert_next(slist, slist_last(slist), create_int(i));
  }
  SListNode middle = slist_first(slist);
  for (int i = 0; i < N / 2; i++) {
    middle = slist_next(slist, middle);
  }
  slist_remove_next(slist, middle);
  TEST_CHECK(slist_size(slist) == N - 1);

  slist_destroy(slist);
}

void test_find() {
  int N = 1000;
  int array[N];

  SList slist = slist_create(NULL);

  for (int i = 0; i < N; i++) {
    array[i] = i;
    slist_insert_next(slist, SLIST_BOF, &array[i]);
  }

  // Find all elements:
  for (int i = 0; i < N; i++) {
    int *value = slist_find(slist, &i, compare_ints);
    TEST_CHECK(value == &array[i]);
  }

  // Find a value that is not part of the slist:
  int not_exists = -1;
  TEST_CHECK(slist_find(slist, &not_exists, compare_ints) == NULL);

  slist_destroy(slist);
}

void test_find_node() {
  int N = 1000;
  int array[N];

  SList slist = slist_create(NULL);

  for (int i = 0; i < N; i++) {
    array[i] = i;
    slist_insert_next(slist, SLIST_BOF, &array[i]);
  }

  SListNode node = slist_first(slist);

  for (int i = N - 1; i >= 0; i--) {
    SListNode found_node = slist_find_node(slist, &i, compare_ints);

    TEST_CHECK(found_node == node);
    TEST_CHECK(slist_node_value(slist, found_node) == &array[i]);

    node = slist_next(slist, node);
  }

  slist_destroy(slist);
}

void test_concatenate(void) {
  int N = 10;
  int array[N];

  SList a = slist_create(NULL);
  SList b = slist_create(NULL);

  for (int i = 0; i < N / 2; i++) {
    array[i] = i;
    slist_insert_next(a, slist_last(a), &array[i]);
  }
  for (int i = N / 2; i < N; i++) {
    array[i] = i;
    slist_insert_next(b, slist_last(b), &array[i]);
  }

  SList concatenated = slist_concat(a, b);

  TEST_CHECK(concatenated != NULL);

  // Traverse slist:
  SListNode node = slist_first(concatenated);
  for (int i = 0; i < N; i++) {
    int *value = slist_node_value(concatenated, node);

    TEST_CHECK(value == &array[i]);
    TEST_CHECK(*value == array[i]);

    node = slist_next(concatenated, node);
  }

  slist_destroy(concatenated);
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

    {NULL, NULL} // End of tests.
};