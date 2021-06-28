#include "ADTOrderedSet.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct ordered_set {
    CompareFunc compare;
    DestroyFunc destroy_key;
    DestroyFunc destroy_value;
    int size;

    OrderedSetNode first;
    OrderedSetNode last;

    OrderedSetNode header;
};

struct ordered_set_node {
    OrderedSetNode top;
    OrderedSetNode bottom;
    OrderedSetNode next;
    OrderedSetNode previous;

    void* value;
};

/// @brief Creates and returns an Ordered Set node with given value.
///
static OrderedSetNode oset_create_node(void* value) {
    OrderedSetNode node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    node->top = OSET_EOF;
    node->bottom = OSET_EOF;
    node->next = OSET_EOF;
    node->previous = OSET_EOF;

    node->value = value;

    return node;
}

OrderedSet oset_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
    OrderedSet oset = malloc(sizeof(*oset));
    if (oset == NULL) {
        return OSET_ERROR;
    }

    oset->compare = compare;
    oset->destroy_key = destroy_key;
    oset->destroy_value = destroy_value;

    oset->first = OSET_EOF;
    oset->last = OSET_EOF;
    oset->size = 0;

    oset->header = oset_create_node(strdup("H"));
    if (oset->header == NULL) {
        return OSET_ERROR;
    }

    return oset;
}

void oset_destroy(OrderedSet oset) {}

DestroyFunc oset_set_destroy_key(OrderedSet oset, DestroyFunc destroy_key) { return NULL; }

DestroyFunc oset_set_destroy_value(OrderedSet oset, DestroyFunc destroy_value) { return NULL; }

int oset_size(OrderedSet oset) { return -1; }

void oset_insert(OrderedSet oset, void* key, void* value) {}

bool oset_remove(OrderedSet oset, void* key) { return false; }

void* oset_find(OrderedSet oset, void* key) { return NULL; }

void* oset_get_at(OrderedSet oset, int pos) { return NULL; }

bool oset_remove_at(OrderedSet oset, int pos) { return false; }

OrderedSet oset_split(OrderedSet oset, void* split_key) { return OSET_ERROR; }

void oset_merge(OrderedSet a, OrderedSet b) {}

void oset_concat(OrderedSet a, OrderedSet b) {}

OrderedSetNode oset_find_node(OrderedSet oset, void* key) { return OSET_EOF; }

OrderedSetNode oset_get_at_node(OrderedSet oset, int pos) { return OSET_EOF; }

void* oset_node_key(OrderedSet oset, OrderedSetNode node) { return NULL; }

void* oset_node_value(OrderedSet oset, OrderedSetNode node) { return NULL; }

OrderedSetNode oset_first(OrderedSet oset) { return OSET_EOF; }

OrderedSetNode oset_last(OrderedSet oset) { return OSET_EOF; }

OrderedSetNode oset_next(OrderedSet oset, OrderedSetNode node) { return OSET_EOF; }

OrderedSetNode oset_previous(OrderedSet oset, OrderedSetNode node) { return OSET_EOF; }
