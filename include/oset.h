/// @file oset.h
///
/// Ordered Set Abstract Data Type.
///
/// Implementation independent.
///
/// The user does not need to know how an Ordered Set is implemented, they use
/// the API functions provided `oset_<operation>` with the appropriate
/// parameters.
///
/// Operations supported:
/// - oset_create(): Creates and returns an ordered set.
/// - oset_destroy(): Frees all the memory allocated by an ordered set.
/// - oset_size(): Returns the number of elements in an ordered set.
/// - oset_insert(): Associates a key with a value.
/// - oset_remove(): Removes a key.
/// - oset_find(): Finds the value associated with a key.
/// - oset_find_node(): Finds the node of the value associated with
/// a key.
/// - oset_split(): Removes all elements with keys >= split_key and returns them
/// in a new ordered set.
/// - oset_merge(): Creates an ordered set containing the elements
/// of specified ordered sets.
/// - oset_concat(): Appends ordered set B to the end of ordered set A.
/// - oset_node_key(): Returns key of a node.
/// - oset_node_value(): Returns value of a node.
/// - oset_first(): Returns the first node of an ordered set.
/// - oset_last(): Returns the last node of an ordered set.
/// - oset_next(): Returns the next node of a node.
/// - oset_previous(): Returns the previous node of a node.
/// - oset_set_destroy_key(): Changes function called each time a key is
/// removed.
/// - oset_set_destroy_value(): Changes function called each time a value is
/// removed.

#pragma once

#include "common_types.h"

#define OSET_ERROR (OrderedSet)0

#include <stdbool.h> // bool
#include <stddef.h>  // size_t

typedef struct ordered_set *OrderedSet;
typedef struct ordered_set_node *OrderedSetNode;

/// @brief Creates and returns an Ordered Set.
///
/// @param compare Compares two elements.
/// @param destroy_key If destroy_key != NULL, call destroy_key(key) each time a
/// key is removed.
/// @param destroy_value If destroy_value != NULL, call destroy_value(value)
/// each time a value is
///                      removed.
///
/// @return Newly created Ordered Set, or OSET_ERROR, if failed to allocate
/// enough memory.
///
OrderedSet oset_create(CompareFunc compare, DestroyFunc destroy_key,
                       DestroyFunc destroy_value);

/// @brief Frees all the memory allocated by given Ordered Set.
///
/// Any operation on the Ordered Set after its destruction, results in undefined
/// behaviour.
///
void oset_destroy(OrderedSet oset);

/// @brief Changes function called each time a key is removed to destroy_key.
///
/// @return Previous destroy_key function.
///
DestroyFunc oset_set_destroy_key(OrderedSet oset, DestroyFunc destroy_key);

/// @brief Changes function called each time a value is removed to
/// destroy_value.
///
/// @return Previous destroy_value function.
///
DestroyFunc oset_set_destroy_value(OrderedSet oset, DestroyFunc destroy_value);

/// @brief Returns the number of elements in the Ordered Set.
///
/// @return The number of elements in the Ordered Set.
///
size_t oset_size(OrderedSet oset);

/// @brief Associates a key with a value (inserting the key if not already
/// present). Duplicate keys are treated like a stack, Last In First Out.
///
/// Keys can not be NULL.
///
void oset_insert(OrderedSet oset, void *key, void *value);

/// @brief Removes specified key.
///        Duplicate keys are treated like a stack, Last In First Out.
///
/// Keys can not be NULL.
///
/// @return true, if key was removed successfully, otherwise false.
///
bool oset_remove(OrderedSet oset, void *key);

/// @brief Finds and returns the value associated with specified key.
///        Duplicate keys are treated like a stack, Last In First Out.
///
/// Keys can not be NULL.
///
/// @return Value associated with specified key, or NULL, if key not found.
///
void *oset_find(OrderedSet oset, void *key);

/// @brief Removes all elements with keys >= split_key and returns them in a new
///        Ordered Set.
///
/// split_key can not be NULL. split_key < smallest_key transfers all elements
/// to newly created Ordered Set. split_key > largest_key returns an empty
/// Ordered Set. Splitting an empty Ordered Set results in an error.
///
/// @return Newly created Ordered Set with keys >= split_key, or OSET_ERROR, if
/// an error occurred.
///
OrderedSet oset_split(OrderedSet oset, void *split_key);

/// @brief Creates and returns a new Ordered Set containing the elements of
/// Ordered Set a and
///        Ordered Set b.
///
/// If an element appears in both Ordered Sets, use the value field from Ordered
/// Set  b  . Any operation on the Ordered Sets  a  and  b  after they are
/// merged results in undefined behaviour. a == b, results in undefined
/// behaviour.
///
OrderedSet oset_merge(OrderedSet a, OrderedSet b);

/// @brief Appends B to the end of A.
///
/// last_key in a > first_key in b, results in undefined behaviour. a == b,
/// results in undefined behaviour. Any operation on  b  after it is merged with
/// a  results in undefined behaviour.
///
void oset_concat(OrderedSet a, OrderedSet b);

#define OSET_BOF                                                               \
    (OrderedSetNode)0 // Defines the virtual beginning of the Ordered Set.
#define OSET_EOF                                                               \
    (OrderedSetNode)0 // Defines the virtual end of the Ordered Set.

/// @brief Finds and returns the node of the value associated with specified
/// key.
///        Duplicate keys are treated like a stack, Last In First Out.
///
/// Keys can not be NULL.
///
/// @return Node of the value associated with specified key, or OSET_EOF, if key
/// not found.
///
OrderedSetNode oset_find_node(OrderedSet oset, void *key);

/// @brief Returns key of specified node.
///
void *oset_node_key(OrderedSet oset, OrderedSetNode node);

/// @brief Returns value of specified node.
///
void *oset_node_value(OrderedSet oset, OrderedSetNode node);

/// @brief Returns the first node of the Ordered Set, or SET_EOF, if Ordered set
/// is empty.
///
OrderedSetNode oset_first(OrderedSet oset);

/// @brief Returns the last node of the Ordered Set, or SET_EOF, if Ordered set
/// is empty.
///
OrderedSetNode oset_last(OrderedSet oset);

/// @brief Returns the next node of node, or OSET_EOF, if node is last.
///
OrderedSetNode oset_next(OrderedSet oset, OrderedSetNode node);

/// @brief Returns the previous node of node, or OSET_BOF, if node is first.
///
OrderedSetNode oset_previous(OrderedSet oset, OrderedSetNode node);
