#pragma once

/// @brief Pointer to function that compares elements a and b.
///
/// @return < 0, if a < b, or, > 0, if b < a, or, 0, if a and b are equivalent
///
typedef int (*CompareFunc)(const void* a, const void* b);


/// @brief Pointer to function that destroys a value.
///
///
typedef void (*DestroyFunc)(void* value);