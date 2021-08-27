/// \file slist.h
///
/// Singly Linked List Abstract Data Type.
///
/// Implementation independent.
///
/// The user does not need to know how a Singly Linked List is implemented, they
/// use the API functions provided `slist_<operation>` with the appropriate
/// parameters.

#ifndef SLIST_H
#define SLIST_H

#include "common_types.h" // CompareFunc, DestroyFunc
#include <stdlib.h>       // size_t

/// SList type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a Singly Linked List is implemented, they
/// use the API functions provided `slist_<operation>` with the appropriate
/// parameters.
typedef struct slist *SList;

/// SListNode type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a SListNode is implemented, they use the
/// API functions provided `slist_<operation>_node` and `slist_node_<operation>`
/// with the appropriate parameters.
typedef struct slist_node *SListNode;

#define SLIST_BOF (SListNode)0 ///< Defines the "virtual" beginning of a slist.
#define SLIST_EOF (SListNode)0 ///< Defines the "virtual" end of a slist.

/// Creates and returns a new slist.
///
/// @param destroy If destroy != NULL, then each time an item is removed,
///                destroy(value) is called.
///
/// @return Newly created slist.
///

/// Allocate space for a new singly linked list.
///
/// If \p destroy_value is not NULL, then when an element gets removed,
/// `destroy_value(value)` is called to deallocate the space held by value.
///
/// Typical usage:
/// \code {.c}
///   SList slist1 = slist_create(NULL);
///   SList slist2 = slist_create(free);
/// \endcode
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Newly created list, or NULL if an error occured.
SList slist_create(DestroyFunc destroy_value);

/// Deallocate the space held by \p slist .
///
/// Any operation on \p slist after its destruction, causes in undefined
/// behaviour.
///
/// Typical usage:
/// \code {.c}
///   slist_destroy(mylist);
/// \endcode
void slist_destroy(SList slist);

/// Change the function called on each element's removal to
/// \p destroy_value .
///
/// Typical usage:
/// \code {.c}
///   SList mylist = slist_create(free);
///   DestroyFunc old = slist_set_destroy_value(mylist, NULL);
///   // old == free
/// \endcode
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Previous `destroy_value` function.
DestroyFunc slist_set_destroy_value(SList slist, DestroyFunc destroy);

/// Return the number of elements in \p slist .
size_t slist_size(SList);

/// Insert a new node with \p value after \p node .
///
/// If \p node is `SLIST_BOF`, insert new node at the beginning.
///
/// If \p node is `SLIST_BOF`, it causes undefined behaviour.
///
///
/// Typical usage:
/// \code {.c}
///   SList numbers = slist_create(NULL);
///   // numbers: []
///
///   int number1 = 1234;
///   slist_insert_next(numbers, SLIST_BOF, &number1);
///   // numbers: [ 1234 ]
///
///
///   SListNode node = slist_first(numbers);
///   int number2 = 56;
///   slist_insert_next(numbers, node, &number2);
///   // numbers: [ 1234, 56 ]
///
///   int number3 = 0;
///   slist_insert(numbers, SLIST_BOF, &number3);
///   // numbers: [ 0, 1234, 56 ]
///
///
///   slist_destroy(numbers);
/// \endcode
void slist_insert_next(SList slist, SListNode node, void *value);

/// Remove the next node of \p node .
///
/// If \p node is `SLIST_BOF`, remove first node.
///
/// If \p node is `SLIST_EOF`, it causes undefined behaviour.
///
/// \code {.c}
///   SList numbers = slist_create(NULL);
///
///   ...
///
///   // numbers: [ 0, 5, 4, 10 ]
///
///   slist_remove_next(numbers, SLIST_BOF);
///   // numbers: [ 5, 4, 10 ]
///
///   slist_remove_next(numbers, slist_first(numbers));
///   // numbers: [ 5, 10 ]
///
/// \endcode
void slist_remove_next(SList slist, SListNode node);

/// Append \p src to the end of \p dest .
///
/// After operation any operation on \p src causes undefined behaviour.
///
/// If \p dest is \p src , it causes undefined behaviour.
///
/// \code {.c}
///   SList veggies;
///   SList fruits;
///
///   ...
///
///   // veggies: [ tomato, lettuce ]
///   // fruits: [ apple, mango, banana ]
///
///   slist_concat(fruits, veggies);
///   // fruits: [ apple, mango, banana, tomato, lettuce ]
///
/// \endcode
///
/// \return Resulting singly linked list \p dest .
SList slist_concat(SList dest, SList src);

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
/// \return Node with value equivalent to \p value , or `SLIST_EOF` if value was
/// not found.
SListNode slist_find_node(SList slist, void *value, CompareFunc compare);

/// Find and return the first value equivalent to \p value based on
/// \p compare .
///
/// \p compare , compares two elements \p a and \p b :
/// - If \p a < \p b , return number < 0.
/// - If \p a > \p b , return number > 0.
/// - If \p a equivalent to \p b , return 0.
///
/// \param compare Compares two elements. \sa CompareFunc.
///
/// \return Value equivalent to \p value , or `NULL` if value was
/// not found.
void *slist_find(SList slist, void *value, CompareFunc compare);

/// Return the value at \p pos in \p slist .
///
/// If \p pos < 0, get value of first element.
///
/// If \p pos > size of \p list , get value of last element.
///
/// \return Value at \p pos , or NULL if error.
void *slist_get_at(SList slist, int pos);

/// Return the value of \p node .
///
/// If \p node is `NULL`, it causes to undefined behaviour.
void *slist_node_value(SList slist, SListNode node);

/// \defgroup traversal Traversal functions
///@{

/// Return first node, or `SLIST_BOF` if \p slist is empty.
SListNode slist_first(SList slist);

/// Return last node, or `SLIST_EOF` if \p slist is empty.
SListNode slist_last(SList slist);

/// Return the next node of \p node , or `SLIST_EOF` if \p node is the last node
/// in \p slist .
SListNode slist_next(SList slist, SListNode node);

///@} // End of traversal.

#endif