/// \file pqueue.h
///
/// Priority Queue Abstract Data Type.
///
/// Implementation independent.
///
/// The user does not need to know how a Priority Queue is implemented, they use
/// the API functions provided `pqueue_<operation>` with the appropriate
/// parameters.

#ifndef PQUEUE_H
#define PQUEUE_H

#include "common_types.h" // CompareFunc, DestroyFunc
#include "vector.h"       // Vector
#include <stdbool.h>      // bool
#include <stddef.h>       // size_t

/// Priority Queue type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a Priority Queue is implemented, they use
/// the API functions provided `pqueue_<operation>` with the appropriate
/// parameters.
typedef struct priority_queue *PQueue;

/// Allocate space for a new priority queue.
///
/// Elements are compared based on \p compare .
/// \p compare should work as follows when comparing two elements (*a* and *b*):
/// - If *a* < *b*, return number < 0.
/// - If *a* > *b*, return number > 0.
/// - If *a* equivalent to *b*, return 0.
///
/// If \p destroy_value is not NULL, then when an element gets removed,
/// `destroy_value(value)` is called to deallocate the space held by value.
///
/// \param compare Compares two elements. \sa CompareFunc.
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
/// \param values Initialize the priority queue using \p values, if not `NULL`.
///
/// \return Newly created priority queue, or NULL if an error occured.
PQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value,
                     Vector values);

/// Deallocate the space held by \p pqueue .
///
/// Any operation on \p pqueue after its destruction, causes undefined
/// behaviour.
void pqueue_destroy(PQueue pqueue);

/// Return the highest-priority value of \p pqueue without removing it.
///
/// \return Highest-priority element of \p pqueue, or NULL, if \p pqueue is
/// empty.
void *pqueue_peek(PQueue pqueue);

/// Add \p value to \p pqueue.
void pqueue_insert(PQueue pqueue, void *value);

/// Remove the highest-priority element from \p pqueue.
void pqueue_pull(PQueue pqueue);

/// Return the number of elements in \p pqueue.
size_t pqueue_size(PQueue pqueue);

/// Returns `true` if \p pqueue is empty, else, `false`.
bool pqueue_is_empty(PQueue pqueue);

/// Change the function called on each element's removal to
/// \p destroy_value .
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Previous `destroy_value` function.
DestroyFunc pqueue_set_destroy_value(PQueue pqueue, DestroyFunc destroy_value);

#endif // PQUEUE_H