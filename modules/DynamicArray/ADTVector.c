#include "ADTVector.h"

#include <stdlib.h>

#define VECTOR_MIN_CAPACITY 10

struct vector {
    DestroyFunc destroy_value;
    int size;          // Number of elements in Vector.
    int capacity;      // Allocated space. Condition capacity >= size always needs to be satisfied.
    VectorNode array;  // Arrray of struct vector_node. Contains the data.
};

struct vector_node {
    void* value;
};

Vector vector_create(int size, DestroyFunc destroy_value) {
    Vector vec = malloc(sizeof(*vec));
    if (vec == NULL) {
        return VECTOR_ERROR;
    }

    vec->size = size;
    vec->destroy_value = destroy_value;

    // Allocate VECTOR_MIN_CAPACITY memory to avoid multiple unecesary resizes.
    vec->capacity = size < VECTOR_MIN_CAPACITY ? VECTOR_MIN_CAPACITY : size;
    vec->array = calloc(vec->capacity, sizeof(*vec->array));  // Initialize to 0 (NULL).

    return vec;
}

void vector_destroy(Vector vec) {}
DestroyFunc vector_set_destroy_value(Vector vec, DestroyFunc destroy_value) { return NULL; }
int vector_size(Vector vec) { return 0; }
void vector_insert_last(Vector vec, void* value) {}
void vector_remove_last(Vector vec) {}
void* vector_find(Vector vec, void* value, CompareFunc compare) { return NULL; }
void* vector_get_at(Vector vec, int pos) { return NULL; }
void vector_set_at(Vector vec, int pos, void* value) {}
VectorNode vector_find_node(Vector vec, void* value, CompareFunc compare) { return NULL; }
void* vector_node_value(Vector vec, VectorNode node) { return NULL; }
VectorNode vector_first(Vector vec) { return NULL; }
VectorNode vector_last(Vector vec) { return NULL; }
VectorNode vector_next(Vector vec, VectorNode node) { return NULL; }
VectorNode vector_previous(Vector vec, VectorNode node) { return NULL; }
