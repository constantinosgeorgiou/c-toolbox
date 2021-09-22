#include "list.h"

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
  ListNode previous;
  void *value;
};

/// @brief Creates a bidirectional list node with given value.
///
/// @return Newly created bidirectional list node, or NULL if an error occured.
///
static ListNode create_node(void *value) {
  ListNode node = malloc(sizeof(*node));
  if (node == NULL) {
    return NULL;
  }

  node->value = value;
  node->next = NULL;
  node->previous = NULL;

  return node;
}

List list_create(DestroyFunc destroy) {
  List list = malloc(sizeof(*list));

  list->size = 0;
  list->destroy = destroy;

  list->sentinel = malloc(sizeof(*list->sentinel));
  if (list->sentinel == NULL) {
    return NULL;
  }

  list->sentinel->next = NULL;
  list->sentinel->previous = NULL;

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

size_t list_size(List list) { return list->size; }

void list_insert(List list, ListNode node, void *value) {
  if (node == LIST_BOF) {
    node = list->sentinel;
  }

  ListNode new = create_node(value);
  if (new == NULL)
    return;

  // Insert new node after given node:
  new->next = node->next;
  new->previous = node;
  node->next = new;
  if (new->next != LIST_EOF)
    new->next->previous = new;

  if (list->last == node)
    list->last = new;

  list->size++;
}

void list_remove(List list, ListNode node) {
  if (node == LIST_EOF) {
    node = list->last;
  }

  ListNode remove = node;

  if (node->next != LIST_EOF) {
    node->next->previous = node->previous;
  }
  if (node->previous != LIST_BOF) {
    node->previous->next = node->next;
  }

  if (list->destroy != NULL) {
    list->destroy(remove->value);
  }

  if (remove == list->last) {
    list->last = remove->previous;
  }

  list->size--;

  free(remove);
}

void list_concat(List a, List b) {
  assert(a != b);

  // Connect last node of a to first node of b:
  a->last->next = b->sentinel->next;
  a->last->next->previous = a->last;

  // Update last pointer and size:
  a->last = b->last;
  a->size += b->size;

  // Isolate sentinel node and destroy bidirectional list b:
  b->sentinel->next = NULL;
  list_destroy(b);
}

ListNode list_find_node(List list, void *value, CompareFunc compare) {
  for (ListNode node = list->sentinel->next; node != NULL; node = node->next) {
    if (compare(value, node->value) == 0) {
      return node; // FOUND
    }
  }

  return LIST_EOF; // NOT FOUND
}

void *list_find(List list, void *value, CompareFunc compare) {
  ListNode node = list_find_node(list, value, compare);
  return node == NULL ? NULL : node->value;
}

void *list_node_value(List list, ListNode node) {
  assert(node != NULL);
  return node->value;
}

ListNode list_first(List list) { return list->sentinel->next; }

ListNode list_last(List list) {
  if (list->last == list->sentinel) {
    return LIST_EOF; // Bidirectional list is empty.
  } else {
    return list->last;
  }
}

ListNode list_next(List list, ListNode node) {
  assert(node != NULL);
  return node->next;
}

ListNode list_previous(List list, ListNode node) {
  assert(node != NULL);
  return node->previous;
}

void *list_get_at(List list, int position) {
  if (position < 0 || position > list->size) {
    return NULL; // Out of bounds
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
