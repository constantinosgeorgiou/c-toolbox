////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file ADTBidirectionalList.c
/// @author Constantinos Georgiou
/// @brief Implementation of ADTBidirectionalList interface via Doubly Linked List.
/// @version 1
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ADTBidirectionalList.h"

#include <assert.h>
#include <stdlib.h>

struct blist {
    BListNode sentinel;
    BListNode last;
    int size;
    DestroyFunc destroy;
};

struct blist_node {
    BListNode next;
    BListNode previous;
    void* value;
};

/// @brief Creates a bidirectional list node with given value.
///
/// @return Newly created bidirectional list node, or NULL if an error occured.
///
static BListNode create_node(void* value) {
    BListNode node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    node->value = value;
    node->next = NULL;
    node->previous = NULL;

    return node;
}

BList blist_create(DestroyFunc destroy) {
    BList blist = malloc(sizeof(*blist));

    blist->size = 0;
    blist->destroy = destroy;

    blist->sentinel = malloc(sizeof(*blist->sentinel));
    if (blist->sentinel == NULL) {
        return NULL;
    }

    blist->sentinel->next = NULL;
    blist->sentinel->previous = NULL;

    blist->last = blist->sentinel;

    return blist;
}

void blist_destroy(BList blist) {
    BListNode node = blist->sentinel;
    while (node != NULL) {
        BListNode next = node->next;

        if (node != blist->sentinel && blist->destroy != NULL) {
            blist->destroy(node->value);
        }

        free(node);
        node = next;
    }

    free(blist);
}

DestroyFunc blist_set_destroy_value(BList blist, DestroyFunc destroy) {
    DestroyFunc old = blist->destroy;
    blist->destroy = destroy;
    return old;
}

int blist_size(BList blist) { return blist->size; }

BListNode blist_insert(BList blist, BListNode node, void* value) {
    if (node == BLIST_BOF) {
        node = blist->sentinel;
    }

    BListNode new = create_node(value);
    if (new == NULL) {
        return NULL;
    }

    // Insert new node after given node:
    new->next = node->next;
    new->previous = node;
    node->next = new;
    if (new->next != BLIST_EOF) {
        new->next->previous = new;
    }

    if (blist->last == node) {
        blist->last = new;
    }

    blist->size++;

    return new;
}

void blist_remove(BList blist, BListNode node) {
    if (node == BLIST_EOF) {
        node = blist->last;
    }

    BListNode remove = node;

    if (node->next != BLIST_EOF) {
        node->next->previous = node->previous;
    }
    if (node->previous != BLIST_BOF) {
        node->previous->next = node->next;
    }

    if (blist->destroy != NULL) {
        blist->destroy(remove->value);
    }

    if (remove == blist->last) {
        blist->last = remove->previous;
    }

    blist->size--;

    free(remove);
}

BListNode blist_concatenate(BList a, BList b) {
    assert(a != b);

    // Connect last node of a to first node of b:
    a->last->next = b->sentinel->next;

    // Update last pointer and size:
    a->last = b->last;
    a->size += b->size;

    // Isolate sentinel node and destroy bidirectional list b:
    b->sentinel->next = NULL;
    list_destroy(b);

    return a->sentinel->next;
}

BListNode blist_find_node(BList blist, void* value, CompareFunc compare) {
    for (BListNode node = blist->sentinel->next; node != NULL; node = node->next) {
        if (compare(value, node->value) == 0) {
            return node;  // FOUND
        }
    }

    return BLIST_EOF;  // NOT FOUND
}

void* blist_find(BList blist, void* value, CompareFunc compare) {
    BListNode node = blist_find_node(blist, value, compare);
    return node == NULL ? NULL : node->value;
}

void* blist_node_value(BList blist, BListNode node) {
    assert(node != NULL);
    return node->value;
}

////////////    TRAVERSAL    ///////////////////////////////////////////////////////////////////////

BListNode blist_first(BList blist) { return blist->sentinel->next; }

BListNode blist_last(BList blist) {
    if (blist->last == blist->sentinel) {
        return BLIST_EOF;  // Bidirectional list is empty.
    } else {
        return blist->last;
    }
}

BListNode blist_next(BList blist, BListNode node) {
    assert(node != NULL);
    return node->next;
}

BListNode blist_previous(BList blist, BListNode node) {
    assert(node != NULL);
    return node->previous;
}

void* blist_get_at(BList blist, int position) {
    if (position < 0 || position > blist->size) {
        return NULL;  // Out of bounds
    }

    BListNode node = blist->sentinel->next;

    // Traverse list:
    for (int index = 0; index < blist->size; index++) {
        if (index == position) {
            return node->value;
        }
        node = node->next;
    }

    return NULL;
}
