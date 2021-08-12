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

size_t stack_size(Stack stack) { return list_size(stack->data); }

bool stack_is_empty(Stack stack) {
    return list_size(stack->data) != 0 ? false : true;
}

void* stack_peek(Stack stack) {
    if (stack_is_empty(stack))
        return NULL;
    else
        return list_node_value(stack->data, list_first(stack->data));
}

void stack_push(Stack stack, void* value) {
    list_insert_next(stack->data, LIST_BOF, value);
}

void stack_pop(Stack stack) { list_remove_next(stack->data, LIST_BOF); }