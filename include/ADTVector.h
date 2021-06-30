#pragma once

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

#define VECTOR_ERROR (Vector)0

typedef struct vector* Vector;

// Δημιουργεί και επιστρέφει ένα νεό vector μεγέθους size, με στοιχεία αρχικοποιημένα σε NULL.
// Αν δεν υπάρχει διαθέσιμη μνήμη επιστρέφει VECTOR_FAIL.
// Αν destroy_value != NULL, τότε καλείται destroy_value(value) κάθε φορά που αφαιρείται (ή αντικαθίσταται) ένα στοιχείο.

Vector vector_create(int size, DestroyFunc destroy_value);

// Ελευθερώνει όλη τη μνήμη που δεσμεύει το vector vec.
// Οποιαδήποτε λειτουργία πάνω στο vector μετά το destroy είναι μη ορισμένη.

void vector_destroy(Vector vec);

// Αλλάζει τη συνάρτηση που καλείται σε κάθε αφαίρεση/αντικατάσταση στοιχείου σε
// destroy_value. Επιστρέφει την προηγούμενη τιμή της συνάρτησης.

DestroyFunc vector_set_destroy_value(Vector vec, DestroyFunc destroy_value);

// Επιστρέφει τον αριθμό στοιχείων που περιέχει το vector vec.

int vector_size(Vector vec);

// Προσθέτει την τιμή value στο _τέλος_ του vector vec. Το μέγεθος του vector μεγαλώνει κατά 1.
// Αν δεν υπάρχει διαθέσιμη μνήμη το vector παραμένει όπως ήταν (αυτό μπορεί να ελεγχθεί με τη vector_size)

void vector_insert_last(Vector vec, void* value);

// Αφαιρεί το τελευταίο στοιχείο του vector. Το μέγεθος του vector μικραίνει κατά 1.
// Αν το vector είναι κενό η συμπεριφορά είναι μη ορισμένη.

void vector_remove_last(Vector vec);

// Βρίσκει και επιστρέφει το πρώτο στοιχείο στο vector που να είναι ίσο με value
// (με βάση τη συνάρτηση compare), ή NULL αν δεν βρεθεί κανένα στοιχείο.

void* vector_find(Vector vec, void* value, CompareFunc compare);

// Επιστρέφει την τιμή στη θέση pos του vector vec (μη ορισμένο αποτέλεσμα αν pos < 0 ή pos >= size)

void* vector_get_at(Vector vec, int pos);

// Αλλάζει την τιμή στη θέση pos του Vector vec σε value.
// ΔΕΝ μεταβάλλει το μέγεθος του vector, αν pos >= size το αποτέλεσμα δεν είναι ορισμένο.

void vector_set_at(Vector vec, int pos, void* value);

// Οι σταθερές αυτές συμβολίζουν εικονικούς κόμβους _πριν_ τον πρώτο και _μετά_ τον τελευταίο
#define VECTOR_BOF (VectorNode)0
#define VECTOR_EOF (VectorNode)0

typedef struct vector_node* VectorNode;

// Βρίσκει το πρώτο στοιχείο στο vector που να είναι ίσο με value (με βάση τη συνάρτηση compare).
// Επιστρέφει τον κόμβο του στοιχείου, ή VECTOR_EOF αν δεν βρεθεί.

VectorNode vector_find_node(Vector vec, void* value, CompareFunc compare);

// Επιστρέφει το περιεχόμενο του κόμβου node

void* vector_node_value(Vector vec, VectorNode node);

// Διάσχιση του vector ////////////////////////////////////////////////////////////
//
// Οι παρακάτω συναρτήσεις επιτρέπουν τη διάσχιση του vector μέσω κόμβων.
// Δεν είναι τόσο συχνά χρησιμοποιούμενες όσο για άλλα ADTs, γιατί μπορούμε
// εύκολα να διασχίσουμε το array και μέσω indexes. Παραμένουν πάντως χρήσιμες,
// τόσο για ομοιομορφία με τους άλλους ADTs, αλλά και γιατί για κάποιες υλοποιήσεις
// η διάσχιση μέσω κόμβων μπορεί να είναι πιο γρήγορη.

// Επιστρέφουν τον πρώτο και τον τελευταίο κομβο του vector, ή VECTOR_BOF / VECTOR_EOF αντίστοιχα αν το vector είναι κενό

VectorNode vector_first(Vector vec);
VectorNode vector_last(Vector vec);

// Επιστρέφουν τον επόμενο και τον προηγούμενο κομβο του node, ή VECTOR_EOF / VECTOR_BOF
// αντίστοιχα αν ο node δεν έχει επόμενο / προηγούμενο.

VectorNode vector_next(Vector vec, VectorNode node);
VectorNode vector_previous(Vector vec, VectorNode node);
