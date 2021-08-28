/// \file map.h
///
/// Map Abstract Data Type.
///
/// Implementation independent.
///
/// The user does not need to know how a Map is implemented, they use
/// the API functions provided `map_<operation>` with the appropriate
/// parameters.

#ifndef MAP_H
#define MAP_H

#include "common_types.h" // CompareFunc, DestroyFunc
#include <stdbool.h>      // bool
#include <stddef.h>       // size_t

/// Map type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a Map is implemented, they use the API
/// functions provided `map_<operation>` with the appropriate parameters.
typedef struct map *Map;

/// Allocate space for a new map.
///
/// Elements are compared based on \p compare .
/// \p compare should work as follows when comparing two elements (*a* and *b*):
/// - If *a* < *b*, return number < 0.
/// - If *a* > *b*, return number > 0.
/// - If *a* equivalent to *b*, return 0.
///
/// If \p destroy_key is not NULL, then when an element gets removed,
/// `destroy_key(key)` is called to deallocate the space held by key.
///
/// If \p destroy_value is not NULL, then when an element gets removed,
/// `destroy_value(value)` is called to deallocate the space held by value.
///
/// Typical usage:
/// \code {.c}
///   int compare_ints(const void* a, const void* b){
///     return *(int*)a - *(int*)b;
///   }
///
///   Map map1 = map_create(compare_ints, NULL, NULL);
///   Map map2 = map_create(compare_ints, free, free);
/// \endcode
///
/// \param compare Compares two elements. \sa CompareFunc. Can NOT be NULL.
/// \param destroy_key When an element gets removed, `destroy_key(key)` is
/// called, if not NULL, to deallocate the space held by key.
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Newly created map, or NULL if an error occured.
Map map_create(CompareFunc compare, DestroyFunc destroy_key,
               DestroyFunc destroy_value);

/// Deallocate the space held by \p map .
///
/// Any operation on \p map after its destruction, causes undefined behaviour.
void map_destroy(Map map);

/// Changes the destroy function called on each key's removal/overwrite
/// to \p destroy_key .
///
/// Typical usage:
/// \code {.c}
///   int compare_ints(const void* a, const void* b){
///     return *(int*)a - *(int*)b;
///   }
///
///   Map mymap = map_create(compare_ints, free, NULL);
///   DestroyFunc old = map_set_destroy_key(mymap, NULL);
///   // old == free
/// \endcode
///
/// \param destroy_key When an element gets removed, `destroy_key(key)` is
/// called, if not NULL, to deallocate the space held by key.
///
/// \return Previous `destroy_key` function.
DestroyFunc map_set_destroy_key(Map map, DestroyFunc destroy_key);

/// Changes the destroy function called on each value's removal/overwrite
/// to \p destroy_value .
///
/// Typical usage:
/// \code {.c}
///   int compare_ints(const void* a, const void* b){
///     return *(int*)a - *(int*)b;
///   }
///
///   Map mymap = map_create(compare_ints, NULL, free);
///   DestroyFunc old = map_set_destroy_value(mymap, NULL);
///   // old == free
/// \endcode
///
/// \param destroy_value When an element gets removed, `destroy_value(value)` is
/// called, if not NULL, to deallocate the space held by value.
///
/// \return Previous `destroy_value` function.
DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value);

/// Returns the number of elements in \p map .
size_t map_size(Map map);

/// Inserts key with value to map. If an equivalent key is already part
/// of the map,
///        overwrite the old key and value with the new ones.
///
/// NOTE:
/// While the key is part of the map, any change to its content results in
/// undefined behaviour.
///

/// Associate \p key with \p value .
///
/// Insert \p key if not already present.
///
/// Duplicate keys are overwritten.
///
/// \p key can not be `NULL`.
///
/// While a key is part of the \p map , any change to its content causes
/// undefined behaviour.
void map_insert(Map map, void *key, void *value);

/// Remove \p key from \p map .
///
/// \p key can not be `NULL`.
///
/// \return true, if \p key was removed successfully, otherwise false.
bool map_remove(Map map, void *key);

/// Find and return the value associated with \p key .
///
/// \p key can not be `NULL`.
///
/// \note
/// `NULL` is returned:
///  - if \p key is not part of \p map .
///  - if \p key is associated with the value `NULL`.
/// To differentiate the two circumstances use map_find_node().
///
/// \return Value associated with \p key , or NULL if \p key is not part of
/// \p map .
void *map_find(Map map, void *key);

#define MAP_EOF (MapNode)0 ///< Defines the "virtual" end of the map.

/// MapNode type.
///
/// Incomplete struct, to keep it implementation independent.
///
/// The user does not need to know how a MapNode is implemented, they use the
/// API functions provided `map_<operation>_node()` and `map_node_<operation>()`
/// with the appropriate parameters.
typedef struct map_node *MapNode;

/// Find and return the first node with key equivalent to \p key .
///
/// \p key can not be `NULL`.
///
/// \return Node with key equivalent to \p key , or `MAP_EOF` if \p key is not
/// part of \p map .
MapNode map_find_node(Map map, void *key);

/// Return the value of \p node .
///
/// If \p node is `NULL`, it causes to undefined behaviour.
void *map_node_value(Map map, MapNode node);

/// Return the key of \p node .
///
/// If \p node is `NULL`, it causes to undefined behaviour.
void *map_node_key(Map map, MapNode node);

/// \defgroup traversal Traversal functions
/// \note Map traversal is arbitrary.
///@{

/// Return the first node, or `MAP_EOF` if \p map is empty.
MapNode map_first(Map);

/// Return the next node of \p node , or `MAP_EOF` if if there is no succeeding
/// node.
MapNode map_next(Map, MapNode);

///@}  // End of traversal

/// \defgroup hashing Hash functions
///@{

/// Hash an element.
///
/// \return Hash of element.
typedef unsigned int (*HashFunc)(void *);

/// Set the hash function of \p map to \p hash_func .
///
/// \warning Call map_set_hash_function() **right after** map_create() and
/// before any other map_<operation> function. Failing to do so causes undefined
/// behaviour.
///
void map_set_hash_function(Map map, HashFunc hash_func);

/// Hash \p value of type char*.
unsigned int hash_string(void *value);

/// Hash \p value of type int.
unsigned int hash_int(void *value);

///@} // End of hashing

#endif // MAP_H
