#include "ADTOrderedSet.h"

#include <assert.h>   // assert
#include <stdbool.h>  // true, false
#include <stdlib.h>   // malloc, free, sizeof
#include <string.h>

/// @brief Levels of forward pointers a node can have.
///
/// Start at 16 levels, because the Oredered Set can contain 2^16 elements which is plenty.
/// In the event the Oredered Set has 2^16 elements, double its max level.
///
#define OSET_LEVELS 16

struct ordered_set {
    CompareFunc compare;
    DestroyFunc destroy_key;
    DestroyFunc destroy_value;

    int max_level;  // Maximum level possible for a node of the Ordered Set.
                    // Oredered Set can have 2^max_level, if that number is reached, then
                    // double the max_level.

    int size;  // Number of elements in the Ordered Set.

    OrderedSetNode first;
    OrderedSetNode last;

    OrderedSetNode header;
};

struct ordered_set_node {
    OrderedSetNode* forward;
    OrderedSetNode previous;

    int* width;      // Number of bottom layer links being traversed by each of the forward links.
    int levels;      // Number of forward links.
    bool is_header;  // true, if node is header, otherwise false.

    void* key;
    void* value;
};

/// @brief Creates and returns an Ordered Set node.
///
/// @param levels Number of forward links.
/// @param is_header true if node is header node, else false.
///
static OrderedSetNode node_create(void* key, void* value, int levels, bool is_header) {
    OrderedSetNode node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    // Allocate forward array and initialize it to NULL.
    node->forward = calloc(levels * sizeof(*node->forward), levels);
    if (node->forward == NULL) {
        return NULL;
    }

    // Allocate width array.
    node->width = malloc(levels * sizeof(*node->width));
    if (node->width == NULL) {
        return NULL;
    }

    node->levels = levels;
    node->previous = OSET_BOF;
    node->is_header = is_header;

    node->key = key;
    node->value = value;

    return node;
}

/// @brief Frees all the memory allocated by given Ordered Set node.
///
/// Any operation on the Ordered Set node after its destruction, results in undefined behaviour.
///
static void node_destroy(OrderedSetNode node, DestroyFunc destroy_key, DestroyFunc destroy_value) {
    if (destroy_key != NULL) destroy_key(node->key);
    if (destroy_value != NULL) destroy_value(node->value);
    free(node->forward);
    free(node->width);
    free(node);
}

OrderedSet oset_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
    OrderedSet oset = malloc(sizeof(*oset));
    if (oset == NULL) {
        return OSET_ERROR;
    }

    oset->compare = compare;
    oset->destroy_key = destroy_key;
    oset->destroy_value = destroy_value;

    oset->max_level = OSET_LEVELS;
    oset->size = 0;

    oset->first = OSET_EOF;
    oset->last = OSET_EOF;

    // Header nodes don't need to have neither keys nor values.
    oset->header = node_create(OSET_BOF, OSET_BOF, oset->max_level, true);
    if (oset->header == NULL) {
        return OSET_ERROR;
    }

    return oset;
}

void oset_destroy(OrderedSet oset) {
    OrderedSetNode node = oset->header;

    while (node != OSET_EOF) {
        OrderedSetNode next = node->forward[0];

        node_destroy(node, oset->destroy_key, oset->destroy_value);

        node = next;
    }

    free(oset);
}

DestroyFunc oset_set_destroy_key(OrderedSet oset, DestroyFunc destroy_key) {
    DestroyFunc old = oset->destroy_key;
    oset->destroy_key = destroy_key;
    return old;
}

DestroyFunc oset_set_destroy_value(OrderedSet oset, DestroyFunc destroy_value) {
    DestroyFunc old = oset->destroy_value;
    oset->destroy_value = destroy_value;
    return old;
}

int oset_size(OrderedSet oset) { return oset->size; }

void oset_insert(OrderedSet oset, void* key, void* value) {
    assert(key != NULL);
}

bool oset_remove(OrderedSet oset, void* key) {
    assert(key != NULL);

    return false;
}

void* oset_find(OrderedSet oset, void* key) {
    assert(key != NULL);
    OrderedSetNode node = oset_find_node(oset, key);
    return node != NULL ? node->value : NULL;
}

void* oset_get_at(OrderedSet oset, int pos) { return NULL; }

bool oset_remove_at(OrderedSet oset, int pos) { return false; }

OrderedSet oset_split(OrderedSet oset, void* split_key) { return OSET_ERROR; }

void oset_merge(OrderedSet a, OrderedSet b) {}

void oset_concat(OrderedSet a, OrderedSet b) {}

OrderedSetNode oset_find_node(OrderedSet oset, void* key) {
    assert(key != NULL);

    // OrderedSetNode node = node_find_previous(oset, key);

    // if (node->next != OSET_EOF && oset->compare(node->next->key, key) == 0) {
    //     return node->next;
    // }

    return OSET_EOF;
}

OrderedSetNode oset_get_at_node(OrderedSet oset, int pos) { return OSET_EOF; }

void* oset_node_key(OrderedSet oset, OrderedSetNode node) {
    assert(node != NULL);
    return node->key;
}

void* oset_node_value(OrderedSet oset, OrderedSetNode node) {
    assert(node != NULL);
    return node->value;
}

OrderedSetNode oset_first(OrderedSet oset) { return oset->first; }

OrderedSetNode oset_last(OrderedSet oset) { return oset->last; }

OrderedSetNode oset_next(OrderedSet oset, OrderedSetNode node) {
    assert(node != NULL);
    // return node->next;
    return NULL;
}

OrderedSetNode oset_previous(OrderedSet oset, OrderedSetNode node) {
    assert(node != NULL);
    return node->previous->is_header ? OSET_BOF : node->previous;
}
