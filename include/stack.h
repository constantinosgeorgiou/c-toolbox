/// \file stack.h
///
/// Stack Abstract Data Type.
///
/// Implementation independent.
///
/// The user does not need to know how a Stack is implemented, they use the API
/// functions provided `stack_<operation>` with the appropriate parameters.

#ifndef STACK_H
#define STACK_H

#include "common_types.h" // DestroyFunc
#include <stdbool.h>      // bool
#include <stddef.h>       // size_t

/// Stack type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a Stack is implemented, they use the API
/// functions provided `stack_<operation>` with the appropriate parameters.
typedef struct stack *Stack;

/// Allocate space for a new \p stack.
///
/// If \p destroy_value is not NULL, then when an element gets removed,
/// `destroy_value(value)` is called to deallocate the space held by value.
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Newly created stack, or NULL if an error occured.
Stack stack_create(DestroyFunc destroy_value);

/// Deallocate the space held by \p stack.
///
/// Any operation on the \p stack after its destruction, results in
/// undefined behaviour.
void stack_destroy(Stack stack);

/// Change the function called on each element's removal to
/// \p destroy_value .
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Previous `destroy_value` function.
DestroyFunc stack_set_destroy_value(Stack stack, DestroyFunc destroy_value);

/// Return the number of elements in \p stack.
size_t stack_size(Stack stack);

/// Return `true` if \p stack is empty, else, `false`.
bool stack_is_empty(Stack stack);

/// Return the first element of \p stack without removing it.
///
/// \return Highest-priority element of \p stack, or NULL, if \p stack is
/// empty.
void *stack_peek(Stack stack);

/// Add \p value to \p stack.
void stack_push(Stack stack, void *value);

/// Remove the most recently added element that was not yet removed.
///
/// Calling `stack_pop()` on an empty stack, does nothing.
void stack_pop(Stack stack);

#endif // STACK_H