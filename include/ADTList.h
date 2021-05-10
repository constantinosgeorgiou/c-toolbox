////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file ADTList.h
/// @author Constantinos Georgiou
/// @brief Interface for List Abastract Data Type (ADT).
///        Operations included:
///          insertion, removal, look up, serial traversal, accessing at given index, appending.
/// @version 1.0
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

/// @brief Shorthand for types of comparison and destroy functions.
///
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

#define LIST_BOF (ListNode)0  // Defines the "virtual" beginning of the list.
#define LIST_EOF (ListNode)0  // Defines the "virtual" end of the list.

typedef struct list* List;
typedef struct list_node* ListNode;

/// @brief Creates and returns a new list.
///
/// @param destroy If destroy != NULL, then each time an item is removed,
///                destroy(value) is called.
///
/// @return Newly created list.
///
List list_create(DestroyFunc destroy);

/// @brief Frees all the allocated memory of the given list.
///
/// Any operation on the list after its destruction, results in undefined behaviour.
///
void list_destroy(List);

/// @brief Changes the function called on each element removal/overwrite to given destroy.
///
/// @param destroy Defines a destroy function.
///
/// @return Previous destroy function.
///
DestroyFunc list_set_destroy_value(List list, DestroyFunc destroy);

/// @brief Returns the number of elements in given list.
///
int list_size(List);

/// @brief Inserts a new node with given value after given node, or at the beginning
///        if node == LIST_BOF. (node == LIST_EOF) causes undefined behaviour.
///
/// @param list Defines a list.
/// @param node Defines a list node.
/// @param value Defines a value to be inserted.
///
/// @return Pointer to newly inserted node, or NULL if an error occurred.
///
ListNode list_insert_next(List list, ListNode node, void* value);

/// @brief Removes the next node after given node, or first node if node == LIST_BOF.
///
/// @param list Defines a list.
/// @param node Defines a list node.
///
/// @return Pointer to given node, or NULL if an error occurred.
///
ListNode list_remove_next(List list, ListNode node);

/// @brief Concatenates list b, to list a. After operation list b can NOT be used.
///        (a == b) causes undefined behaviour.
///
/// @return Pointer to the beginning of the concatenated list.
///
ListNode list_concatenate(List list, List to_append);

/// @brief Finds and returns the first list node with value equivalent to value, (based on compare
///        function).
///
/// @param list Defines a list.
/// @param value Defines desired value.
/// @param compare Defines a compare function.
///
/// @return The list node of given value, or LIST_EOF if value was not found.
///
ListNode list_find_node(List list, void* value, CompareFunc compare);

/// @brief Finds and returns the first value equivalent to given value, (based on compare function).
///
/// @param list Defines a list.
/// @param value Defines a desired value.
/// @param compare Defines a compare function.
///
/// @return The desired value, or NULL if value was not found.
///
void* list_find(List list, void* value, CompareFunc compare);

/// @brief Returns the value of given list node.
///
/// @return Content of given node.
///
void* list_node_value(List, ListNode);

////////////    TRAVERSAL    ///////////////////////////////////////////////////////////////////////

/// @brief Returns first list node, or LIST_BOF if list is empty.
///
ListNode list_first(List);

/// @brief Returns last list node, or LIST_EOF if list is empty.
///
ListNode list_last(List);

/// @brief Returns the next node of given node, or LIST_EOF if given node is the last node.
///
ListNode list_next(List, ListNode);

/// @brief Returns the value of given position in list.
///        (position < 0 or position >= size) results in undefined behaviour.
///
/// @param position Defines the position in the list.
///
/// @return Value of given position, or NULL if error.
///
void* list_get_at(List, int position);

////////////    END OF TRAVERSAL    ////////////////////////////////////////////////////////////////
