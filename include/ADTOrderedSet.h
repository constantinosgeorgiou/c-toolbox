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

#define OSET_ERROR (OrderedSet)0

#include <stdbool.h>  // bool

typedef struct ordered_set* OrderedSet;
typedef struct ordered_set_node* OrderedSetNode;

/// @brief Creates and returns an Ordered Set.
///
/// @param compare Compares two elements.
/// @param destroy_key If destroy_key != NULL, call destroy_key(key) each time a key is removed.
/// @param destroy_value If destroy_value != NULL, call destroy_value(value) each time a value is
///                      removed.
///
/// @return Newly created Ordered Set, or OSET_ERROR, if failed to allocate enough memory.
///
OrderedSet oset_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value);

/// @brief Frees all the memory allocated by given Ordered Set.
///
/// Any operation on the Ordered Set after its destruction, results in undefined behaviour.
///
void oset_destroy(OrderedSet oset);

/// @brief Changes function called each time a key is removed to destroy_key.
///
/// @return Previous destroy_key function.
///
DestroyFunc oset_set_destroy_key(OrderedSet oset, DestroyFunc destroy_key);

/// @brief Changes function called each time a value is removed to destroy_value.
///
/// @return Previous destroy_value function.
///
DestroyFunc oset_set_destroy_value(OrderedSet oset, DestroyFunc destroy_value);

/// @brief Returns the number of elements in the Ordered Set.
///
/// @return The number of elements in the Ordered Set.
///
int oset_size(OrderedSet oset);

/// @brief Associates a key with a value (inserting the key if not already present).
///        Duplicate keys are treated like a stack, Last In First Out.
///
/// Keys can not be NULL.
///
void oset_insert(OrderedSet oset, void* key, void* value);

/// @brief Removes specified key.
///        Duplicate keys are treated like a stack, Last In First Out.
///
/// Keys can not be NULL.
///
/// @return true, if key was removed successfully, otherwise false.
///
bool oset_remove(OrderedSet oset, void* key);

/// @brief Finds and returns the value associated with specified key.
///        Duplicate keys are treated like a stack, Last In First Out.
///
/// Keys can not be NULL.
///
/// @return Value associated with specified key, or NULL, if key not found.
///
void* oset_find(OrderedSet oset, void* key);

/// @brief Gets the value at specified position.
///
/// pos >= size returns last value. pos < 0 returns first value.
///
/// @return Value at specified position.
///
void* oset_get_at(OrderedSet oset, int pos);

/// @brief Removes element at specified position.
///
/// pos >= size removes last value. pos < 0 removes first value.
///
/// @return true, if removed successfully, otherwise false.
///
bool oset_remove_at(OrderedSet oset, int pos);

/// @brief Removes all elements with keys >= split_key and returns them in a new
///        Ordered Set.
///
/// split_key can not be NULL. split_key < smallest_key and split_key > largest_key returns an
/// empty Ordered Set. Splitting an empty Ordered Set results in an error.
///
/// @return Newly created Ordered Set with keys >= split_key, or OSET_ERROR, if an error occurred.
///
OrderedSet oset_split(OrderedSet oset, void* split_key);

/// @brief Merges two Ordered Sets.
///
/// Any operation on  b  after it is merged with  a  results in undefined behaviour.
/// a == b, results in undefined behaviour.
///
void oset_merge(OrderedSet a, OrderedSet b);

/// @brief Concatenates two Ordered Sets.
///
/// last_key in a > first_key in b, results in undefined behaviour.
///
void oset_concat(OrderedSet a, OrderedSet b);

#define OSET_BOF (OrderedSetNode)0  // Defines the virtual beginning of the Ordered Set.
#define OSET_EOF (OrderedSetNode)0  // Defines the virtual end of the Ordered Set.

/// @brief Finds and returns the node of the value associated with specified key.
///        Duplicate keys are treated like a stack, Last In First Out.
///
/// Keys can not be NULL.
///
/// @return Node of the value associated with specified key, or OSET_EOF, if key not found.
///
OrderedSetNode oset_find_node(OrderedSet oset, void* key);

/// @brief Gets the node at specified position.
///
/// pos >= size returns last value. pos < 0 returns first value.
///
/// @return Node at specified position.
///
OrderedSetNode oset_get_at_node(OrderedSet oset, int pos);

/// @brief Returns key of specified node.
///
void* oset_node_key(OrderedSet oset, OrderedSetNode node);

/// @brief Returns value of specified node.
///
void* oset_node_value(OrderedSet oset, OrderedSetNode node);

/// @brief Returns the first node of the Ordered Set, or SET_EOF, if Ordered set is empty.
///
OrderedSetNode oset_first(OrderedSet oset);

/// @brief Returns the last node of the Ordered Set, or SET_EOF, if Ordered set is empty.
///
OrderedSetNode oset_last(OrderedSet oset);

/// @brief Returns the next node of node, or OSET_EOF, if node is last.
///
OrderedSetNode oset_next(OrderedSet oset, OrderedSetNode node);

/// @brief Returns the previous node of node, or OSET_BOF, if node is first.
///
OrderedSetNode oset_previous(OrderedSet oset, OrderedSetNode node);
