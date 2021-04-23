///////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT BList μέσω διπλά συνδεδεμένης λίστας.
//
///////////////////////////////////////////////////////////

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ADTBList.h"

// Ενα BList είναι pointer σε αυτό το struct
struct blist {
    BListNode dummy;
    BListNode last;
    int size;
    DestroyFunc destroy_value;
};

struct blist_node {
    BListNode next;
    BListNode previous;
    void* value;
};

// Δημιουργεί και επιστρέφει μια νέα λίστα.
// Αν destroy_value != NULL, τότε καλείται destroy_value(value) κάθε φορά που αφαιρείται ένα στοιχείο.
//
BList blist_create(DestroyFunc destroy_value) {
    BList blist = malloc(sizeof(*blist));
    blist->size = 0;
    blist->destroy_value = destroy_value;

    blist->dummy = malloc(sizeof(*blist->dummy));
    blist->dummy->next = NULL;
    blist->dummy->previous = NULL;

    blist->last = blist->dummy;

    return blist;
}

// Επιστρέφει τον αριθμό στοιχείων που περιέχει η λίστα.
//
int blist_size(BList blist) { return blist->size; }

// Creates a new node
//
BListNode blist_create_node(void* value) {
    BListNode node = malloc(sizeof(*node));

    node->value = value;
    node->next = NULL;
    node->previous = NULL;

    return node;
}

// Προσθέτει έναν νέο κόμβο __πριν__ τον node (δηλαδή αν ο node είχε θέση i στη λίστα, o νέος κόμβος
// παίρνει τη θέση i και ο node πηγαίνει στην i+1), ή στo τέλος αν node == BLIST_EOF, με περιεχόμενο value.
//
void blist_insert(BList blist, BListNode node, void* value) {
    // Δημιουργία του νέου κόμβου
    BListNode new = blist_create_node(value);

    // Αν το node είναι NULL απλά εισάγουμε στο τέλος!
    if (node == NULL) {
        node = blist->last;
        node->next = new;
        new->previous = node;

        // Ενημέρωση του last
        blist->last = new;

    } else {
        // Σύνδεση του new ανάμεσα στο node και το node->next
        new->previous = node->previous;
        new->next = node->previous->next;
        node->previous = new;
        new->previous->next = new;
    }

    // Ενημέρωση του size
    blist->size++;
}

// Αφαιρεί τον κόμβο node (πρέπει να υπάρχει στη λίστα).
//
void blist_remove(BList blist, BListNode node) {
    // Αν το node είναι NULL απλά διαγράφουμε από το τέλος!
    if (node == NULL || node == blist->last) {
        node = blist->last->previous;  // Προτελευταίο

        BListNode removed = node->next;

        if (blist->destroy_value != NULL)
            blist->destroy_value(removed->value);

        free(removed);

        // Ενημέρωση του last
        blist->last = node;
    } else {
        node->previous->next = node->next;
        node->next->previous = node->previous;

        BListNode removed = node;

        if (blist->destroy_value != NULL)
            blist->destroy_value(removed->value);

        free(removed);
    }

    // Ενημέρωση του size
    blist->size--;
}

// Επιστρέφει την πρώτη τιμή που είναι ισοδύναμη με value
// (με βάση τη συνάρτηση compare), ή NULL αν δεν υπάρχει
//
void* blist_find(BList blist, void* value, CompareFunc compare) {
    BListNode node = blist_find_node(blist, value, compare);
    return node == NULL ? NULL : node->value;
}

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση στοιχείου σε
// destroy_value. Επιστρέφει την προηγούμενη τιμή της συνάρτησης.
//
DestroyFunc blist_set_destroy_value(BList blist, DestroyFunc destroy_value) {
    DestroyFunc old = blist->destroy_value;
    blist->destroy_value = destroy_value;
    return old;
}

// Ελευθερώνει όλη τη μνήμη που δεσμεύει η λίστα blist.
// Οποιαδήποτε λειτουργία πάνω στη λίστα μετά το destroy είναι μη ορισμένη.
//
void blist_destroy(BList blist) {
    // Διασχίζουμε όλη τη λίστα και κάνουμε free όλους τους κόμβους,
    // συμπεριλαμβανομένου και του dummy!
    //
    BListNode node = blist->dummy;
    while (node != NULL) {            // while αντί για for, γιατί θέλουμε να διαβάσουμε
        BListNode next = node->next;  // το node->next _πριν_ κάνουμε free!

        // Καλούμε τη destroy_value, αν υπάρχει (προσοχή, όχι στον dummy!)
        if (node != blist->dummy && blist->destroy_value != NULL)
            blist->destroy_value(node->value);

        free(node);
        node = next;
    }

    // Τέλος free το ίδιο το struct
    free(blist);
}

// Διάσχιση της λίστας /////////////////////////////////////////////

// Επιστρέφει τον πρώτο κόμβο της λίστας, ή BLIST_BOF αν η λίστα είναι κενή
//
BListNode blist_first(BList blist) { return blist->dummy->next; }

// Επιστρέφει τον τελευταίο κόμβο της λίστας, ή BLIST_ΕOF αν η λίστα είναι κενή
//
BListNode blist_last(BList blist) {
    // Προσοχή, αν η λίστα είναι κενή το last δείχνει στον dummy, εμείς όμως θέλουμε να επιστρέψουμε NULL, όχι τον dummy!
    //
    if (blist->last == blist->dummy)
        return BLIST_EOF;  // κενή λίστα
    else
        return blist->last;
}

// Επιστρέφει τον επόμενο κομβο του node, ή BLIST_EOF
// αν ο node δεν έχει επόμενο.
//
BListNode blist_next(BList blist, BListNode node) {
    assert(node != NULL);  // LCOV_EXCL_LINE (αγνοούμε το branch από τα coverage reports, είναι δύσκολο να τεστάρουμε το false γιατί θα κρασάρει το test)
    return node->next;
}

// Επιστρέφει τον προηγούμενο κομβο του node, ή BLIST_BOF
// αν ο node δεν έχει προηγούμενο.
//
BListNode blist_previous(BList blist, BListNode node) {
    assert(node != NULL);  // LCOV_EXCL_LINE (αγνοούμε το branch από τα coverage reports, είναι δύσκολο να τεστάρουμε το false γιατί θα κρασάρει το test)
    return node->previous;
}

// Επιστρέφει το περιεχόμενο του κόμβου node
//
void* blist_node_value(BList blist, BListNode node) {
    assert(node != NULL);  // LCOV_EXCL_LINE
    return node->value;
}

// Βρίσκει τo πρώτo στοιχείο που είναι ισοδύναμο με value (με βάση τη συνάρτηση compare).
// Επιστρέφει τον κόμβο του στοιχείου, ή BLIST_EOF αν δεν βρεθεί.
//
BListNode blist_find_node(BList blist, void* value, CompareFunc compare) {
    // διάσχιση όλης της λίστας, καλούμε την compare μέχρι να επιστρέψει 0
    //
    for (BListNode node = blist->dummy->next; node != NULL; node = node->next)
        if (compare(value, node->value) == 0)
            return node;  // βρέθηκε

    return NULL;  // δεν υπάρχει
}