/// @file pqueue.c
///
/// Implementation of Priority Queue Abstract Data Type.
///
/// @note Nodes are 1-based.

#include "pqueue.h"

#include "vector.h"    // Vector, vector_create
#include "common_types.h" // DestroyFunc
#include <assert.h>       // assert

#include <stdbool.h> // bool
#include <stdlib.h>  // size_t, malloc

struct priority_queue {
    Vector data;
    CompareFunc compare;
    DestroyFunc destroy_value;
};

/// Returns the value of NODE
///
static void *node_value(PQueue pqueue, int node) {
    return vector_get_at(pqueue->data, node - 1); // Nodes start from 1.
}

/// Swaps values of NODE1 and NODE2
///
static void node_swap(PQueue pqueue, int node1, int node2) {
    void *value1 = node_value(pqueue, node1);
    void *value2 = node_value(pqueue, node2);

    // Nodes are 1-based.

    vector_set_at(pqueue->data, node1 - 1, value2);
    vector_set_at(pqueue->data, node2 - 1, value1);
}

/// Restores heap property.
///
/// All nodes preserve the heap property except NODE which is last and could be
/// greater than its parent.
///
/// @param node Last node of heap.
///
static void bubble_up(PQueue pqueue, int node) {
    if (node == 1) return;

    int parent = node / 2;

    if (pqueue->compare(node_value(pqueue, parent), node_value(pqueue, node)) <
        0) {
        node_swap(pqueue, parent, node);
        bubble_up(pqueue, parent);
    }
}

/// Restores heap property.
///
/// All nodes preserve the heap property except NODE which could be
/// lesser than its children.
///
static void bubble_down(PQueue pqueue, int node) {
    int left_child = 2 * node;
    int right_child = left_child + 1;

    size_t size = pqueue_size(pqueue);
    if (left_child > size) return;

    int max_child = left_child;
    if (right_child <= size &&
        pqueue->compare(node_value(pqueue, left_child),
                        node_value(pqueue, right_child)) < 0)
        max_child = right_child;

    if (pqueue->compare(node_value(pqueue, node),
                        node_value(pqueue, max_child)) < 0) {
        node_swap(pqueue, node, max_child);
        bubble_down(pqueue, max_child);
    }
}

static void heapify(PQueue pqueue, Vector values) {
    // Insert values into priority queue.
    size_t size = vector_size(values);
    for (int i = 0; i < size; i++)
        vector_insert_last(pqueue->data, vector_get_at(values, i));

    // Preserve heap property
    int last_internal_node = vector_size(pqueue->data) / 2;
    for (int i = last_internal_node; i > 0; i--)
        bubble_down(pqueue, i);
}

PQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value,
                     Vector values) {
    assert(compare != NULL);

    PQueue pqueue = malloc(sizeof(*pqueue));
    if (pqueue == NULL) return NULL;

    // destroy_value of vector pqueue->data is NULL because it will get called
    // when swapping two values.
    pqueue->data = vector_create(0, NULL);
    if (pqueue->data == NULL) return NULL;

    pqueue->compare = compare;
    pqueue->destroy_value = destroy_value;

    // Initialize heap if needed.
    if (values != NULL) heapify(pqueue, values);

    return pqueue;
}

void pqueue_destroy(PQueue pqueue) {
    vector_set_destroy_value(pqueue->data, pqueue->destroy_value);
    vector_destroy(pqueue->data);
    free(pqueue);
}

void *pqueue_peek(PQueue pqueue) { return node_value(pqueue, 1); }

void pqueue_insert(PQueue pqueue, void *element) {
    vector_insert_last(pqueue->data, element);

    // All nodes preserve the heap property except the last node which is
    // greater than its parent.
    // pqeueu_size() as node in bubble_up() because nodes are 1-based, so new
    // pqueue size is the desired node.
    bubble_up(pqueue, pqueue_size(pqueue));
}

void pqueue_pull(PQueue pqueue) {
    int last_node = pqueue_size(pqueue);
    assert(last_node != 0);

    if (pqueue->destroy_value != NULL)
        pqueue->destroy_value(pqueue_peek(pqueue));

    node_swap(pqueue, 1, last_node);
    vector_remove_last(pqueue->data);

    // All nodes preserve the heap property except the new root which could be
    // lesser than its children.
    // 1 as node in bubble_down() because nodes are 1-based.
    bubble_down(pqueue, 1);
}

size_t pqueue_size(PQueue pqueue) { return vector_size(pqueue->data); }

bool pqueue_is_empty(PQueue pqueue) {
    return vector_size(pqueue->data) != 0 ? false : true;
}

DestroyFunc pqueue_set_destroy_value(PQueue pqueue, DestroyFunc destroy_value) {
    DestroyFunc old = pqueue->destroy_value;
    pqueue->destroy_value = destroy_value;
    return old;
}
