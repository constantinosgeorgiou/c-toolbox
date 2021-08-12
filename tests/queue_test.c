#include "queue.h"

#include "acutest.h"

void test_create(void) {
    Queue queue = queue_create(free);

    TEST_CHECK(queue != NULL);
    TEST_CHECK(queue_size(queue) == 0);
    TEST_CHECK(queue_is_empty(queue) == true);

    DestroyFunc destroy_value = queue_set_destroy_value(queue, NULL);
    TEST_CHECK(destroy_value == free);

    queue_destroy(queue);
}

void test_enqueue(void) {
    Queue queue = queue_create(NULL);

    int N = 1000;
    int* array = malloc(N * sizeof(*array));

    for (int i = 0; i < N; i++) {
        queue_enqueue(queue, &array[i]);
        TEST_CHECK(queue_size(queue) == (i + 1));
        TEST_CHECK(queue_front(queue) == &array[0]);
        TEST_CHECK(queue_back(queue) == &array[i]);
    }

    queue_destroy(queue);
    free(array);
}

void test_dequeue(void) {
    Queue queue = queue_create(NULL);
    int N = 1000;
    int* array = malloc(N * sizeof(*array));

    for (int i = 0; i < N; i++) queue_enqueue(queue, &array[i]);

    for (int i = 0; i < N; i++) {
        TEST_CHECK(queue_front(queue) == &array[i]);
        queue_dequeue(queue);
        TEST_CHECK(queue_size(queue) == ((N - 1) - i));
    }

    queue_destroy(queue);
    free(array);
}

TEST_LIST = {
    {"queue_create", test_create},
    {"queue_enqueue", test_enqueue},
    {"queue_dequeue", test_dequeue},

    {NULL, NULL}  // End of tests.
};