/// \file set.h
///
/// Set Abstract Data Type.
///
/// Implementation independent.
///
/// The user does not need to know how a Set is implemented, they use the API
/// functions provided `set_<operation>` with the appropriate parameters.

#ifndef SET_H
#define SET_H

#include "common_types.h" // CompareFunc, DestroyFunc
#include <stdbool.h>      // bool
#include <stddef.h>       // size_t

/// Set type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a Set is implemented, they use the API
/// functions provided `set_<operation>` with the appropriate parameters.
typedef struct set *Set;

/// Allocate space for a new set.
///
/// \p compare , compares two elements *a* and *b*:
/// - If *a* < *b* , return number < 0.
/// - If *a* > *b* , return number > 0.
/// - If *a* equivalent to *b*, return 0.
///
/// If \p destroy_value is not NULL, then when an element gets removed,
/// `destroy_value(value)` is called to deallocate the space held by value.
///
/// \param compare Compares two elements. \sa CompareFunc.
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Newly created set, or NULL if an error occured.
Set set_create(CompareFunc compare, DestroyFunc destroy_value);

/// Deallocate the space held by \p set .
///
/// Any operation on \p set after its destruction, causes undefined behaviour.
void set_destroy(Set set);

/// Return the number of elements in the \p set.
int set_size(Set set);

/// Insert \p value to \p set.
///
/// If there is a value equivalent to \p value already present in \p set,
/// overwrite it.
///
/// \warning While a value is part of \p set, any change to its content causes
/// undefined behaviour.
void set_insert(Set set, void *value);

/// Remove \p value from \p set.
///
/// \p value can not be `NULL`.
///
/// \return `true` if removed successfully, otherwise false.
bool set_remove(Set set, void *value);

/// Find and return value equivalent to \p value.
///
/// \p value can not be `NULL`.
///
/// \return Value equivalent to \p value, or NULL if \p value is not part of
/// \p set.
void *set_find(Set set, void *value);

/// Changes the destroy function called on each value's removal/overwrite
/// to \p destroy_value .
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Previous `destroy_value` function.
DestroyFunc set_set_destroy_value(Set set, DestroyFunc destroy_value);

/// SetNode type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a SetNode is implemented, they use the
/// API functions provided `set_<operation>_node` and `set_node_<operation>`
/// with the appropriate parameters.
typedef struct set_node *SetNode;

#define SET_BOF (SetNode)0 ///< Defines the virtual beginning of the Set.
#define SET_EOF (SetNode)0 ///< Defines the virtual end of the Set.

/// Return the value of \p node .
///
/// If \p node is `NULL`, it causes to undefined behaviour.
void *set_node_value(Set set, SetNode node);

/// Find and return the first node with value equivalent to \p value.
///
/// \p compare , compares two elements \p a and \p b :
/// - If \p a < \p b , return number < 0.
/// - If \p a > \p b , return number > 0.
/// - If \p a equivalent to \p b , return 0.
///
/// \param compare Compares two elements. \sa CompareFunc.
///
/// \return Node with value equivalent to \p value , or `SET_EOF` if value was
/// not found.
SetNode set_find_node(Set set, void *value);

/// \defgroup traversal Traversal functions
///@{

/// Return first node, or `SET_BOF` if \p set is empty.
SetNode set_first(Set set);

/// Return last node, or `SET_EOF` if \p set is empty.
SetNode set_last(Set set);

/// Return the next node of \p node , or `SET_EOF` if \p node is the last node
/// in \p set.
SetNode set_next(Set set, SetNode node);

/// Return the previous node of \p node , or `SET_BOF` if \p node is the first
/// node in \p set.
SetNode set_previous(Set set, SetNode node);

///@}  // End of traversal.

#endif // SET_H
