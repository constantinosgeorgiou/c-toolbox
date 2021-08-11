/// @file stack.h
///
/// Stack Abstract Data Type.
///
/// Implementation indepenent.
///
/// The user does not need to know how a Stack is implemented, they use the API
/// functions provided `stack_<operation>` with the appropriate parameters.
///
/// Operations supported:
/// - stack_create():             Allocates memory for a stack.
/// - stack_destroy():            Frees the allocated memory of a stack.
/// - stack_set_destroy_value():  Changes the function called on each element
///                                 removal/overwrite, to given function.
/// - stack_size():               Returns the number of elements in the stack.
/// - stack_is_empty():           Returns true if the stack is empty, or false
///                                 if it is not.
/// - stack_peek():               Returns the first element without removing
///                                 it.
/// - stack_push():               Adds an elements to the stack.
/// - stack_pop():                Removes the most recently added element that
///                                 was not yet removed.

#pragma once

#include <stdbool.h>  // bool
#include <stdlib.h>   // size_t

#include "common_types.h"  // DestroyFunc

/// Stack type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a Stack is implemented, they use the API
/// functions provided `stack_<operation>` with the appropriate parameters.
///
typedef struct stack* Stack;

/// Creates and returns a stack.
///
/// @param destroy_value If destroy_value != NULL, call `destroy_value(value)`
///                      each time a value is removed.
/// @return Newly created stack.
///
Stack stack_create(DestroyFunc destroy_value);

/// Frees all the memory allocated for STACK.
///
/// Any operation on STACK after its destruction, results in undefined
/// behaviour.
///
void stack_destroy(Stack stack);

/// @brief Changes function called each time a value is removed to
///        DESTROY_VALUE.
///
/// @return Previous destroy_value function.
///
DestroyFunc stack_set_destroy_value(Stack stack, DestroyFunc destroy_value);

/// @brief Returns the number of elements in STACK.
///
/// @return The number of elements in STACK.
///
size_t stack_size(Stack stack);

/// Returns `true` if STACK is empty, else, `false`.
///
bool stack_is_empty(Stack stack);

/// Returns the first element of STACK without removing it.
///
/// @return First element of STACK, or NULL, if STACK is empty.
///
void* stack_peek(Stack stack);

/// Adds VALUE to STACK.
///
void stack_push(Stack stack, void* value);

/// Removes the most recently added element that was not yet removed.
///
/// Calling `stack_pop()` on an empty stack, does nothing.
///
void stack_pop(Stack stack);