#include "pqueue.h"

#include <stdbool.h>

#include "acutest.h"
#include "test_companion.h"
#include "vector.h"

void test_create(void) {
  PQueue pqueue = pqueue_create(compare_ints, free, NULL);

  TEST_CHECK(pqueue != NULL);
  TEST_CHECK(pqueue_size(pqueue) == 0);
  TEST_CHECK(pqueue_is_empty(pqueue) == true);

  DestroyFunc destroy_value = pqueue_set_destroy_value(pqueue, NULL);
  TEST_CHECK(destroy_value == free);

  pqueue_destroy(pqueue);

  // Create with initial values.

  int N = 5;

  Vector values = vector_create(0, NULL);
  for (int i = 0; i < N; i++) {
    vector_insert_last(values, create_int(i));
  }

  pqueue = pqueue_create(compare_ints, free, values);

  TEST_CHECK(pqueue != NULL);
  TEST_CHECK(pqueue_size(pqueue) == N);
  TEST_CHECK(pqueue_is_empty(pqueue) == false);

  TEST_CHECK(*(int *)pqueue_peek(pqueue) == N - 1);
  pqueue_pull(pqueue);
  TEST_CHECK(*(int *)pqueue_peek(pqueue) == N - 2);

  vector_destroy(values);
  pqueue_destroy(pqueue);
}

void test_insert(void) {
  PQueue pqueue = pqueue_create(compare_ints, NULL, NULL);
  int N = 1000;
  int *array = malloc(N * sizeof(*array));

  for (int i = 0; i < N; i++) {
    array[i] = i;
    pqueue_insert(pqueue, &array[i]);
    TEST_CHECK(pqueue_size(pqueue) == i + 1);
    TEST_CHECK(pqueue_peek(pqueue) == &array[i]);
  }

  pqueue_destroy(pqueue);
  free(array);
}

void test_pull(void) {
  PQueue pqueue = pqueue_create(compare_ints, free, NULL);

  int N = 10;
  int **array = malloc(N * sizeof(*array));
  for (int i = 0; i < N; i++)
    array[i] = create_int(i);
  shuffle(array, N);

  for (int i = 0; i < N; i++)
    pqueue_insert(pqueue, array[i]);

  for (int i = N - 1; i >= 0; i--) {
    int *value = pqueue_peek(pqueue);
    TEST_CHECK(*value == i);
    TEST_CHECK(pqueue_peek(pqueue) == value);
    pqueue_pull(pqueue);
    TEST_CHECK(pqueue_size(pqueue) == i);
  }

  pqueue_destroy(pqueue);

  pqueue = pqueue_create(compare_ints, NULL, NULL);
  pqueue_insert(pqueue, &N);
  TEST_CHECK(pqueue_peek(pqueue) == &N);
  pqueue_pull(pqueue);
  pqueue_destroy(pqueue);
  free(array);
}

TEST_LIST = {
    {"pqueue_create", test_create},
    {"pqueue_insert", test_insert},
    {"pqueue_pull", test_pull},

    {NULL, NULL} // End of tests.
};