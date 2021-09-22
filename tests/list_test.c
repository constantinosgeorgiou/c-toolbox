////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file ADTBidirectionalList_test.c
/// @author Constantinos Georgiou
/// @brief Unit tests for implementation of ADT Bidirectional List.
/// @version 1.0
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "list.h"

#include "acutest.h"

#include "test_companion.h"

void test_create(void) {
  List list = list_create(NULL);
  DestroyFunc destroy = list_set_destroy_value(list, NULL);

  TEST_CHECK(list != NULL);
  TEST_CHECK(list_size(list) == 0);
  TEST_CHECK(destroy == NULL);

  list_destroy(list);
}

void test_insert(void) {
  int N = 1000;

  // Insert at the beginning.
  List list = list_create(free);
  for (int i = 0; i < N; i++) {
    list_insert(list, LIST_BOF, create_int(i));

    ListNode first = list_first(list);
    int *value = list_node_value(list, first);

    TEST_CHECK(*value == i);

    TEST_CHECK(list_size(list) == (i + 1));
  }
  list_destroy(list);

  // Insert at the end.
  list = list_create(free);
  for (int i = 0; i < N; i++) {
    list_insert(list, list_last(list), create_int(i));

    ListNode last = list_last(list);
    int *value = list_node_value(list, last);

    TEST_CHECK(*value == i);

    TEST_CHECK(list_size(list) == (i + 1));
  }
  list_destroy(list);
}

void test_remove(void) {
  List list = list_create(free);

  int N = 10;

  // Insert at the beginning. Remove from the beginning.
  for (int i = 0; i < N; i++) {
    list_insert(list, LIST_BOF, create_int(i));
  }
  for (int i = N - 1; i >= 0; i--) {
    TEST_CHECK(*(int *)list_node_value(list, list_first(list)) == i);
    list_remove(list, list_first(list));
  }
  TEST_CHECK(list_size(list) == 0);

  // Insert at the beginning. Remove from the end.
  for (int i = 0; i < N; i++) {
    list_insert(list, LIST_BOF, create_int(i));
  }
  for (int i = 0; i < N; i++) {
    TEST_CHECK(*(int *)list_node_value(list, list_last(list)) == i);
    list_remove(list, list_last(list));
  }
  TEST_CHECK(list_size(list) == 0);

  // Insert at the end. Remove from the beginning.
  for (int i = 0; i < N; i++) {
    list_insert(list, list_last(list), create_int(i));
  }
  for (int i = 0; i < N; i++) {
    TEST_CHECK(*(int *)list_node_value(list, list_first(list)) == i);
    list_remove(list, list_first(list));
  }
  TEST_CHECK(list_size(list) == 0);

  // Insert at the end. Remove from the end.
  for (int i = 0; i < N; i++) {
    list_insert(list, list_last(list), create_int(i));
  }
  for (int i = N - 1; i >= 0; i--) {
    TEST_CHECK(*(int *)list_node_value(list, list_last(list)) == i);
    list_remove(list, list_last(list));
  }
  TEST_CHECK(list_size(list) == 0);

  // Remove from the middle.
  for (int i = 0; i < N; i++) {
    list_insert(list, list_last(list), create_int(i));
  }
  ListNode middle = list_first(list);
  for (int i = 0; i < N / 2; i++) {
    middle = list_next(list, middle);
  }
  list_remove(list, middle);
  TEST_CHECK(list_size(list) == N - 1);

  list_destroy(list);
}

void test_find() {
  List list = list_create(NULL);
  int N = 1000;
  int array[N];

  for (int i = 0; i < N; i++) {
    array[i] = i;
    list_insert(list, LIST_BOF, &array[i]);
  }

  // Find all elements:
  for (int i = 0; i < N; i++) {
    int *value = list_find(list, &i, compare_ints);
    TEST_CHECK(value == &array[i]);
  }

  // Find a value that is not part of the bidirectional list:
  int not_exists = -1;
  TEST_CHECK(list_find(list, &not_exists, compare_ints) == NULL);

  list_destroy(list);
}

void test_find_node() {
  List list = list_create(NULL);

  int N = 1000;
  int array[N];

  for (int i = 0; i < N; i++) {
    array[i] = i;
    list_insert(list, LIST_BOF, &array[i]);
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
    list_insert(a, list_last(a), &array[i]);
  }
  for (int i = N / 2; i < N; i++) {
    array[i] = i;
    list_insert(b, list_last(b), &array[i]);
  }

  list_concat(a, b);

  TEST_CHECK(a != NULL);

  // Traverse bidirectional list:
  ListNode node = list_first(a);
  for (int i = 0; i < N; i++) {
    int *value = list_node_value(a, node);

    TEST_CHECK(value == &array[i]);
    TEST_CHECK(*value == array[i]);

    node = list_next(a, node);
  }

  // Traverse bidirectional list in REVERSE:
  node = list_last(a);
  for (int i = N - 1; i >= 0; i--) {
    int *value = list_node_value(a, node);

    TEST_CHECK(value == &array[i]);
    TEST_CHECK(*value == array[i]);

    node = list_previous(a, node);
  }

  list_destroy(a);
}

void test_traverse(void) {
  int N = 10;
  int array[N];

  List list = list_create(NULL);

  for (int i = 0; i < N; i++) {
    array[i] = i;
    list_insert(list, list_last(list), &array[i]);
  }

  // Traverse bidirectional list:
  ListNode node = list_first(list);
  for (int i = 0; i < N; i++) {
    int *value = list_node_value(list, node);

    TEST_CHECK(value == &array[i]);
    TEST_CHECK(*value == array[i]);

    node = list_next(list, node);
  }

  // Traverse bidirectional list in REVERSE:
  node = list_last(list);
  for (int i = N - 1; i >= 0; i--) {
    int *value = list_node_value(list, node);

    TEST_CHECK(value == &array[i]);
    TEST_CHECK(*value == array[i]);

    node = list_previous(list, node);
  }

  list_destroy(list);
}

void test_get_at(void) {
  int N = 10;
  int array[N];

  List list = list_create(NULL);

  for (int i = 0; i < N; i++) {
    array[i] = i;
    list_insert(list, list_last(list), &array[i]);
  }

  for (int i = 0; i < N; i++) {
    int *value = list_get_at(list, i);

    TEST_CHECK(value == &array[i]);
    TEST_CHECK(*value == array[i]);
  }

  list_destroy(list);
}

TEST_LIST = {
    {"create", test_create},
    {"insert", test_insert},
    {"remove", test_remove},
    {"find", test_find},
    {"find_node", test_find_node},
    {"concatenate", test_concatenate},
    {"traverse", test_traverse},
    {"get_at", test_get_at},
    {NULL, NULL} // End of tests.
};