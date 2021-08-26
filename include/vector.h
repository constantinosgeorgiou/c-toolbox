/// @file vector.h
///
/// Vector Abstract Data Type.
///
/// Implementation independent.
///
/// The user does not need to know how a Vector is implemented, they use the API
/// functions provided `vector_<operation>` with the appropriate parameters.
///
/// Operations supported:
/// - vector_create(): Creates and returns a new vector with specified size and
///                    elements initialized to NULL.
/// - vector_destroy(): Frees all the memory allocated by a vector.
/// - vector_insert_last(): Inserts a value at the end of a vector.
/// - vector_remove_last(): Removes the last element of a vector.
/// - vector_get_at(): Gets the value at a specified position.
/// - vector_set_at(): Sets the value at a specified position.
/// - vector_find(): Finds and returns the first element equivalent to a value.
/// - vector_find_node(): Finds and returns the first node with value equivalent to a value. 
/// - vector_node_value(): Returns value of a node.
/// - vector_first(): Returns the first node of a vector.
/// - vector_last(): Returns the last node of a vector.
/// - vector_next(): Returns the next node of a node.
/// - vector_previous(): Returns the previous node of a node.
/// - vector_size(): Returns the number of elements in a vector.
/// - vector_set_destroy_value(): Changes function called each time a value is
///                               removed/overwriten, to destroy_value.

#pragma once

#include "common_types.h" // DestroyFunc
#include <stdlib.h>       // size_t

#define VECTOR_ERROR (Vector)0 ///< Defines a failed operation.

typedef struct vector *Vector;

/// Creates and returns a new vector with specified size and elements
/// initialized to NULL.
///
/// @param destroy_value If destroy_value != NULL, call destroy_value(value)
/// each time a value is removed/overwritten.
///
/// @return Newly created Vector, or VECTOR_ERROR, if failed to allocate enough
/// memory.
///
Vector vector_create(int size, DestroyFunc destroy_value);

/// Frees all the memory allocated by VEC.
///
/// Any operation on VEC after its destruction, results in undefined
/// behaviour.
///
void vector_destroy(Vector vec);

/// Changes function called each time a value is removed/overwriten, to
/// destroy_value.
///
/// @return Previous destroy_value function.
///
DestroyFunc vector_set_destroy_value(Vector vec, DestroyFunc destroy_value);

/// Returns the number of elements in VEC.
///
size_t vector_size(Vector vec);

/// Inserts VALUE at the end of VEC. Increases the size of VEC by 1.
///
void vector_insert_last(Vector vec, void *value);

/// Removes the last element of VEC. Decreases the size of VEC by 1.
///
/// If VEC is empty, nothing is done.
///
void vector_remove_last(Vector vec);

/// Finds and returns the first element equivalent to VALUE (based on COMPARE).
///
/// @return First element equivalent to VALUE, or NULL, if element not found.
///
void *vector_find(Vector vec, void *value, CompareFunc compare);

/// Gets the value at specified position.
///
/// If POS < 0 gets value of first element. If POS > size gets value of last
/// element.
///
/// @return Value at specified position.
///
void *vector_get_at(Vector vec, int pos);

/// Sets the value at specified position.
///
/// If POS < 0 sets value of first element. If POS > size sets value of last
/// element.
///
void vector_set_at(Vector vec, int pos, void *value);

#define VECTOR_BOF (VectorNode)0 ///< Defines the virtual beginning of Vector.
#define VECTOR_EOF (VectorNode)0 ///< Defines the virtual end of Vector.

typedef struct vector_node *VectorNode;

/// Finds and returns the first node with value equivalent to VALUE (based on
/// COMPARE).
///
/// @return First node with value equivalent to VALUE, or VECTOR_EOF, if node
/// with value not found.
///
VectorNode vector_find_node(Vector vec, void *value, CompareFunc compare);

/// Returns value of NODE.
///
void *vector_node_value(Vector vec, VectorNode node);

/// Returns the first node of VEC, or VECTOR_EOF, if VEC is empty.
///
VectorNode vector_first(Vector vec);

/// Returns the last node of VEC, or VECTOR_EOF, if VEC is empty.
///
VectorNode vector_last(Vector vec);

/// Returns the next node of NODE, or VECTOR_EOF, if NODE is last.
///
VectorNode vector_next(Vector vec, VectorNode node);

/// Returns the previous node of NODE, or VECTOR_BOF, if NODE is first.
///
VectorNode vector_previous(Vector vec, VectorNode node);
