#pragma once

/// Compare elements \p a and \p b .
///
/// If \p a < \p b , return number < 0.
///
/// If \p a > \p b , return number > 0.
///
/// If \p a equivalent to \p b , return 0.
///
/// @return Number < 0, if a < b. Number > 0, if a > b. 0, if a and b are equivalent.
typedef int (*CompareFunc)(const void* a, const void* b);


/// Destroys a value.
typedef void (*DestroyFunc)(void* value);