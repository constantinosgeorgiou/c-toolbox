////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file ADTList.h
/// @author Constantinos Georgiou
/// @brief Interface for Bidirectional List Abastract Data Type (ADT).
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

#define BLIST_BOF (BListNode)0  // Defines the "virtual" beginning of the bidirectional list.
#define BLIST_EOF (BListNode)0  // Defines the "virtual" end of the bidirectional list.

typedef struct blist* BList;
typedef struct blist_node* BListNode;

/// @brief Creates and returns a new bidirectional list.
///
/// @param destroy If destroy != NULL, then each time an item is removed,
///                      destroy(value) is called.
///
/// @return Newly created bidirectional list.
///
BList blist_create(DestroyFunc destroy);

/// @brief Frees all the allocated memory of the given bidirectional list.
///
/// Any operation on the bidirectional list after its destruction, results in undefined behaviour.
///
void blist_destroy(BList);

/// @brief Changes the function called on each element removal/overwrite to given destroy.
///
/// @param destroy Defines a destroy function.
///
/// @return Previous destroy function.
///
DestroyFunc blist_set_destroy_value(BList blist, DestroyFunc destroy);

/// @brief Returns the number of elements in given bidirectional list.
///
int blist_size(BList);

/// @brief Inserts a new node with given value __before__ given node, or at the end
///        if node == BLIST_BOF.
///
/// @param blist Defines a bidirectional list.
/// @param node Defines a bidirectional list node.
/// @param value Defines a value to be inserted.
///
/// @return Pointer to newly inserted node, or NULL if an error occurred.
///
BListNode blist_insert(BList blist, BListNode node, void* value);

// Αφαιρεί τον κόμβο node (πρέπει να υπάρχει στη λίστα).

/// @brief Removes the given node, or last node if node == BLIST_EOF.
///
/// @param blist Defines a list.
/// @param node Defines a list node.
///
/// @return Pointer to the beginning of the bidirectional list, or NULL if an error occurred.
///
void blist_remove(BList blist, BListNode node);

/// @brief Appends to_append bidirectional list, to given bidirectional list. After operation
///        to_append bidirectional list can NOT be used.
///        list == to_append causes undefined behaviour.
///
/// @param blist Defines a bidirectional list.
/// @param to_append Defines a bidirectional list to be appended.
///
/// @return Pointer to the beginning of the appened bidirectional list, or NULL if an error occured.
///
BListNode blist_append(BList blist, BList to_append);

/// @brief Finds and returns the first bidirectional list node with value equivalent to value,
///        (based on compare function).
///
/// @param blist Defines a bidirectional list.
/// @param value Defines desired value.
/// @param compare Defines a compare function.
///
/// @return The bidirectional list node of given value, or BLIST_EOF if value was not found.
///
BListNode blist_find_node(BList blist, void* value, CompareFunc compare);

// Επιστρέφει την πρώτη τιμή που είναι ισοδύναμη με value
// (με βάση τη συνάρτηση compare), ή NULL αν δεν υπάρχει

/// @brief Finds and returns the first value equivalent to given value, (based on compare function).
///
/// @param blist Defines a bidirectional list.
/// @param value Defines a desired value.
/// @param compare Defines a compare function.
///
/// @return The desired value, or NULL if value was not found.
///
void* blist_find(BList blist, void* value, CompareFunc compare);

/// @brief Returns the value of given list node. If node == NULL, results to undefined behaviour.
///
/// @return Content of given node.
///
void* blist_node_value(BList blist, BListNode node);

////////////    TRAVERSAL    ///////////////////////////////////////////////////////////////////////

/// @brief Returns first bidirectional list node, or BLIST_BOF if bidirectional list is empty.
///
BListNode blist_first(BList blist);

/// @brief Returns last bidirectional list node, or BLIST_EOF if bidirectional list is empty.
///
BListNode blist_last(BList blist);

/// @brief Returns the next node of given node, or BLIST_EOF if given node is the last node.
///
BListNode blist_next(BList blist, BListNode node);

/// @brief Returns the previous node of given node, or BLIST_BOF if given node is the first node.
///
BListNode blist_previous(BList blist, BListNode node);

/// @brief Returns the value of given position in bidirectional list.
///        (position < 0 or position >= size) results in undefined behaviour.
///
/// @param position Defines the position in the bidirectional list.
///
/// @return Value of given position, or NULL if error.
///
void* blist_get_at(BList, int position);

////////////    END OF TRAVERSAL    ////////////////////////////////////////////////////////////////
