////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file ADTList_test.c
/// @author Constantinos Georgiou
/// @brief Unit tests for implementation of ADT List.
/// @version 1.0
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ADTList.h"

#include "acutest.h"

/// @brief Allocates memory for an integer with given value.
///
/// @return Newly created pointer to integer.
///
static int* create_int(int value) {
    int* pointer = malloc(sizeof(int));
    if (pointer == NULL) {
        return NULL;
    }
    *pointer = value;
    return pointer;
}

void test_create(void) {
    List list = list_create(NULL);
    DestroyFunc destroy = list_set_destroy_value(list, NULL);

    TEST_CHECK(list != NULL);
    TEST_CHECK(list_size(list) == 0);
    TEST_CHECK(destroy == NULL);

    list_destroy(list);
}

void test_insert(void) {
    int N = 10;

    // Insert at the beginning.
    List list = list_create(free);
    for (int i = 0; i < N; i++) {
        ListNode inserted = list_insert_next(list, LIST_BOF, create_int(i));

        if (TEST_CHECK(inserted != NULL)) {
            ListNode first = list_first(list);
            int* value = list_node_value(list, inserted);
            TEST_CHECK(inserted == first);
            TEST_CHECK(*value == i);
            TEST_CHECK(list_size(list) == (i + 1));
        }
    }

    // Check if elements were inserted in descending order:
    ListNode node = list_first(list);
    for (int i = N - 1; i >= 0; i--) {
        int* value = list_node_value(list, node);
        TEST_CHECK(*value == i);
        node = list_next(list, node);
    }

    // Check last element:
    TEST_CHECK(*(int*)list_node_value(list, list_last(list)) == 0);

    // Destroy to test insertion at the end:
    list_destroy(list);

    // Insert at the end.
    list = list_create(free);
    for (int i = 0; i < N; i++) {
        ListNode inserted = list_insert_next(list, list_last(list), create_int(i));

        if (TEST_CHECK(inserted != NULL)) {
            ListNode last = list_last(list);
            int* value = list_node_value(list, inserted);

            TEST_CHECK(inserted == last);
            TEST_CHECK(*value == i);
            TEST_CHECK(list_size(list) == (i + 1));
        }
    }

    // Check if elements were inserted in ascending order:
    node = list_first(list);
    for (int i = 0; i < N; i++) {
        TEST_CHECK(*(int*)list_node_value(list, node) == i);
        node = list_next(list, node);
    }

    // Check last element:
    TEST_CHECK(*(int*)list_node_value(list, list_last(list)) == (N - 1));

    // Check in-between insertion by inserting a NULL node after the first node.
    ListNode first_node = list_first(list);
    list_insert_next(list, first_node, NULL);
    TEST_CHECK(list_node_value(list, list_next(list, first_node)) == NULL);

    list_destroy(list);
}

void test_remove_next(void) {
    // Δημιουργία λίστας που καλεί αυτόματα τη free σε κάθε στοιχείο που αφαιρείται
    List list = list_create(free);

    int N = 1000;
    int* array[N];

    // Χρησιμοποιούμε την insert για να γεμίσουμε την λίστα, αφού την έχουμε δοκιμάσει ήδη στην test_insert()
    for (int i = 0; i < N; i++) {
        // Δημιουργούμε δυναμικά δεσμευμένα αντικείμενα για να δοκιμάσουμε την destroy_function
        array[i] = malloc(sizeof(int));
        *array[i] = i;
        list_insert_next(list, LIST_BOF, array[i]);
    }

    for (int i = N - 1; i >= 0; i--) {
        // Διαγράφουμε απο την αρχή και ελέγχουμε εάν η τιμή του πρώτου κόμβου
        // ήταν η ίδια με αυτή που κάναμε insert παραπάνω
        TEST_CHECK(list_node_value(list, list_first(list)) == array[i]);
        list_remove_next(list, LIST_BOF);

        // Ελέγχουμε ότι ενημερώνεται (μειώνεται) το size/μέγεθος της λίστας
        TEST_CHECK(list_size(list) == i);
    }

    // Ξαναγεμίζουμε την λίστα για να δοκιμάσουμε την διαγραφή απο ενδιάμεσο κόμβο
    for (int i = 0; i < N; i++) {
        array[i] = malloc(sizeof(int));
        *array[i] = i;
        list_insert_next(list, LIST_BOF, array[i]);
    }

    // Δοκιμάζουμε την διαγραφή κόμβων ενδιάμεσα της λίστας, και συγκεκριμένα του δεύτερου κόμβου απο την αρχή
    list_remove_next(list, list_first(list));
    TEST_CHECK(list_size(list) == N - 1);

    list_destroy(list);
}

// Σύγκριση δύο int pointers
int compare_ints(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

void test_find() {
    List list = list_create(NULL);
    int N = 1000;
    int array[N];

    // Εισάγουμε δοκιμαστικές τιμές στον πίνακα, για να ελέγξουμε την test_find
    for (int i = 0; i < N; i++) {
        array[i] = i;
        list_insert_next(list, LIST_BOF, &array[i]);
    }

    // Εύρεση όλων των στοιχείων
    for (int i = 0; i < N; i++) {
        int* value = list_find(list, &i, compare_ints);
        TEST_CHECK(value == &array[i]);
    }

    // Δοκιμάζουμε, για μια τυχαία τιμή που δεν μπορεί πιθανώς να υπάρχει στην λίστα,
    // αν η list_find γυρνάει σωστά NULL pointer
    int not_exists = -1;
    TEST_CHECK(list_find(list, &not_exists, compare_ints) == NULL);

    list_destroy(list);
}

void test_find_node() {
    List list = list_create(NULL);

    // Εισαγωγή τιμών στον πίνακα
    int N = 1000;
    int array[N];

    for (int i = 0; i < N; i++) {
        array[i] = i;
        list_insert_next(list, LIST_BOF, &array[i]);
    }

    // Ξεκινάμε από την αρχή της λίστας
    ListNode node = list_first(list);

    for (int i = N - 1; i >= 0; i--) {
        // Ελέγχουμε ότι η list_find_node βρίσκει σωστά τον πρώτο κόμβο με value τον δείκτη &array[i].
        // Σε αυτή την λίστα, δοκιμάζουμε ότι ο πρώτος κόμβος περιέχει τον δείκτη &array[N - 1],
        // o δεύτερος τον &array[998] κοκ
        ListNode found_node = list_find_node(list, &i, compare_ints);
        TEST_CHECK(found_node == node);
        TEST_CHECK(list_node_value(list, found_node) == &array[i]);

        // Προχωράμε στον επόμενο κόμβο για να προσπελάσουμε όλη την λίστα
        node = list_next(list, node);
    }

    list_destroy(list);
}

void test_concatenate(void) {}
void test_traverse(void) {}
void test_get_at(void) {}

TEST_LIST = {
    {"create", test_create},
    {"insert", test_insert},
    {"remove", test_remove_next},
    {"find", test_find},
    {"find_node", test_find_node},
    {"concatenate", test_concatenate},
    {"traverse", test_traverse},
    {"get_at", test_get_at},
    {NULL, NULL}  // End of tests.
};