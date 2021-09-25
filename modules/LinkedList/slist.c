#include "slist.h"

#include <assert.h> // assert
#include <stdlib.h> // size_t, sizeof, malloc, free

struct slist {
  SListNode sentinel;
  SListNode last;
  int size;
  DestroyFunc destroy_value;
};

struct slist_node {
  SListNode next;
  void *value;
};

SList slist_create(DestroyFunc destroy_value) {
  SList slist = malloc(sizeof(*slist));
  if (slist == NULL)
    return NULL;

  slist->sentinel = malloc(sizeof(*slist->sentinel));
  if (slist->sentinel == NULL) {
    free(slist);
    return NULL;
  }
  slist->sentinel->next = NULL;

  slist->size = 0;
  slist->destroy_value = destroy_value;

  slist->last = slist->sentinel;

  return slist;
}

void slist_destroy(SList slist) {
  SListNode node = slist->sentinel;
  while (node != NULL) {
    SListNode next = node->next;

    if (node != slist->sentinel && slist->destroy_value != NULL) {
      slist->destroy_value(node->value);
    }

    free(node);
    node = next;
  }

  free(slist);
}

DestroyFunc slist_set_destroy_value(SList slist, DestroyFunc destroy_value) {
  DestroyFunc old = slist->destroy_value;
  slist->destroy_value = destroy_value;
  return old;
}

size_t slist_size(SList slist) { return slist->size; }

void slist_insert_next(SList slist, SListNode node, void *value) {
  if (node == SLIST_BOF)
    node = slist->sentinel; // Insert node next to sentinel node.

  SListNode new_node = malloc(sizeof(*new_node));
  if (new_node == NULL)
    return;

  new_node->value = value;

  new_node->next = node->next;
  node->next = new_node;

  if (slist->last == node)
    slist->last = new_node;

  slist->size += 1;
}

void slist_remove_next(SList slist, SListNode node) {
  if (node == SLIST_BOF)
    node = slist->sentinel; // Remove node next to sentinel node.

  SListNode removed = node->next;
  if (removed == NULL)
    return;

  if (slist->destroy_value != NULL)
    slist->destroy_value(removed->value);

  node->next = removed->next;

  free(removed);

  if (slist->last == removed)
    slist->last = node;

  slist->size -= 1;
}

SList slist_concat(SList dest, SList src) {
  assert(dest != src && "slist_concat: slists given are the same.");

  dest->last->next = src->sentinel->next;

  // Update last pointer and size:
  dest->last = src->last;
  dest->size += src->size;

  // Isolate sentinel node and destroy src slist:
  src->sentinel->next = NULL;
  slist_destroy(src);

  return dest;
}

SListNode slist_find_node(SList slist, void *value, CompareFunc compare) {
  for (SListNode node = slist->sentinel->next; node != NULL;
       node = node->next) {
    if (compare(value, node->value) == 0)
      return node; // FOUND
  }

  return SLIST_EOF; // NOT FOUND
}

void *slist_find(SList slist, void *value, CompareFunc compare) {
  SListNode node = slist_find_node(slist, value, compare);
  return node == NULL ? NULL : node->value;
}

void *slist_node_value(SList slist, SListNode node) {
  assert(node != NULL);
  return node->value;
}

SListNode slist_first(SList slist) { return slist->sentinel->next; }

SListNode slist_last(SList slist) {
  if (slist->last == slist->sentinel)
    return SLIST_EOF;
  else
    return slist->last;
}

SListNode slist_next(SList slist, SListNode node) {
  assert(node != NULL);
  return node->next;
}

void *slist_get_at(SList slist, int pos) {
  if (pos < 0)
    pos = 0;
  if (pos > slist->size)
    pos = slist->size;

  SListNode node = slist->sentinel->next;

  // Traverse slist:
  for (int index = 0; index < slist->size; index++) {
    if (index == pos)
      return node == NULL ? NULL : node->value;

    node = node->next;
  }

  return NULL;
}