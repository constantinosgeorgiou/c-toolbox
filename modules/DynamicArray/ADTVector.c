#include "ADTVector.h"

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
