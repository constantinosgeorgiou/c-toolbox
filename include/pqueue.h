/// @file pqueue.h
///
/// Priority Queue Abstract Data Type.
///
/// Implementation independent.
///
/// The user does not need to know how a Priority Queue is implemented, they use
/// the API functions provided `pqueue_<operation>` with the appropriate
/// parameters.
///
/// Operations supported:
/// - pqueue_create(): Allocates memory for a priority queue.
/// - pqueue_destroy(): Frees the allocated memory of a queue.
/// - pqueue_peek(): Returns the highest-priority element but does not remove
///                  it.
/// - pqueue_insert(): Adds an element to the priority queue.
/// - pqueue_pull(): Removes the highest-priority element.
/// - pqueue_size(): Returns the number of values in a queue.
/// - pqueue_is_empty(): Returns true if queue is empty, or false if not.
/// - pqueue_set_destroy_value(): Changes the function called on each element
///                               removal/overwrite, to given function.

#pragma once

#include <stdbool.h> // bool
#include <stdlib.h>  // size_t

#include "common_types.h" // DestroyFunc
#include "vector.h"       // Vector

/// Priority Queue type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a Priority Queue is implemented, they use
/// the API functions provided `pqueue_<operation>` with the appropriate
/// parameters.
///
typedef struct priority_queue *PQueue;

/// Creates and returns a priority queue.
///
/// @param compare Compares two values. Defines the priority of the values.
/// @param destroy_value If destroy_value != NULL, call
///                        `destroy_value(value)` each time a value is
///                        removed.
/// @param values If values != NULL, initialize the priority queue using
///                 VALUES.
/// @return
///
PQueue pqueue_create(CompareFunc compare, DestroyFunc destroy_value,
                     Vector values);

/// Frees all the memory allocated for PQUEUE.
///
/// Any operation on PQUEUE after its destruction, results in undefined
/// behaviour.
///
void pqueue_destroy(PQueue pqueue);

/// Returns the highest-priority value of PQUEUE without removing it.
///
/// @return Highest-priority element of PQUEUE, or NULL, if PQUEUE is empty.
///
void *pqueue_peek(PQueue pqueue);

/// Adds ELEMENT to the PQUEUE.
///
void pqueue_insert(PQueue pqueue, void *element);

/// Removes the highest-priority element.
///
void pqueue_pull(PQueue pqueue);

/// Returns the number of values in PQUEUE.
///
/// @return The number of values in PQUEUE.
///
size_t pqueue_size(PQueue pqueue);

/// Returns `true` if PQUEUE is empty, else, `false`.
///
bool pqueue_is_empty(PQueue pqueue);

/// Changes function called each time a value is removed to DESTROY_VALUE.
///
/// @return Previous destroy_value function.
///
DestroyFunc pqueue_set_destroy_value(PQueue pqueue, DestroyFunc destroy_value);
