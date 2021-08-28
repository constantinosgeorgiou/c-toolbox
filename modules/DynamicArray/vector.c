#include "vector.h"

#include <assert.h> // assert
#include <stdlib.h> // malloc, free, sizeof, size_t

#define VECTOR_MIN_CAPACITY 10

struct vector {
  DestroyFunc destroy_value;
  int size;     // Number of elements in Vector.
  int capacity; // Allocated space. Condition capacity >= size always needs to
                // be satisfied.
  VectorNode array; // Arrray of struct vector_node. Contains the data.
};

struct vector_node {
  void *value;
};

Vector vector_create(int size, DestroyFunc destroy_value) {
  Vector vec = malloc(sizeof(*vec));
  if (vec == NULL) {
    return NULL;
  }

  vec->size = size;
  vec->destroy_value = destroy_value;

  // Allocate VECTOR_MIN_CAPACITY memory to avoid multiple unecesary resizes.
  vec->capacity = size < VECTOR_MIN_CAPACITY ? VECTOR_MIN_CAPACITY : size;
  vec->array =
      calloc(vec->capacity, sizeof(*vec->array)); // Initialize to 0 (NULL).

  return vec;
}

void vector_destroy(Vector vec) {
  if (vec->destroy_value != NULL) {
    for (int i = 0; i < vec->size; i++) {
      vec->destroy_value(vec->array[i].value);
    }
  }

  free(vec->array);
  free(vec);
}

DestroyFunc vector_set_destroy_value(Vector vec, DestroyFunc destroy_value) {
  DestroyFunc old = vec->destroy_value;
  vec->destroy_value = destroy_value;
  return old;
}

size_t vector_size(Vector vec) { return vec->size; }

void vector_insert_last(Vector vec, void *value) {
  if (vec->capacity == vec->size) {
    vec->capacity *= 2;
    vec->array = realloc(vec->array, vec->capacity * sizeof(*vec->array));
  }

  // Increase array and insert element.
  vec->array[vec->size].value = value;
  vec->size++;
}

void vector_remove_last(Vector vec) {
  if (vec->size != 0) {
    if (vec->destroy_value != NULL) {
      vec->destroy_value(vec->array[vec->size - 1].value);
    }

    vec->size--;

    // Reduce size if 75% of Vector is empty to free up memory.
    if (vec->capacity > vec->size * 4 &&
        vec->capacity > 2 * VECTOR_MIN_CAPACITY) {
      vec->capacity /= 2;
      vec->array = realloc(vec->array, vec->capacity * sizeof(*vec->array));
    }
  }
}

void *vector_find(Vector vec, void *value, CompareFunc compare) {
  VectorNode node = vector_find_node(vec, value, compare);
  return node == VECTOR_EOF ? NULL : node->value;
}

void *vector_get_at(Vector vec, int pos) {
  if (pos < 0)
    pos = 0;
  if (pos > vec->size)
    pos = vec->size - 1;
  return vec->array[pos].value;
}

void vector_set_at(Vector vec, int pos, void *value) {
  if (pos < 0)
    pos = 0;
  if (pos > vec->size)
    pos = vec->size - 1;

  // Call destroy_value on the value to be overwritten.
  if (value != vec->array[pos].value && vec->destroy_value != NULL) {
    vec->destroy_value(vec->array[pos].value);
  }

  vec->array[pos].value = value;
}

VectorNode vector_find_node(Vector vec, void *value, CompareFunc compare) {
  for (int i = 0; i < vec->size; i++) {
    if (compare(vec->array[i].value, value) == 0) {
      return &vec->array[i];
    }
  }

  return VECTOR_EOF;
}

void *vector_node_value(Vector vec, VectorNode node) {
  assert(node != NULL);
  return node->value;
}

VectorNode vector_first(Vector vec) {
  return vec->size == 0 ? VECTOR_BOF : &vec->array[0];
}

VectorNode vector_last(Vector vec) {
  return vec->size == 0 ? VECTOR_EOF : &vec->array[vec->size - 1];
}

VectorNode vector_next(Vector vec, VectorNode node) {
  return node == &vec->array[vec->size - 1] ? VECTOR_EOF : node + 1;
}

VectorNode vector_previous(Vector vec, VectorNode node) {
  return node == &vec->array[0] ? VECTOR_BOF : node - 1;
}
