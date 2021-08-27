/// \file list.h
///
/// List Abstract Data Type.
///
/// Implementation independent.
///
/// The user does not need to know how a List is implemented, they use
/// the API functions provided `list_<operation>` with the appropriate
/// parameters.

#ifndef LIST_H
#define LIST_H

#include "common_types.h" // CompareFunc, DestroyFunc
#include <stdlib.h>       // size_t

/// List type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a List is implemented, they use the API
/// functions provided `list_<operation>` with the appropriate parameters.
typedef struct list *List;

/// ListNode type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a ListNode is implemented, they use the
/// API functions provided `list_<operation>_node` and `list_node_<operation>`
/// with the appropriate parameters.
typedef struct list_node *ListNode;

#define LIST_BOF (ListNode)0 ///< Defines the "virtual" beginning of a list.
#define LIST_EOF (ListNode)0 ///< Defines the "virtual" end of a list.

/// Allocate space for a new list.
///
/// If \p destroy_value is not NULL, then when an element gets removed,
/// `destroy_value(value)` is called to deallocate the space held by value.
///
/// Typical usage:
/// \code {.c}
///   List list1 = list_create(NULL);
///   List list2 = list_create(free);
/// \endcode
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Newly created list, or NULL if an error occured.
List list_create(DestroyFunc destroy_value);

/// Deallocate the space held by \p list .
///
/// Any operation on the \p list after its destruction, results in
/// undefined behaviour.
///
/// Typical usage:
/// \code {.c}
///   list_destroy(mylist);
/// \endcode
void list_destroy(List list);

/// Change the function called on each element's removal to
/// \p destroy_value .
///
/// Typical usage:
/// \code {.c}
///   List mylist = list_create(free);
///   DestroyFunc old = list_set_destroy_value(mylist, NULL);
///   // old == free
/// \endcode
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Previous `destroy_value` function.
DestroyFunc list_set_destroy_value(List list, DestroyFunc destroy_value);

/// Return the number of elements in \p list .
size_t list_size(List list);

/// Insert a new node with \p value *after* \p node .
///
/// If \p node is `LIST_BOF`, insert new node at the beginning of \p list .
///
/// If \p node is `LIST_EOF`, it causes undefined behaviour.
///
/// Typical usage:
/// \code {.c}
///   List numbers = list_create(NULL);
///   // numbers: []
///
///   int number1 = 1234;
///   list_insert(numbers, LIST_BOF, &number1);
///   // numbers: [ 1234 ]
///
///
///   ListNode node = list_first(numbers);
///   int number2 = 56;
///   list_insert(numbers, node, &number2);
///   // numbers: [ 1234, 56 ]
///
///   int number3 = 0;
///   list_insert(numbers, LIST_BOF, &number3);
///   // numbers: [ 0, 1234, 56 ]
///
///
///   list_destroy(numbers);
/// \endcode
void list_insert(List list, ListNode node, void *value);

/// Remove \p node from \p list .
///
/// If \p node is `LIST_EOF`, remove last node.
///
/// If \p node is `LIST_BOF`, it causes undefined behaviour.
///
/// \code {.c}
///   List numbers = list_create(NULL);
///
///   ...
///
///   // numbers: [ 0, 5, 4, 10 ]
///
///   list_remove(numbers, LIST_EOF);
///   // numbers: [ 0, 5, 4]
///
///   list_remove(numbers, list_first(numbers));
///   // numbers: [ 5, 4]
///
/// \endcode
void list_remove(List list, ListNode node);

/// Append \p b to the end of \p a .
///
/// After operation any operation on \p b causes undefined behaviour.
///
/// If \p a is \p b , it causes undefined behaviour.
///
/// \code {.c}
///   List veggies;
///   List fruits;
///
///   ...
///
///   // veggies: [ tomato, lettuce ]
///   // fruits: [ apple, mango, banana ]
///
///   list_concat(fruits, veggies);
///   // fruits: [ apple, mango, banana, tomato, lettuce ]
///
/// \endcode
void list_concat(List a, List b);

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
/// \return Node with value equivalent to \p value , or `LIST_EOF` if value was
/// not found.
ListNode list_find_node(List list, void *value, CompareFunc compare);

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
void *list_find(List list, void *value, CompareFunc compare);

/// Return the value at \p pos in \p list .
///
/// If \p pos < 0, get value of first element.
///
/// If \p pos > size of \p list , get value of last element.
///
/// \return Value at \p pos , or NULL if error.
void *list_get_at(List list, int pos);

/// Return the value of \p node .
///
/// If \p node is `NULL`, it causes to undefined behaviour.
void *list_node_value(List list, ListNode node);

/// \defgroup traversal Traversal functions
///@{

/// Return first node, or `LIST_BOF` if \p list is empty.
ListNode list_first(List list);

/// Return last node, or `LIST_EOF` if \p list is empty.
ListNode list_last(List list);

/// Return the next node of \p node , or `LIST_EOF` if \p node is the last node
/// in \p list .
ListNode list_next(List list, ListNode node);

/// Return the previous node of \p node , or `LIST_BOF` if \p node is the first
/// node in \p list .
ListNode list_previous(List list, ListNode node);

///@} // End of traversal.

#endif // LIST_H