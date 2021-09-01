/// \file test_companion.h
///
/// Various functions used when testing the modules.
///
///
///

#include <stdlib.h> // malloc, sizeof

/// @brief Compares two int*.
///
/// @return < 0, if a < b, or, > 0, if b < a, or, 0, if a and b are equivalent
///
int compare_ints(const void *a, const void *b) { return *(int *)a - *(int *)b; }

/// @brief Allocates memory for an integer with \p value.
///
/// @return Newly created pointer to integer, or NULL if an error occuredI.
///
int *create_int(int value) {
  int *pointer = malloc(sizeof(*pointer));
  if (pointer == NULL)
    return NULL;

  *pointer = value;

  return pointer;
}

/// @brief Shuffles the values of an array.
///
void shuffle(int *array[], size_t size) {
  for (int i = 0; i < size; i++) {
    int j = i + rand() / (RAND_MAX / (size - i) + 1);
    int *t = array[j];
    array[j] = array[i];
    array[i] = t;
  }
}

/// @brief Creates and returns an array containing (int*)
///
/// @param multiplier Multiplies the values of the array. (i * multiplier)
///
int **create_array(size_t size, int multiplier) {
  int **array = malloc(size * sizeof(*array));
  if (array == NULL)
    return NULL;

  for (int i = 0; i < size; i++)
    array[i] = create_int(multiplier * i);

  return array;
}