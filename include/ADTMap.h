////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file ADTMap.h
/// @author Constantinos Georgiou
/// @brief Interface for Map Abastract Data Type (ADT).
///        Operations included:
///          insertion, removal, look up, arbitrary traversal.
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

#include <stdbool.h>

typedef struct map* Map;

/// @brief Creates and returns a new map, in which the elements are compared based on given
///        compare function.
///
/// @param compare Used to compare the elements.
/// @param destroy_key If destroy_key != NULL, then each time a key is removed,
///                    destroy_key(key) is called.
/// @param destroy_value If destroy_value != NULL, then each time a value is removed,
///                      destroy_value(value) is called.
///
/// @return Newly created map.
///
Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value);

/// @brief Frees all the memory allocated by given map.
///
/// Any operation on the map after its destruction, results in undefined behaviour.
///
void map_destroy(Map);

/// @brief Changes the destroy function called on each key's removal/overwrite to given destroy
///        function.
///
/// @return Previous destroy function.
///
DestroyFunc map_set_destroy_key(Map map, DestroyFunc destroy_key);

/// @brief Changes the destroy function called on each value's removal/overwrite to given destroy
///        function.
///
/// @return Previous destroy function.
///
DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value);

/// @brief Returns the number of elements in given map.
///
int map_size(Map map);

/// @brief Inserts key with value to map. If an equivalent key is already part of the map,
///        overwrite the old key and value with the new ones.
///
/// NOTE:
/// While the key is part of the map, any change to its content results in undefined behaviour.
///
void map_insert(Map map, void* key, void* value);

/// @brief Removes the key equivalent to given key from the map, if it exists.
///
/// @return true if key was found and removed, or false, if key was not found.
///
bool map_remove(Map map, void* key);

/// @brief Finds the value of given key.
///
/// NOTE:
/// NULL is returned either when given key does not exist, or when given key exists and its value
/// is NULL. If there's a need to differentiate the two circumstances use map_find_node.
///
/// @return Value of given key, or NULL if key does not exist.
///
void* map_find(Map map, void* key);

#define MAP_EOF (MapNode)0  // Defines the "virtual" end of the map.

typedef struct map_node* MapNode;

/// @brief Finds node with key equivalent to given key.
///
/// @return Node with key equivalent to given key, or MAP_EOF if key is not part of given map.
///
MapNode map_find_node(Map map, void* key);

/// @brief Returns value of given node.
///
/// @return Value of given node.
///
void* map_node_value(Map map, MapNode node);

/// @brief Returns key of given node.
///
/// @return Key of given node.
///
void* map_node_key(Map map, MapNode node);

////////////    TRAVERSAL    ///////////////////////////////////////////////////////////////////////

/// NOTE: Map traversal is arbitrary.

/// @brief Returns the first node, or MAP_EOF if map is empty.
///
MapNode map_first(Map);

/// @brief Returns the next node of given node, or MAP_EOF if there is no succeeding node.
///
MapNode map_next(Map, MapNode);

////////////    END OF TRAVERSAL    ////////////////////////////////////////////////////////////////

////////////    HASHING    /////////////////////////////////////////////////////////////////////////

/// @brief Pointer to function that hashes an element…
///
/// @return Hash of element.
///
typedef unsigned int (*HashFunc)(void*);

/// @brief Hashes values of type char*.
///
unsigned int hash_string(void* value);

/// @brief Hashes values of type int.
///
unsigned int hash_int(void* value);

/// @brief Sets the hash function of given map.
///
/// NOTE:
/// Needs to be called right AFTER map_create and before any other map function. Failing to
/// do so results in undefined behaviour.
///
void map_set_hash_function(Map map, HashFunc hash_func);

////////////    END OF HASHING    //////////////////////////////////////////////////////////////////
