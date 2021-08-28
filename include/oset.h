/// @file oset.h
///
/// Ordered Set Abstract Data Type.
///
/// Implementation independent.
///
/// The user does not need to know how an Ordered Set is implemented, they use
/// the API functions provided `oset_<operation>` with the appropriate
/// parameters.

#ifndef OSET_H
#define OSET_H

#include "common_types.h" // CompareFunc, DestroyFunc
#include <stdbool.h>      // bool
#include <stddef.h>       // size_t

/// OrderedSet type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how an OrderedSet is implemented, they use
/// the API functions provided `oset_<operation>` with the appropriate
/// parameters.
typedef struct ordered_set *OrderedSet;

/// Allocate space for a new ordered set.
///
/// Elements are compared based on \p compare .
/// \p compare should work as follows when comparing two elements (*a* and *b*):
/// - If *a* < *b*, return number < 0.
/// - If *a* > *b*, return number > 0.
/// - If *a* equivalent to *b*, return 0.
///
/// If \p destroy_key is not NULL, then when an element gets removed,
/// `destroy_key(key)` is called to deallocate the space held by key.
///
/// If \p destroy_value is not NULL, then when an element gets removed,
/// `destroy_value(value)` is called to deallocate the space held by value.
///
/// \param compare Compares two elements. \sa CompareFunc.
/// \param destroy_key When an element gets removed, `destroy_key(key)` is
/// called, if not NULL, to deallocate the space held by key.
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Newly created ordered set, or NULL if an error occured.
OrderedSet oset_create(CompareFunc compare, DestroyFunc destroy_key,
                       DestroyFunc destroy_value);

/// Deallocate the space held by \p oset .
///
/// Any operation on \p oset after its destruction, causes undefined behaviour.
void oset_destroy(OrderedSet oset);

/// Changes the destroy function called on each key's removal/overwrite
/// to \p destroy_key .
///
/// Typical usage:
/// \code {.c}
///   int compare_ints(const void* a, const void* b){
///     return *(int*)a - *(int*)b;
///   }
///
///   OrderedSet myoset = oset_create(compare_ints, free, NULL);
///   DestroyFunc old = set_set_destroy_key(myoset, NULL);
///   // old == free
/// \endcode
///
/// \param destroy_key When an element gets removed, `destroy_key(key)` is
/// called, if not NULL, to deallocate the space held by key.
///
/// \return Previous `destroy_key` function.
DestroyFunc oset_set_destroy_key(OrderedSet oset, DestroyFunc destroy_key);

/// Changes the destroy function called on each value's removal/overwrite
/// to \p destroy_value .
///
/// Typical usage:
/// \code {.c}
///   int compare_ints(const void* a, const void* b){
///     return *(int*)a - *(int*)b;
///   }
///
///   OrderedSet myoset = oset_create(compare_ints, NULL, free);
///   DestroyFunc old = oset_set_destroy_value(mymap, NULL);
///   // old == free
/// \endcode
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Previous `destroy_value` function.
DestroyFunc oset_set_destroy_value(OrderedSet oset, DestroyFunc destroy_value);

/// Return the number of elements in the \p oset .
size_t oset_size(OrderedSet oset);

/// Associates a key with a value (inserting the key if not already
/// present). Duplicate keys are treated like a stack, Last In First Out.
///
/// Keys can not be NULL.
///

/// Associates \p key with \p value.
///
/// Insert \p key if not already present.
///
/// Duplicate keys are treated like a stack, *Last In First Out*.
///
/// \p key can not be `NULL`.
///
/// While a key is part of \p oset , any change to its content causes
/// undefined behaviour.
void oset_insert(OrderedSet oset, void *key, void *value);

/// Remove \p key from \p oset .
///
/// Duplicate keys are treated like a stack, *Last In First Out*.
///
/// \p key can not be `NULL`.
///
/// \param oset
/// \param key
/// \return true, if \p key was removed successfully, otherwise false.
bool oset_remove(OrderedSet oset, void *key);

/// Find and return the value associated with \p key .
///
/// Duplicate keys are treated like a stack, *Last In First Out*.
///
/// \p key can not be `NULL`.
///
/// \return Value associated with \p key , or NULL if \p key is not part of
/// \p oset .
void *oset_find(OrderedSet oset, void *key);

/// Remove all elements with keys >= \p split_key and return them in a new
/// ordered set.
///
/// \p split_key can not be NULL.
///
/// if \p split_key < smallest_key, transfer all elements to newly created
/// ordered set.
///
/// if \p split_key > largest_key, return an empty ordered set.
///
/// Splitting an empty ordered set results in an error.
///
/// @return Newly created Ordered Set with keys >= \p split_key, or NULL, if an
/// error occurred.
OrderedSet oset_split(OrderedSet oset, void *split_key);

/// Create and return a new ordered set containing the elements of \p a and
/// \p b .
///
/// If an element appears in both ordered sets, use the value field from \p b .
///
/// If \p a is \p b , it causes undefined behaviour.
///
/// Any operation on \p a and \p b after they are merged causes undefined
/// behaviour.
OrderedSet oset_merge(OrderedSet a, OrderedSet b);

/// Append \p b to the end of \p a .
///
/// If last_key in \p a > first_key in \p b , it causes undefined behaviour.
///
/// If \p a is \p b , it causes undefined behaviour.
///
/// Any operation on \p b after it is merged with \p a causes undefined
/// behaviour.
void oset_concat(OrderedSet a, OrderedSet b);

/// OrderedSetNode type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a OrderedSetNode is implemented, they use
/// the API functions provided `oset_<operation>_node` and
/// `oset_node_<operation>` with the appropriate parameters.
typedef struct ordered_set_node *OrderedSetNode;

/// Defines the virtual beginning of the Ordered Set.
#define OSET_BOF (OrderedSetNode)0

/// Defines the virtual end of the Ordered Set.
#define OSET_EOF (OrderedSetNode)0

/// Find and return the node associated with \p key .
///
/// Duplicate keys are treated like a stack, *Last In First Out*.
///
/// \p key can not be `NULL`.
///
/// \return Value associated with \p key , or NULL if \p key is not part of
/// \p oset .
OrderedSetNode oset_find_node(OrderedSet oset, void *key);

/// Return the key of \p node .
///
/// If \p node is `NULL`, it causes to undefined behaviour.
void *oset_node_key(OrderedSet oset, OrderedSetNode node);

/// Return the value of \p node .
///
/// If \p node is `NULL`, it causes to undefined behaviour.
void *oset_node_value(OrderedSet oset, OrderedSetNode node);

/// \defgroup traversal Traversal functions
///@{

/// Return first node, or `OSET_BOF` if \p oset is empty.
OrderedSetNode oset_first(OrderedSet oset);

/// Return last node, or `OSET_EOF` if \p oset is empty.
OrderedSetNode oset_last(OrderedSet oset);

/// Return the next node of \p node , or `OSET_EOF` if \p node is the last node
/// in \p oset .
OrderedSetNode oset_next(OrderedSet oset, OrderedSetNode node);

/// Return the previous node of \p node , or `OSET_BOF` if \p node is the first
/// node in \p oset .
OrderedSetNode oset_previous(OrderedSet oset, OrderedSetNode node);

///@}  // End of traversal.

#endif // OSET_H