#include "oset.h"

#include <assert.h>  // assert
#include <stdbool.h> // true, false
#include <stdio.h>   // printf
#include <stdlib.h>  // malloc, free, sizeof
#include <string.h>
#include <time.h> // time

#include "pcg_basic.h" // pcg32_srandom, pcg32_boundedrand
#include "vector.h"

/// @brief Levels of forward pointers a node can have.
///
/// Start at 16 levels, because the Oredered Set can contain 2^16 elements which
/// is plenty. In the event the Oredered Set has 2^16 elements, double its max
/// level.
///
#define OSET_LEVELS 16

/// @brief Precalculated sizes to used when oset->size needs to be doubled.
///
///
static size_t sizes[] = {
    65536,      // 2^16
    4294967296, // 2^32
    UINTMAX_MAX // Can't fit 2^64 in size_t.
};

struct ordered_set {
  CompareFunc compare;
  DestroyFunc destroy_key;
  DestroyFunc destroy_value;

  int max_level; // Maximum level possible for a node of the Ordered Set.
                 // Oredered Set can have 2^max_level, if that number is
                 // reached, then double the max_level.

  size_t capacity; // Maximum number of elements in the Ordered Set. Doubles if
                   // capacity == size.
  size_t size;     // Number of elements in the Ordered Set.

  OrderedSetNode first;
  OrderedSetNode last;

  OrderedSetNode header;
};

struct ordered_set_node {
  OrderedSetNode *forward;
  OrderedSetNode previous;

  int levels;     // Number of forward links.
  bool is_header; // true, if node is header, otherwise false.

  void *key;
  void *value;
};

/// @brief Returns the maximum between two integers.
///
static int max(int a, int b) { return a > b ? a : b; }

/// @brief Increases the capacity of specified Ordered Set.
///
static void capacity_increase(OrderedSet oset) {
  size_t old_capacity = oset->capacity;

  int sizes_len = sizeof(sizes) / sizeof(size_t);

  for (int i = 0; i < sizes_len; i++) {
    if (sizes[i] > old_capacity) {
      oset->capacity = sizes[i];
      break;
    }
  }
}

/// @brief Determines if the Random Number Generator is seeeded.
///
/// true, if seeded, otherwise false.
///
static bool seeded = false;

/// @brief Seeds the Random Number Generator.
///
/// @param initseq Selects the output sequence for the RNG, you can pass any
/// 64-bit value, although
///                only the low 63 bits are significant.
///
static void random_seed(int initseq) {
  pcg32_srandom(time(NULL) ^ (intptr_t)&printf, (intptr_t)&initseq);
  seeded = true;
}

/// @brief Choose a random level between 1 and max_level.
///
/// @param max_level Maximum level in an Ordered Set.
///
static int level_random(int max_level) {
  int level = 1;

  // "Flip coins". Increase level until tails(0).
  while (pcg32_boundedrand(2) && level < max_level)
    level++;

  return level;
}

/// @brief Creates and returns an Ordered Set node.
///
/// @param levels Number of forward links.
/// @param is_header true if node is header node, else false.
///
static OrderedSetNode node_create(void *key, void *value, int levels,
                                  bool is_header) {
  OrderedSetNode node = malloc(sizeof(*node));
  if (node == NULL) {
    return NULL;
  }

  // Allocate forward array and initialize it to NULL.
  node->forward = calloc(levels * sizeof(*node->forward), levels);
  if (node->forward == NULL) {
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
/// Any operation on the Ordered Set node after its destruction, results in
/// undefined behaviour.
///
/// @param update Contains nodes whose forward links need to be updated. If
/// update == NULL,
///               no link updates take place.
///
static void node_destroy(OrderedSetNode node, DestroyFunc destroy_key,
                         DestroyFunc destroy_value, Vector update) {
  // Link previous nodes to forward nodes.
  if (update != NULL) {
    for (int i = node->levels - 1; i >= 0; i--) {
      OrderedSetNode prev_node = vector_get_at(update, i);
      prev_node->forward[i] = node->forward[i];
    }
  }

  if (node->is_header == false) {
    if (destroy_key != NULL)
      destroy_key(node->key);
    if (destroy_value != NULL)
      destroy_value(node->value);
  }

  free(node->forward);
  free(node);
}

/// @brief Finds and returns previous node of node with specified key.
///
/// @param update Tracks the nodes traversed to previous node. If update ==
/// NULL, traversed nodes
///               are not tracked.
///
/// @return Previous node of node with specified key.
///
static OrderedSetNode node_find_previous(OrderedSet oset, void *key,
                                         Vector update) {
  assert(key != NULL);

  OrderedSetNode node = oset->header;

  // Traverse levels from top to bottom.
  for (int i = node->levels - 1; i >= 0; i--) {
    // Traverse forward links in level.
    while (node->forward[i] != OSET_EOF &&
           oset->compare(node->forward[i]->key, key) < 0) {
      node = node->forward[i];
    }

    // Track traversed nodes.
    if (update != NULL) {
      vector_set_at(update, i, node);
    }
  }

  return node;
}

OrderedSet oset_create(CompareFunc compare, DestroyFunc destroy_key,
                       DestroyFunc destroy_value) {
  OrderedSet oset = malloc(sizeof(*oset));
  if (oset == NULL)
    return NULL;

  oset->compare = compare;
  oset->destroy_key = destroy_key;
  oset->destroy_value = destroy_value;

  oset->max_level = OSET_LEVELS;
  oset->capacity = sizes[0];
  oset->size = 0;

  oset->first = OSET_EOF;
  oset->last = OSET_EOF;

  // Header nodes don't need to have neither keys nor values.
  oset->header = node_create(OSET_BOF, OSET_BOF, oset->max_level, true);
  if (oset->header == NULL)
    return NULL;
  oset->header->levels =
      1; // Allocate all levels up to max_level, but start at level 1.

  // Seed Pseudo Random Number Generator, if not yet seeded.
  if (seeded == false)
    random_seed(oset->max_level);

  return oset;
}

void oset_destroy(OrderedSet oset) {
  OrderedSetNode node = oset->header;

  while (node != OSET_EOF) {
    OrderedSetNode next = node->forward[0];

    node_destroy(node, oset->destroy_key, oset->destroy_value, NULL);

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

size_t oset_size(OrderedSet oset) { return oset->size; }

void oset_insert(OrderedSet oset, void *key, void *value) {
  assert(key != NULL);

  // Increase capacity if needed.
  if (oset->size == oset->capacity) {
    capacity_increase(oset);
    oset->max_level *= 2;
  }

  OrderedSetNode new_node =
      node_create(key, value, level_random(oset->max_level), false);

  // Increase header levels if needed.
  if (oset->header->levels < new_node->levels)
    oset->header->levels = new_node->levels;

  Vector update = vector_create(oset->max_level, NULL);

  OrderedSetNode target = node_find_previous(oset, key, update);

  // Insert new_node after node.
  for (int i = new_node->levels - 1; i >= 0; i--) {
    OrderedSetNode node = vector_get_at(update, i);
    new_node->forward[i] = node->forward[i];
    node->forward[i] = new_node;
  }

  // Update previous pointers.
  new_node->previous = target;
  if (new_node->forward[0] != OSET_EOF)
    new_node->forward[0]->previous = new_node;

  // Update first pointer.
  if (oset->first == OSET_EOF ||
      oset->compare(new_node->key, oset->first->key) < 0) {
    oset->first = new_node;
  }

  // Update last pointer.
  if (oset->last == OSET_EOF ||
      oset->compare(new_node->key, oset->last->key) > 0) {
    oset->last = new_node;
  }

  // Update size.
  oset->size++;

  // Free memory allocated for the vector.
  vector_destroy(update);
}

bool oset_remove(OrderedSet oset, void *key) {
  assert(key != NULL);

  Vector update = vector_create(oset->max_level, NULL);

  OrderedSetNode target = node_find_previous(oset, key, update);
  if (target->forward[0] == OSET_EOF ||
      oset->compare(target->forward[0]->key, key) != 0) {
    // Free memory allocated for the vector.
    vector_destroy(update);

    // Specified key was not found.
    return false;
  }

  // Update first pointer.
  if (target->forward[0] == oset->first) {
    oset->first = oset->first->forward[0];
  }

  // Update last pointer.
  if (target->forward[0] == oset->last) {
    oset->last = target->is_header ? OSET_BOF : target;
  }

  // Destroy node including its top levels.
  node_destroy(target->forward[0], oset->destroy_key, oset->destroy_value,
               update);

  // Update size.
  oset->size--;

  // Free memory allocated for the vector.
  vector_destroy(update);

  return true;
}

void *oset_find(OrderedSet oset, void *key) {
  assert(key != NULL);
  OrderedSetNode node = oset_find_node(oset, key);
  return node != NULL ? node->value : NULL;
}

OrderedSet oset_split(OrderedSet oset, void *split_key) {
  if (oset->size == 0)
    return NULL;

  OrderedSet split =
      oset_create(oset->compare, oset->destroy_key, oset->destroy_value);
  if (split == NULL)
    return NULL;

  // Initialize split Ordered Set from oset metadata.
  if (split->max_level < oset->max_level) {
    split->max_level = oset->max_level; // Update max_level of split.
    // Reallocate and initialize to NULL the forward array of split header node.
    free(split->header->forward);
    split->header->forward = calloc(
        split->max_level * sizeof(*split->header->forward), split->max_level);
    if (split->header->forward == NULL)
      return NULL;
  }
  split->header->levels = oset->header->levels;

  OrderedSetNode node = oset->header;

  // Traverse levels from top to bottom.
  for (int i = node->levels - 1; i >= 0; i--) {
    // Traverse forward links in level.
    while (node->forward[i] != OSET_EOF &&
           oset->compare(node->forward[i]->key, split_key) <= 0) {
      node = node->forward[i];
    }
    split->header->forward[i] = node->forward[i];
    node->forward[i] = OSET_EOF;
  }

  // Update SPLIT previous pointers.
  if (split->header->forward[0] != OSET_EOF)
    split->header->forward[0]->previous = split->header;

  // Update first and last pointers of SPLIT.
  split->first = split->header->forward[0];
  if (split->first != OSET_EOF)
    split->last = oset->last; // Atleast one element is in SPLIT.

  // Update first and last pointers of OSET.
  if (oset->first == split->first)
    oset->first = OSET_EOF;
  oset->last = node;

  // Decrease excess levels of original Ordered Set.
  node = oset->header;
  while (node->forward[node->levels - 1] == OSET_EOF && node->levels - 1 > 0) {
    node->levels--;
  }

  // Decrease excess levels of split Ordered Set.
  node = split->header;
  while (node->forward[node->levels - 1] == OSET_EOF && node->levels - 1 > 0) {
    node->levels--;
  }

  // Update sizes.
  size_t size = 0;
  for (node = split->header->forward[0]; node != OSET_EOF;
       node = node->forward[0])
    size++;
  split->size = size;
  oset->size -= split->size;

  return split;
}

OrderedSet oset_merge(OrderedSet a, OrderedSet b) {
  assert(a != b);

  // Hold the last nodes on each level of MERGED.
  Vector last_nodes = vector_create(max(a->max_level, b->max_level), NULL);

  // Create new Ordered Set.
  OrderedSet merged = oset_create(a->compare, a->destroy_key, a->destroy_value);
  if (a == NULL)
    return NULL;

  // Initialize new Ordered Set.
  if (merged->max_level < max(a->max_level, b->max_level)) {
    // Update max_level to reallocate the header node.
    merged->max_level = max(a->max_level, b->max_level);

    // Reallocate and initialize to NULL the forward array of MERGED header
    // node.
    free(merged->header->forward);
    merged->header->forward =
        calloc(merged->max_level * sizeof(*merged->header->forward),
               merged->max_level);
    if (merged->header->forward == NULL)
      return NULL;
  }
  merged->capacity = a->capacity > b->capacity ? a->capacity : b->capacity;
  merged->header->levels = max(a->header->levels, b->header->levels);

  // Initialize last_nodes vector.
  for (int i = 0; i < merged->header->levels; i++)
    vector_set_at(last_nodes, i, merged->header);

  // Transfer nodes from A and B to MERGED.
  while (a->header->forward[0] != OSET_EOF &&
         b->header->forward[0] != OSET_EOF) {
    void *key1 = a->header->forward[0]->key;
    void *key2 = b->header->forward[0]->key;

    if (merged->compare(key1, key2) > 0) {
      // Exchange, key1 and key2, and Ordered Sets A and B.

      void *temp_key = key1;
      key1 = key2;
      key2 = temp_key;

      OrderedSet temp_oset = a;
      a = b;
      b = temp_oset;
    }

    // Add elements to MERGED from A with keys <= key2.
    int lvl = 0;
    do {
      OrderedSetNode node = vector_get_at(last_nodes, lvl);
      node->forward[lvl] = a->header->forward[lvl];
      if (lvl == 0 && node->forward[0] != OSET_EOF)
        node->forward[0]->previous = node;
      lvl++;
    } while (lvl < merged->header->levels &&
             a->header->forward[lvl] != OSET_EOF &&
             merged->compare(a->header->forward[lvl]->key, key2) < 0);
    lvl--;

    // For each level attached to MERGED, find endpoint at that level and remove
    // it from A.
    OrderedSetNode node = a->header->forward[lvl];
    for (int i = lvl; i >= 0; i--) {
      while (node->forward[i] != OSET_EOF &&
             merged->compare(node->forward[i]->key, key2) <= 0) {
        node = node->forward[i];
      }
      vector_set_at(last_nodes, i, node);
      a->header->forward[i] = node->forward[i];
      if (i == 0 && node->forward[i] != OSET_EOF) {
        node->forward[i]->previous = a->header;
      }
    }
  }

  // Connect left over nodes to MERGED.
  OrderedSet left_over = b->header->forward[0] == OSET_EOF ? a : b;
  for (int i = 0; i < left_over->header->levels; i++) {
    OrderedSetNode node = vector_get_at(last_nodes, i);
    node->forward[i] = left_over->header->forward[i];
    if (i == 0 && node->forward[i] != OSET_EOF) {
      node->forward[i]->previous = node;
    }

    // Traverse remaining nodes to find last node.
    while (node->forward[i] != OSET_EOF) {
      node = node->forward[i];
    }
    vector_set_at(last_nodes, i, node);

    // Disconnect B header from left over nodes.
    b->header->forward[i] = OSET_EOF;
  }

  // Update first pointer.
  merged->first = merged->header->forward[0];

  // Update last pointer.
  merged->last = vector_get_at(last_nodes, 0);

  // Update size.
  merged->size = a->size + b->size;

  oset_destroy(a);
  oset_destroy(b);
  vector_destroy(last_nodes);

  return merged;
}

void oset_concat(OrderedSet a, OrderedSet b) {
  assert(a != b);

  // Increase levels of  a  Ordered Set if needed.
  if (a->header->levels < b->header->levels) {
    for (int i = a->header->levels; i < b->header->levels; i++) {
      a->header->forward[i] = OSET_EOF;
    }

    a->header->levels = b->header->levels;
  }

  OrderedSetNode node = a->header;
  // Traverse levels from top to bottom.
  for (int i = node->levels - 1; i >= 0; i--) {
    // Traverse forward links in level.
    while (node->forward[i] != OSET_EOF)
      node = node->forward[i];

    if (i <= b->header->levels) {
      node->forward[i] = b->header->forward[i];
    }
  }

  // Connect previous pointer of first node of B to last node of A to be able to
  // traverse A in descending order.
  if (b->first != NULL)
    b->first->previous = a->last;

  // Update last pointer.
  a->last = b->last;

  // Update size.
  a->size += b->size;

  // Destroy  b  Ordered Set.
  for (int i = b->header->levels; i >= 0; i--)
    b->header->forward[i] = OSET_EOF;
  oset_set_destroy_key(b, NULL);
  oset_set_destroy_value(b, NULL);
  oset_destroy(b);
}

OrderedSetNode oset_find_node(OrderedSet oset, void *key) {
  assert(key != NULL);

  OrderedSetNode node = node_find_previous(oset, key, NULL);

  if (node->forward[0] != OSET_EOF &&
      oset->compare(node->forward[0]->key, key) == 0) {
    return node->forward[0];
  }

  return OSET_EOF;
}

void *oset_node_key(OrderedSet oset, OrderedSetNode node) {
  assert(node != NULL);
  return node->key;
}

void *oset_node_value(OrderedSet oset, OrderedSetNode node) {
  assert(node != NULL);
  return node->value;
}

OrderedSetNode oset_first(OrderedSet oset) { return oset->first; }

OrderedSetNode oset_last(OrderedSet oset) { return oset->last; }

OrderedSetNode oset_next(OrderedSet oset, OrderedSetNode node) {
  assert(node != NULL);
  return node->forward[0];
}

OrderedSetNode oset_previous(OrderedSet oset, OrderedSetNode node) {
  assert(node != NULL);
  return node->previous->is_header ? OSET_BOF : node->previous;
}
