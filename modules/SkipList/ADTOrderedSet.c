#include "ADTOrderedSet.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define OSET_TOF (OrderedSetNode)0  // Defines the virtual topmost level of the Ordered Set.
#define OSET_LOF (OrderedSetNode)0  // Defines the virtual lowermost level of the Ordered Set.

// TODO: Replace rand()

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

    bool is_header;
    int level;

    void* key;
    void* value;
};

enum coin {
    TAILS,
    HEADS
};

static enum coin coinflip() {
    return rand() % 2 ? HEADS : TAILS;
}

/// @brief Creates and returns an Ordered Set node.
///
/// @param level The height of the node.
/// @param is_header true if node is header node, else false.
///
static OrderedSetNode node_create(void* key, void* value, int level, bool is_header) {
    OrderedSetNode node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    node->top = OSET_EOF;
    node->bottom = OSET_EOF;
    node->next = OSET_EOF;
    node->previous = OSET_EOF;

    node->is_header = is_header;
    node->level = level;

    node->key = key;
    node->value = value;

    return node;
}

/// @brief Frees all the memory allocated by given Ordered Set node.
///
/// Any operation on the Ordered Set node after its destruction, results in undefined behaviour.
///
static void node_destroy(OrderedSetNode node, DestroyFunc destroy_key, DestroyFunc destroy_value) {
    if (!node->is_header) {
        if (destroy_key != NULL) {
            destroy_key(node->key);
        }
        if (destroy_value != NULL) {
            destroy_value(node->value);
        }
    }

    free(node);
}

static OrderedSetNode node_find_previous(OrderedSet oset, void* key) {
    OrderedSetNode target = OSET_EOF;  // Points to previous node of node with node->key < key.

    // Traverse from top to botom level.
    OrderedSetNode node = oset->header;
    while (node != OSET_LOF) {
        // Traverse nodes in level with node->key < key.
        while (node->next != OSET_EOF && oset->compare(node->next->key, key) < 0) {
            node = node->next;
        }

        target = node;

        node = node->bottom;
    }

    return target;
}

static void level_create(OrderedSet oset) {
    OrderedSetNode new_header = node_create(
        OSET_BOF, OSET_BOF, 1 + oset->header->level, true);

    OrderedSetNode old_header = oset->header;

    // Connect new_header with old_header verticaly.
    new_header->bottom = old_header;
    old_header->top = new_header;

    // Update Ordered Set header.
    oset->header = new_header;
}

static void node_promote(OrderedSet oset, OrderedSetNode node) {
    OrderedSetNode target = node->previous;
    while (target != OSET_BOF && target->top == OSET_TOF) {
        target = target->previous;
    }
    if (target == OSET_BOF) {
        level_create(oset);
        target = oset->header;
    } else {
        target = target->top;
    }

    OrderedSetNode new_node = node_create(
        node->key, node->value, 1 + node->level, node->is_header);

    // Insert new_node after target.
    new_node->next = target->next;
    new_node->previous = target;
    target->next = new_node;
    if (new_node->next != OSET_EOF) {
        new_node->next->previous = new_node;
    }

    // Connect new_node and node verticaly.
    node->top = new_node;
    new_node->bottom = node;
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

    // Header nodes don't need to have neither keys nor values.
    oset->header = node_create(OSET_BOF, OSET_BOF, 0, true);
    if (oset->header == NULL) {
        return OSET_ERROR;
    }

    return oset;
}

void oset_destroy(OrderedSet oset) {
    OrderedSetNode node = oset->header;
    while (node != OSET_EOF) {
        OrderedSetNode bottom = node->bottom;

        while (node != OSET_EOF) {
            OrderedSetNode next = node->next;

            node_destroy(node, oset->destroy_key, oset->destroy_value);

            node = next;
        }

        node = bottom;
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
    OrderedSetNode node = node_find_previous(oset, key);

    OrderedSetNode new_node = node_create(key, value, 0, false);

    // Insert new_node after node.
    new_node->next = node->next;
    new_node->previous = node;
    node->next = new_node;
    if (new_node->next != OSET_EOF) {
        new_node->next->previous = new_node;
    }

    // Promote new_node into top levels.
    OrderedSetNode promotion = new_node;
    while (coinflip() == HEADS) {
        node_promote(oset, promotion);
        promotion = promotion->top;
    }

    // Update first pointer.
    // Update last pointer.
    // Update size.
}

bool oset_remove(OrderedSet oset, void* key) { return false; }

void* oset_find(OrderedSet oset, void* key) { return NULL; }

void* oset_get_at(OrderedSet oset, int pos) { return NULL; }

bool oset_remove_at(OrderedSet oset, int pos) { return false; }

OrderedSet oset_split(OrderedSet oset, void* split_key) { return OSET_ERROR; }

void oset_merge(OrderedSet a, OrderedSet b) {}

void oset_concat(OrderedSet a, OrderedSet b) {}

OrderedSetNode oset_find_node(OrderedSet oset, void* key) {
    OrderedSetNode node = node_find_previous(oset, key);

    if (node->next != OSET_EOF && oset->compare(node->next->key, key) == 0) {
        return node->next;
    }

    return OSET_EOF;
}

OrderedSetNode oset_get_at_node(OrderedSet oset, int pos) { return OSET_EOF; }

void* oset_node_key(OrderedSet oset, OrderedSetNode node) { return NULL; }

void* oset_node_value(OrderedSet oset, OrderedSetNode node) { return NULL; }

OrderedSetNode oset_first(OrderedSet oset) { return OSET_EOF; }

OrderedSetNode oset_last(OrderedSet oset) { return OSET_EOF; }

OrderedSetNode oset_next(OrderedSet oset, OrderedSetNode node) { return OSET_EOF; }

OrderedSetNode oset_previous(OrderedSet oset, OrderedSetNode node) { return OSET_EOF; }
