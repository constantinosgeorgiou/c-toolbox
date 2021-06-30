#pragma once

#ifndef COMMON_FUNCTIONS
#define COMMON_FUNCTIONS

/// @brief Pointer to function that compares elements a and b.
///
/// @return < 0, if a < b, or, > 0, if b < a, or, 0, if a and b are equivalent
///
typedef int (*CompareFunc)(const void* a, const void* b);

/// @brief Pointer to function that destroys a value.
///
typedef void (*DestroyFunc)(void* value);

#endif

#define VECTOR_ERROR (Vector)0

typedef struct vector* Vector;

/// @brief Creates and returns a vector with specified size and elements initialized to NULL.
///
/// @param destroy_value If destroy_value != NULL, call destroy_value(value) each time a value is
///                      removed/overwritten.
///
/// @return Newly created Vector, or VECTOR_ERROR, if failed to allocate enough memory.
///
Vector vector_create(int size, DestroyFunc destroy_value);

/// @brief Frees all the memory allocated by given Vector.
///
/// Any operation on the Vector after its destruction, results in undefined behaviour.
///
void vector_destroy(Vector vec);

/// @brief Changes function called each time a value is removed/overwriten, to destroy_value.
///
/// @return Previous destroy_value function.
///
DestroyFunc vector_set_destroy_value(Vector vec, DestroyFunc destroy_value);

/// @brief Returns the number of elements in the Vector.
///
/// @return The number of elements in the Vector.
///
int vector_size(Vector vec);

/// @brief Inserts value at the end of Vector. Increases the size of Vector by 1.
///
void vector_insert_last(Vector vec, void* value);

/// @brief Removes the last element of Vector. Decreases the size of Vector by 1.
///
/// If Vector is empty, nothing is done.
///
void vector_remove_last(Vector vec);

/// @brief Finds and returns the first element equivalent to value (based on compare).
///
/// @return First element equivalent to value, or NULL, if element not found.
///
void* vector_find(Vector vec, void* value, CompareFunc compare);

/// @brief Gets the value at specified position.
///
/// If pos < 0 gets value of first element. If pos > size gets value of last element.
///
/// @return Value at specified position.
///
void* vector_get_at(Vector vec, int pos);

/// @brief Sets the value at specified position.
///
/// If pos < 0 sets value of first element. If pos > size sets value of last element.
///
void vector_set_at(Vector vec, int pos, void* value);

#define VECTOR_BOF (VectorNode)0  // Defines the virtual beginning of Vector.
#define VECTOR_EOF (VectorNode)0  // Defines the virtual end of Vector.

typedef struct vector_node* VectorNode;

/// @brief Finds and returns the first node with value equivalent to specified value
///        (based on compare).
///
/// @return First node with value equivalent to specified value, or VECTOR_EOF, if node with value
///         not found.
///
VectorNode vector_find_node(Vector vec, void* value, CompareFunc compare);

/// @brief Returns value of specified node.
///
void* vector_node_value(Vector vec, VectorNode node);

/// @brief Returns the first node of Vector, or VECTOR_EOF, if Vector is empty.
///
VectorNode vector_first(Vector vec);

/// @brief Returns the last node of Vector, or VECTOR_EOF, if Vector is empty.
///
VectorNode vector_last(Vector vec);

/// @brief Returns the next node of specified node, or VECTOR_EOF, if specified node is last.
///
VectorNode vector_next(Vector vec, VectorNode node);

/// @brief Returns the previous node of specified node, or VECTOR_BOF, if specified node is first.
///
VectorNode vector_previous(Vector vec, VectorNode node);
