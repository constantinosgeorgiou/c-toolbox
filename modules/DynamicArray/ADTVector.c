#include "ADTVector.h"

#include <assert.h>
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

int vector_size(Vector vec) { return vec->size; }

void vector_insert_last(Vector vec, void* value) {
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
        if (vec->capacity > vec->size * 4 && vec->capacity > 2 * VECTOR_MIN_CAPACITY) {
            vec->capacity /= 2;
            vec->array = realloc(vec->array, vec->capacity * sizeof(*vec->array));
        }
    }
}

void* vector_find(Vector vec, void* value, CompareFunc compare) { return NULL; }

void* vector_get_at(Vector vec, int pos) {
    assert(pos >= 0 && pos < vec->size);
    return vec->array[pos].value;
}

void vector_set_at(Vector vec, int pos, void* value) {
    assert(pos >= 0 && pos < vec->size);

    // Call destroy_value on the value to be overwritten.
    if (value != vec->array[pos].value && vec->destroy_value != NULL) {
        vec->destroy_value(vec->array[pos].value);
    }

    vec->array[pos].value = value;
}

VectorNode vector_find_node(Vector vec, void* value, CompareFunc compare) { return NULL; }

void* vector_node_value(Vector vec, VectorNode node) { return NULL; }

VectorNode vector_first(Vector vec) { return NULL; }

VectorNode vector_last(Vector vec) { return NULL; }

VectorNode vector_next(Vector vec, VectorNode node) { return NULL; }

VectorNode vector_previous(Vector vec, VectorNode node) { return NULL; }
