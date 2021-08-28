/// @file queue.h
///
/// Queue Abstract Data Type.
///
/// Implementation indepentent.
///
/// The user does not need to know how a Queue is implemented, they use the API
/// functions provided `queue_<operation>` with the appropriate parameters.

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h> // bool
#include <stdlib.h>  // size_t

#include "common_types.h" // DestroyFunc

/// Queue type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a Queue is implemented, they use the API
/// functions provided `queue_<operation>` with the appropriate parameters.
typedef struct queue *Queue;

/// Allocate space for a new queue.
///
/// If \p destroy_value is not NULL, then when an element gets removed,
/// `destroy_value(value)` is called to deallocate the space held by value.
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// @return Newly created queue, or NULL if an error occured.
Queue queue_create(DestroyFunc destroy_value);

///  Deallocate the space held by \p queue.
///
/// Any operation on the \p queue after its destruction, results in
/// undefined behaviour.
void queue_destroy(Queue queue);

/// Add \p value to the back of \p queue.
void queue_enqueue(Queue queue, void *value);

/// Remove an element from the front of \p queue.
///
/// If \p queue is empty, nothing happens.
void queue_dequeue(Queue queue);

/// Return the number of elements in \p queue.
size_t queue_size(Queue queue);

/// Return `true` if \p queue is empty, else, `false`.
bool queue_is_empty(Queue queue);

/// Return the first element of \p queue, or NULL, if \p queue is empty.
void *queue_front(Queue queue);

/// Return the last element of \p queue, or NULL, if \p queue is empty.
void *queue_back(Queue queue);

/// Change the function called on each element's removal to
/// \p destroy_value .
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Previous `destroy_value` function.
DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value);

#endif