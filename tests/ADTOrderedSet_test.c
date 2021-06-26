#include "ADTOrderedSet.h"

#include "acutest.h"

/// @brief Compares two int*.
///
/// @return < 0, if a < b, or, > 0, if b < a, or, 0, if a and b are equivalent
///
static int compare_ints(const void* a, const void* b) { return *(int*)a - *(int*)b; }

void test_create(void) {
    OrderedSet oset = oset_create(compare_ints, free, free);

    TEST_CHECK(oset != OSET_ERROR);
    TEST_CHECK(oset_size(oset) == 0);

    DestroyFunc destroy_key = oset_set_destroy_key(oset, NULL);
    DestroyFunc destroy_value = oset_set_destroy_value(oset, NULL);

    TEST_CHECK(destroy_key == free);
    TEST_CHECK(destroy_value == free);

    // TODO: Create oset with vector containing values.

    oset_destroy(oset);
}

TEST_LIST = {
    {"oset_create", test_create},
    {NULL, NULL}  // End of tests
}