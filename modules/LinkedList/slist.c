////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file ADTList.c
/// @author Constantinos Georgiou
/// @brief Implementation of ADTList interface via Linked List.
/// @version 1
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ADTList.h"

#include <assert.h>
#include <stdlib.h>

struct list {
    ListNode sentinel;
    ListNode last;
    int size;
    DestroyFunc destroy;
};

struct list_node {
    ListNode next;
    void* value;
};

List list_create(DestroyFunc destroy) {
    List list = malloc(sizeof(*list));
    list->size = 0;
    list->destroy = destroy;

    list->sentinel = malloc(sizeof(*list->sentinel));
    if (list->sentinel == NULL) {
        free(list);
        return NULL;
    }
    list->sentinel->next = NULL;

    list->last = list->sentinel;

    return list;
}

void list_destroy(List list) {
    ListNode node = list->sentinel;
    while (node != NULL) {
        ListNode next = node->next;

        if (node != list->sentinel && list->destroy != NULL) {
            list->destroy(node->value);
        }

        free(node);
        node = next;
    }

    free(list);
}

DestroyFunc list_set_destroy_value(List list, DestroyFunc destroy) {
    DestroyFunc old = list->destroy;
    list->destroy = destroy;
    return old;
}

int list_size(List list) {
    return list->size;
}

ListNode list_insert_next(List list, ListNode node, void* value) {
    if (node == LIST_BOF) {  // Insert node next to sentinel node.
        node = list->sentinel;
    }

    ListNode new = malloc(sizeof(*new));
    if (new == NULL) {
        return NULL;
    }
    new->value = value;

    new->next = node->next;
    node->next = new;

    if (list->last == node) {
        list->last = new;
    }

    list->size += 1;

    return new;
}

ListNode list_remove_next(List list, ListNode node) {
    if (node == LIST_BOF) {  // Remove node next to sentinel node.
        node = list->sentinel;
    }

    ListNode removed = node->next;
    if (removed == NULL) {
        return node;
    }

    if (list->destroy != NULL) {
        list->destroy(removed->value);
    }

    node->next = removed->next;

    free(removed);

    if (list->last == removed) {
        list->last = node;
    }

    list->size -= 1;

    return node;
}

List list_concatenate(List a, List b) {
    assert(a != b);

    a->last->next = b->sentinel->next;

    // Update last pointer and size:
    a->last = b->last;
    a->size += b->size;

    // Isolate sentinel node and destroy b list:
    b->sentinel->next = NULL;
    list_destroy(b);

    return a;
}

ListNode list_find_node(List list, void* value, CompareFunc compare) {
    for (ListNode node = list->sentinel->next; node != NULL; node = node->next) {
        if (compare(value, node->value) == 0) {
            return node;  // FOUND
        }
    }

    return LIST_EOF;  // NOT FOUND
}

void* list_find(List list, void* value, CompareFunc compare) {
    ListNode node = list_find_node(list, value, compare);
    return node == NULL ? NULL : node->value;
}

void* list_node_value(List list, ListNode node) {
    assert(node != NULL);
    return node->value;
}

////////////    TRAVERSAL    ///////////////////////////////////////////////////////////////////////

ListNode list_first(List list) {
    return list->sentinel->next;
}

ListNode list_last(List list) {
    if (list->last == list->sentinel) {
        return LIST_EOF;
    } else {
        return list->last;
    }
}

ListNode list_next(List list, ListNode node) {
    assert(node != NULL);
    return node->next;
}

void* list_get_at(List list, int position) {
    if (position < 0 || position > list->size) {
        return NULL;  // Out of bounds
    }

    ListNode node = list->sentinel->next;

    // Traverse list:
    for (int index = 0; index < list->size; index++) {
        if (index == position) {
            return node->value;
        }
        node = node->next;
    }

    return NULL;
}