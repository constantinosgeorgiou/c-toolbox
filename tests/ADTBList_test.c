//////////////////////////////////////////////////////////////////
//
// Unit tests για τον ADT BList.
// Οποιαδήποτε υλοποίηση οφείλει να περνάει όλα τα tests.
//
//////////////////////////////////////////////////////////////////

#include "ADTBList.h"

#include <stdio.h>

#include "acutest.h"  // Απλή βιβλιοθήκη για unit testing

// Επιστρέφει έναν ακέραιο σε νέα μνήμη με τιμή value
int* create_int(int value) {
    int* p = malloc(sizeof(int));
    *p = value;
    return p;
}

void test_create(void) {
    // Δημιουργούμε μια κενή λίστα με NULL δείκτη συνάρτησης delete_value
    BList blist = blist_create(NULL);

    // Ελέγχουμε ότι δεν απέτυχε η malloc στην λίστα, και ότι
    // αρχικοποιείται με μέγεθος 0 (δηλαδή χωρίς κόμβους)
    TEST_CHECK(blist != NULL);
    TEST_CHECK(blist_size(blist) == 0);

    blist_destroy(blist);
}

void test_insert(void) {
    BList blist = blist_create(free);

    int N = 5;

    for (int i = 0; i < N; i++) {
        // εισαγωγή στο τέλος
        blist_insert(blist, BLIST_EOF, create_int(i));

        // Ελέγχουμε εάν ενημερώθηκε (αυξήθηκε) το μέγεθος της λίστας.
        TEST_CHECK(blist_size(blist) == (i + 1));

        // Ελέγχουμε εάν ο τελευταίος κόμβος περιέχει την τιμή που μόλις κάναμε insert
        TEST_CHECK(*(int*)blist_node_value(blist, blist_last(blist)) == i);
    }
    blist_destroy(blist);

    blist = blist_create(free);
    for (int i = 0; i < N; i++) {
        // εισαγωγή στο ένα κόμβο πριν το τέλος
        blist_insert(blist, blist_last(blist), create_int(i));

        // Ελέγχουμε εάν ενημερώθηκε (αυξήθηκε) το μέγεθος της λίστας.
        TEST_CHECK(blist_size(blist) == (i + 1));

        // Ελέγχουμε εάν ο τελευταίος κόμβος περιέχει την τιμή που μόλις κάναμε insert
        TEST_CHECK(*(int*)blist_node_value(blist, blist_previous(blist, blist_last(blist))) == i);
    }
    blist_destroy(blist);

    blist = blist_create(free);
    for (int i = 0; i < N; i++) {
        // εισαγωγή στην αρχή
        blist_insert(blist, blist_first(blist), create_int(i));

        // Ελέγχουμε εάν ενημερώθηκε (αυξήθηκε) το μέγεθος της λίστας.
        TEST_CHECK(blist_size(blist) == (i + 1));

        // Ελέγχουμε εάν ο πρώτος κόμβος περιέχει την τιμή που μόλις κάναμε insert
        TEST_CHECK(*(int*)blist_node_value(blist, blist_first(blist)) == i);
    }
    blist_destroy(blist);
}

void test_remove(void) {
    BList blist = blist_create(free);

    int N = 5;

    for (int i = 0; i < N; i++) {
        blist_insert(blist, BLIST_EOF, create_int(i));
    }
    for (int i = 0; i < N; i++) {
        TEST_CHECK(*(int*)blist_node_value(blist, blist_first(blist)) == i);
        blist_remove(blist, blist_first(blist));
    }
    TEST_CHECK(blist_size(blist) == 0);

    for (int i = 0; i < N; i++) {
        blist_insert(blist, BLIST_EOF, create_int(i));
    }
    for (int i = N - 1; i >= 0; i--) {
        TEST_CHECK(*(int*)blist_node_value(blist, blist_last(blist)) == i);
        blist_remove(blist, BLIST_EOF);
    }
    for (int i = 0; i < N; i++) {
        blist_insert(blist, blist_first(blist), create_int(i));
    }
    for (int i = N - 1; i >= 0; i--) {
        TEST_CHECK(*(int*)blist_node_value(blist, blist_first(blist)) == i);
        blist_remove(blist, blist_first(blist));
    }
    for (int i = 0; i < N; i++) {
        blist_insert(blist, blist_first(blist), create_int(i));
    }
    for (int i = 0; i < N; i++) {
        TEST_CHECK(*(int*)blist_node_value(blist, blist_last(blist)) == i);
        blist_remove(blist, BLIST_EOF);
    }

    // Αφαιρούμε από την μέση
    // ----------------------
    for (int i = 0; i < N; i++) {
        // εισαγωγή στο τέλος
        blist_insert(blist, BLIST_EOF, create_int(i));
    }

    BListNode middle = blist_first(blist);
    for (int i = 0; i < N / 2; i++) {
        middle = blist_next(blist, middle);
    }
    blist_remove(blist, middle);
    TEST_CHECK(blist_size(blist) == N - 1);

    blist_destroy(blist);
}

// Σύγκριση δύο int void*s
int compare_ints(const void* a, const void* b) { return *(int*)a - *(int*)b; }

void test_find() {
    BList blist = blist_create(NULL);
    int N = 1000;
    int array[N];

    // Εισάγουμε δοκιμαστικές τιμές στον πίνακα, για να ελέγξουμε την test_find
    for (int i = 0; i < N; i++) {
        array[i] = i;
        blist_insert(blist, blist_first(blist), &array[i]);
    }

    // Εύρεση όλων των στοιχείων
    for (int i = 0; i < N; i++) {
        int* value = blist_find(blist, &i, compare_ints);
        TEST_CHECK(value == &array[i]);
    }

    // Δοκιμάζουμε, για μια τυχαία τιμή που δεν μπορεί πιθανώς να υπάρχει στην λίστα,
    // αν η list_find γυρνάει σωστά NULL void*
    int not_exists = -1;
    TEST_CHECK(blist_find(blist, &not_exists, compare_ints) == NULL);

    blist_destroy(blist);
}

void test_find_node() {
    BList blist = blist_create(NULL);

    // Εισαγωγή τιμών στον πίνακα
    int N = 1000;
    int array[N];

    for (int i = 0; i < N; i++) {
        array[i] = i;
        blist_insert(blist, blist_first(blist), &array[i]);
    }

    // Ξεκινάμε από την αρχή της λίστας
    BListNode node = blist_first(blist);

    for (int i = N - 1; i >= 0; i--) {
        // Ελέγχουμε ότι η list_find_node βρίσκει σωστά τον πρώτο κόμβο με value τον δείκτη &array[i].
        // Σε αυτή την λίστα, δοκιμάζουμε ότι ο πρώτος κόμβος περιέχει τον δείκτη &array[N - 1],
        // o δεύτερος τον &array[998] κοκ
        BListNode found_node = blist_find_node(blist, &i, compare_ints);
        TEST_CHECK(found_node == node);
        TEST_CHECK(blist_node_value(blist, found_node) == &array[i]);

        // Προχωράμε στον επόμενο κόμβο για να προσπελάσουμε όλη την λίστα
        node = blist_next(blist, node);
    }

    blist_destroy(blist);
}

// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
    {"create", test_create},
    {"insert", test_insert},
    {"remove", test_remove},
    {"find", test_find},
    {"find_node", test_find_node},
    {NULL, NULL}  // τερματίζουμε τη λίστα με NULL
};