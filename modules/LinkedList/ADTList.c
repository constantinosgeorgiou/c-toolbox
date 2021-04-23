///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT List μέσω συνδεδεμένης λίστας.
//
///////////////////////////////////////////////////////////

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
    // node == LIST_BOF(NULL), insert node next to sentinel node:
    if (node == LIST_BOF) {
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
    // node == LIST_BOF(NULL), remove node next to sentinel node:
    if (node == LIST_BOF) {
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
}

void list_remove(List list, ListNode node) {
    ListNode previous = NULL;
    ListNode current = NULL;

    // Traverse list to find previous list node of given node:
    for (current = list->sentinel->next; current != node; current = current->next) {
        previous = current;
    }

    if (previous == NULL) {
        // Remove first node:
        list->sentinel->next = node->next;
    } else {
        // Remove given node:
        previous->next = node->next;
    }

    // Remove node value:
    if (list->destroy != NULL) {
        list->destroy(node->value);
    }

    // Update last:
    if (list->last == node) {
        list->last = previous;
    }

    // Remove node:
    free(node);

    // Update list size:
    list->size -= 1;
}

/// @brief Appends to_append list, to given list. After operation to_append list can not be used.
///        list == to_append causes undefined behaviour.
///
/// @param list Defines a list.
/// @param to_append Defines a list to be appended.
///
/// @return Pointer to the beginning of the appened list, or NULL if an error occured.
///
ListNode list_append(List list, List to_append) {
    if (list == to_append) {
        return list;
    }

    list->last->next = to_append->sentinel->next;

    // Update last pointer and size:
    list->last == to_append->last;
    list->size += to_append->size;

    // Isolate sentinel node and destroy to_append list:
    to_append->sentinel->next = NULL;
    list_destroy(to_append);

    return list->sentinel->next;
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
    if (node == NULL) {
        return NULL;
    } else {
        return node->value;
    }
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
    if (node == NULL) {
        return NULL;
    } else {
        return node->next;
    }
}

void* list_get_at(List list, int position) {
    if (position < 0 || position > list->size) {
        return NULL;  // Out of bounds
    }

    ListNode node = list->sentinel->next;

    // Traverse list:
    for (int index = 0; index < list->size; index += 1) {
        if (index == position) {
            return node->value;
        }
        node = node->next;
    }
}