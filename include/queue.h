/// @file queue.h
///
/// Queue Abstract Data Type.
///
/// Implementation indepentent.
///
/// The user does not need to know how a Queue is implemented, they use the API
/// functions provided `queue_<operation>` with the appropriate parameters.
///
/// Operations supported:
/// - queue_create(): Allocates memory for a queue.
/// - queue_destroy(): Frees the allocated memory of a queue.
/// - queue_enqueue(): Adds an element to the back of a queue.
/// - queue_dequeue(): Removes an element from the front of a queue.
/// - queue_size(): Returns the number of elements in a queue.
/// - queue_is_empty(): Returns true if queue is empty, or false if not.
/// - queue_front(): Returns the first element of a queue.
/// - queue_back(): Returns the last element of a queue.
/// - queue_set_destroy_value(): Changes the function called on each element
///                              removal/overwrite, to given function.

#pragma once

#include <stdbool.h>  // bool
#include <stdlib.h>   // size_t

#include "common_types.h"  // DestroyFunc

/// Queue type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a Queue is implemented, they use the API
/// functions provided `queue_<operation>` with the appropriate parameters.
///
typedef struct queue* Queue;

/// Creates and returns a queue.
///
/// @param destroy_value If destroy_value != NULL, call `destroy_value(value)`
///                      each time a value is removed.
/// @return Newly created queue.
///
Queue queue_create(DestroyFunc destroy_value);

/// Frees all the memory allocated for QUEUE.
///
/// Any operation on QUEUE after its destruction, results in undefined
/// behaviour.
///
void queue_destroy(Queue queue);

/// Adds VALUE to the back of QUEUE.
///
void queue_enqueue(Queue queue, void* value);

/// Removes an element from the front of QUEUE.
///
/// If QUEUE is empty, nothing happens.
///
void queue_dequeue(Queue queue);

/// Returns the number of elements in QUEUE.
///
/// @return The number of elements in QUEUE.
///
size_t queue_size(Queue queue);

/// Returns `true` if QUEUE is empty, else, `false`.
///
bool queue_is_empty(Queue queue);

/// Returns the first element of QUEUE, or NULL, if QUEUE is empty.
///
void* queue_front(Queue queue);

/// Returns the last element of QUEUE, or NULL, if QUEUE is empty.
///
void* queue_back(Queue queue);

/// Changes function called each time a value is removed to DESTROY_VALUE.
///
/// @return Previous destroy_value function.
///
DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value);
