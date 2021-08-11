/// @file stack.h
///
/// Implementation of Stack Abstract Data Type.

#include "stack.h"

#include <stdbool.h>  // bool, true, false
#include <stdlib.h>   // size_t

#include "ADTList.h"  // List

struct stack {
    List data;  ///< Holds the data of the stack.
};

Stack stack_create(DestroyFunc destroy_value) {
    Stack stack = malloc(sizeof(*stack));
    if (stack == NULL) return NULL;

    stack->data = list_create(destroy_value);
    if (stack->data == NULL) return NULL;

    return stack;
}

void stack_destroy(Stack stack) {
    list_destroy(stack->data);
    free(stack);
}

DestroyFunc stack_set_destroy_value(Stack stack, DestroyFunc destroy_value) {
    return list_set_destroy_value(stack->data, destroy_value);
}
