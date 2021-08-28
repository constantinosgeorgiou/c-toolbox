/// \file vector.h
///
/// Vector Abstract Data Type.
///
/// Implementation independent.
///
/// The user does not need to know how a Vector is implemented, they use the API
/// functions provided `vector_<operation>` with the appropriate parameters.

#ifndef VECTOR_H
#define VECTOR_H

#include "common_types.h" // CompareFunc, DestroyFunc
#include <stddef.h>       // size_t

/// Vector type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a Vector is implemented, they use the API
/// functions provided `vector_<operation>` with the appropriate parameters.
typedef struct vector *Vector;

/// Creates and returns a new vector with specified size and elements
/// initialized to NULL.
///
/// \param destroy_value If destroy_value != NULL, call destroy_value(value)
/// each time a value is removed/overwritten.
///
/// \return Newly created Vector, or VECTOR_ERROR, if failed to allocate enough
/// memory.
///

/// Allocates space for a new vector with \p size elements initialized to
/// `NULL`.
///
/// If \p destroy_value is not NULL, then when an element gets removed,
/// `destroy_value(value)` is called to deallocate the space held by value.
///
/// \param size Number of elements in new vector.
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Newly created Vector, or NULL, if an error occured.
Vector vector_create(int size, DestroyFunc destroy_value);

/// Deallocate the space held by \p vec .
///
/// Any operation on the \p vec after its destruction, results in
/// undefined behaviour.
void vector_destroy(Vector vec);

/// Change the function called on each element's removal/overwrite to
/// \p destroy_value .
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Previous `destroy_value` function.
DestroyFunc vector_set_destroy_value(Vector vec, DestroyFunc destroy_value);

/// Return the number of elements in \p vec .
size_t vector_size(Vector vec);

/// Insert \p value at the end of \p vec.
///
/// Increases the size of VEC by 1.
void vector_insert_last(Vector vec, void *value);

/// Remove the last element of \p vec.
///
/// Decreases the size of VEC by 1.
///
/// If \p vec is empty, nothing is done.
void vector_remove_last(Vector vec);

/// Find and return the first value equivalent to \p value based on \p
/// compare.
///
/// \p compare , compares two elements \p a and \p b :
/// - If \p a < \p b , return number < 0.
/// - If \p a > \p b , return number > 0.
/// - If \p a equivalent to \p b , return 0.
///
/// \param compare Compares two elements. \sa CompareFunc.
///
/// \return First element equivalent to \p value, or NULL, if element not found.
///
void *vector_find(Vector vec, void *value, CompareFunc compare);

/// Get the value at position \p pos of \p vec.
///
/// If \p pos < 0, get value of first element.
///
/// If \p pos > size, get value of last element.
///
/// \return Value at position \p pos of \p vec.
///
void *vector_get_at(Vector vec, int pos);

/// Set the value at position \p pos of \p vec to \p value.
///
/// If \p pos < 0, set value of first element.
///
/// If \p pos > size, set value of last element.
///
void vector_set_at(Vector vec, int pos, void *value);

/// VectorNode type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a VectorNode is implemented, they use the
/// API functions provided `vector_<operation>_node` and
/// `vector_node_<operation>` with the appropriate parameters.
typedef struct vector_node *VectorNode;

#define VECTOR_BOF (VectorNode)0 ///< Defines the virtual beginning of a vector.
#define VECTOR_EOF (VectorNode)0 ///< Defines the virtual end of a vector.

/// Find and return the first node with value equivalent to \p value based on
/// \p compare .
///
/// \p compare , compares two elements \p a and \p b :
/// - If \p a < \p b , return number < 0.
/// - If \p a > \p b , return number > 0.
/// - If \p a equivalent to \p b , return 0.
///
/// \param compare Compares two elements. \sa CompareFunc.
///
/// \return Node with value equivalent to \p value , or `VECTOR_EOF` if value
/// was not found.
VectorNode vector_find_node(Vector vec, void *value, CompareFunc compare);

/// Return the value of \p node .
///
/// If \p node is `NULL`, it causes to undefined behaviour.
void *vector_node_value(Vector vec, VectorNode node);

/// \defgroup traversal Traversal functions
///@{

/// Return first node of \p vec, or `VECTOR_BOF` if \p vec is empty.
VectorNode vector_first(Vector vec);

/// Return the last node of \p vec, or `VECTOR_EOF`, if \p vec is empty.
VectorNode vector_last(Vector vec);

/// Return the next node of \p node , or `VECTOR_EOF` if \p node is the last
/// node in \p vec.
VectorNode vector_next(Vector vec, VectorNode node);

/// Return the previous node of \p node , or `VECTOR_BOF` if \p node is the
/// first node in \p vec.
VectorNode vector_previous(Vector vec, VectorNode node);

///@} // End of traversal.

#endif // VECTOR_H
