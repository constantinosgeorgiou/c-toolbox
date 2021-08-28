/////////////////////////////////////////////////////////////////////////////
//
// Υλοποίηση του ADT Map μέσω Hash Table με open addressing (linear probing)
//
/////////////////////////////////////////////////////////////////////////////

#include "ADTMap.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ADTList.h"

// Το μέγεθος του Hash Table ιδανικά θέλουμε να είναι πρώτος αριθμός σύμφωνα με την θεωρία.
// Η παρακάτω λίστα περιέχει πρώτους οι οποίοι έχουν αποδεδιγμένα καλή συμπεριφορά ως μεγέθη.
// Κάθε re-hash θα γίνεται βάσει αυτής της λίστας. Αν χρειάζονται παραπάνω απο 1610612741 στοχεία, τότε σε καθε rehash διπλασιάζουμε το μέγεθος.
int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
                     786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

// Χρησιμοποιούμε open addressing, οπότε σύμφωνα με την θεωρία, πρέπει πάντα να διατηρούμε
// τον load factor του  hash table μικρότερο ή ίσο του 0.5, για να έχουμε αποδoτικές πράξεις
#define MAX_LOAD_FACTOR 0.9

// Δομή του κάθε κόμβου που έχει το hash table (με το οποίο υλοιποιούμε το map)

// Το MapNode περιέχει τα δεδομένα
struct map_node {
    void* key;    // Το κλειδί που χρησιμοποιείται για να hash-αρουμε
    void* value;  // Η τιμή που αντισοιχίζεται στο παραπάνω κλειδί
};

// Δομή του Map (περιέχει όλες τις πληροφορίες που χρεαζόμαστε για το HashTable)
struct map {
    List* array;              // Array of lists (buckets)
    int capacity;             // Πόσο χώρο έχουμε δεσμεύσει.
    int size;                 // Πόσα στοιχεία έχουμε προσθέσει
    CompareFunc compare;      // Συνάρτηση για σύγκρηση δεικτών, που πρέπει να δίνεται απο τον χρήστη
    HashFunc hash_function;   // Συνάρτηση για να παίρνουμε το hash code του κάθε αντικειμένου.
    DestroyFunc destroy_key;  // Συναρτήσεις που καλούνται όταν διαγράφουμε έναν κόμβο απο το map.
    DestroyFunc destroy_value;
};

Map map_create(CompareFunc compare, DestroyFunc destroy_key, DestroyFunc destroy_value) {
    // Δεσμεύουμε κατάλληλα τον χώρο που χρειαζόμαστε για το hash table
    Map map = malloc(sizeof(*map));
    if (map == NULL) {
        return NULL;
    }

    map->capacity = prime_sizes[0];

    // Αρχικοποιούμε τους κόμβους που έχουμε σαν διαθέσιμους.
    map->array = malloc(map->capacity * sizeof(*map->array));
    if (map->array == NULL) {
        return NULL;
    }
    for (int i = 0; i < map->capacity; i++) {
        map->array[i] = list_create(NULL);
    }

    map->size = 0;

    map->compare = compare;
    map->destroy_key = destroy_key;
    map->destroy_value = destroy_value;

    return map;
}

// Επιστρέφει τον αριθμό των entries του map σε μία χρονική στιγμή.
int map_size(Map map) {
    return map->size;
}

// Συνάρτηση για την επέκταση του Hash Table σε περίπτωση που ο load factor μεγαλώσει πολύ.
static void rehash(Map map) {
    // Αποθήκευση των παλιών δεδομένων
    int old_capacity = map->capacity;
    List* old_array = map->array;

    // Βρίσκουμε τη νέα χωρητικότητα, διασχίζοντας τη λίστα των πρώτων ώστε να βρούμε τον επόμενο.
    int prime_no = sizeof(prime_sizes) / sizeof(int);  // το μέγεθος του πίνακα
    for (int i = 0; i < prime_no; i++) {               // LCOV_EXCL_LINE
        if (prime_sizes[i] > old_capacity) {
            map->capacity = prime_sizes[i];
            break;
        }
    }

    // Αν έχουμε εξαντλήσει όλους τους πρώτους, διπλασιάζουμε
    if (map->capacity == old_capacity)  // LCOV_EXCL_LINE
        map->capacity *= 2;             // LCOV_EXCL_LINE

    // Δημιουργούμε ένα μεγαλύτερο hash table
    map->array = malloc(map->capacity * sizeof(List));

    // Αρχικοποιούμε τους κόμβους που έχουμε σαν διαθέσιμους.
    for (int i = 0; i < map->capacity; i++)
        map->array[i] = list_create(NULL);

    // Τοποθετούμε τα entries
    map->size = 0;
    for (int i = 0; i < old_capacity; i++) {
        if (list_size(old_array[i]) != 0) {
            for (ListNode entry = list_first(old_array[i]); entry != LIST_EOF; entry = list_next(old_array[i], entry)) {
                MapNode node = (MapNode)list_node_value(old_array[i], entry);
                // printf("REHASH: node(%p) key(%p) value(%p)\n", node, node->key, node->value);
                map_insert(map, node->key, node->value);
            }
        }

        list_set_destroy_value(old_array[i], free);
        list_destroy(old_array[i]);
    }

    //Αποδεσμεύουμε τον παλιό πίνακα ώστε να μήν έχουμε leaks
    free(old_array);
}

// Creates a bucket node
static MapNode map_node_create(void* key, void* value) {
    MapNode node = malloc(sizeof(*node));

    node->key = key;
    node->value = value;

    return node;
}

// Εισαγωγή στο hash table του ζευγαριού (key, item).

void map_insert(Map map, void* key, void* value) {
    // Check if given key is already in map:
    MapNode in_map = map_find_node(map, key);

    if (in_map) {
        void* old_key = in_map->key;
        void* old_value = in_map->value;

        // Destroy old key, value pair
        if (map->destroy_key != NULL)
            map->destroy_key(old_key);
        if (map->destroy_value != NULL)
            map->destroy_value(old_value);

        // Update key and value
        in_map->key = key;
        in_map->value = value;
    } else {
        assert(map->hash_function != NULL && key != NULL && "Expected key and hash function");

        // Hash key to find the position of insertion
        unsigned int pos = map->hash_function(key) % map->capacity;

        MapNode new = map_node_create(key, value);

        // printf("INSERT: node(%p) key(%p) value(%p)\n", new, new->key, new->value);

        list_insert_next(map->array[pos], list_last(map->array[pos]), (void*)new);

        // Νέο στοιχείο, αυξάνουμε τα συνολικά στοιχεία του map
        map->size++;

        // Αν με την νέα εισαγωγή ξεπερνάμε το μέγιστο load factor, πρέπει να κάνουμε rehash
        float load_factor = (float)map->size / map->capacity;
        if (load_factor > MAX_LOAD_FACTOR)
            rehash(map);
    }
}

// Διαργραφή απο το Hash Table του κλειδιού με τιμή key
bool map_remove(Map map, void* key) {
    assert(map->hash_function != NULL && key != NULL && "Expected key and hash function");

    // Hash key to find the position of insertion
    unsigned int pos = map->hash_function(key) % map->capacity;

    // Store previous bucket entry of entry
    ListNode previous = LIST_BOF;

    for (ListNode entry = list_first(map->array[pos]); entry != LIST_EOF; entry = list_next(map->array[pos], entry)) {
        MapNode node = (MapNode)list_node_value(map->array[pos], entry);

        if (map->compare(node->key, key) == 0) {
            // Destroy node key, value
            if (map->destroy_key != NULL)
                map->destroy_key(node->key);
            if (map->destroy_value != NULL)
                map->destroy_value(node->value);

            // Destroy node and bucket entry
            list_set_destroy_value(map->array[pos], free);
            list_remove_next(map->array[pos], previous);
            list_set_destroy_value(map->array[pos], NULL);

            map->size--;

            return true;
        }

        previous = entry;
    }

    return false;
}

// Αναζήτηση στο map, με σκοπό να επιστραφεί το value του κλειδιού που περνάμε σαν όρισμα.

void* map_find(Map map, void* key) {
    MapNode node = map_find_node(map, key);
    if (node != MAP_EOF)
        return node->value;
    else
        return NULL;
}

DestroyFunc map_set_destroy_key(Map map, DestroyFunc destroy_key) {
    DestroyFunc old = map->destroy_key;
    map->destroy_key = destroy_key;
    return old;
}

DestroyFunc map_set_destroy_value(Map map, DestroyFunc destroy_value) {
    DestroyFunc old = map->destroy_value;
    map->destroy_value = destroy_value;
    return old;
}

// Απελευθέρωση μνήμης που δεσμεύει το map
void map_destroy(Map map) {
    // Traverse each bucket of map array
    for (int i = 0; i < map->capacity; i++) {
        // Traverse only the buckets that contain entries
        if (list_size(map->array[i]) != 0) {
            // Traverse each entry in a bucket
            for (ListNode entry = list_first(map->array[i]); entry != LIST_EOF; entry = list_next(map->array[i], entry)) {
                MapNode node = (MapNode)list_node_value(map->array[i], entry);

                // Destroy MapNode key, value pairs
                if (map->destroy_key != NULL)
                    map->destroy_key(node->key);
                if (map->destroy_value != NULL)
                    map->destroy_value(node->value);
            }
        }

        // Set list destroy value to destroy MapNode structs and the whole list
        list_set_destroy_value(map->array[i], free);
        list_destroy(map->array[i]);
    }

    free(map->array);
    free(map);
}

/////////////////////// Διάσχιση του map μέσω κόμβων ///////////////////////////

MapNode map_first(Map map) {
    for (int i = 0; i < map->capacity; i++) {
        if (list_first(map->array[i]) != LIST_EOF)
            return (MapNode)list_node_value(map->array[i], list_first(map->array[i]));
    }

    return MAP_EOF;
}

MapNode map_next(Map map, MapNode node) {
    assert(map->hash_function != NULL && node != NULL && "Expected key and hash function");

    // Hash key to find the position of insertion
    unsigned int pos = map->hash_function(node->key) % map->capacity;

    for (ListNode entry = list_first(map->array[pos]); entry != LIST_EOF; entry = list_next(map->array[pos], entry)) {
        MapNode current = (MapNode)list_node_value(map->array[pos], entry);

        if (map->compare(current->key, node->key) == 0) {
            // Check if current node is the last in the bucket
            if (list_next(map->array[pos], entry) == LIST_EOF) {
                // Iterate array to find a list that is not empty
                for (pos += 1; pos < map->capacity; pos++) {
                    if (list_size(map->array[pos]) != 0) {
                        // Return first bucket entry node
                        return (MapNode)list_node_value(map->array[pos], list_first(map->array[pos]));
                    }
                }
                return MAP_EOF;  // Reached end of array

            } else {
                // Return next bucket entry node
                return (MapNode)list_node_value(map->array[pos], list_next(map->array[pos], entry));
            }
        }
    }

    return MAP_EOF;  // node was not found in the map
}

void* map_node_key(Map map, MapNode node) {
    return node->key;
}

void* map_node_value(Map map, MapNode node) {
    return node->value;
}

MapNode map_find_node(Map map, void* key) {
    assert(map->hash_function != NULL && key != NULL && "Expected key and hash function");

    unsigned int pos = map->hash_function(key) % map->capacity;

    for (ListNode entry = list_first(map->array[pos]); entry != LIST_EOF; entry = list_next(map->array[pos], entry)) {
        MapNode node = (MapNode)list_node_value(map->array[pos], entry);

        if (map->compare(node->key, key) == 0) {
            return node;  // FOUND IT
        }
    }

    return MAP_EOF;
}

// Αρχικοποίηση της συνάρτησης κατακερματισμού του συγκεκριμένου map.
void map_set_hash_function(Map map, HashFunc func) {
    map->hash_function = func;
}

unsigned int hash_string(void* value) {
    // djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική
    unsigned int hash = 5381;
    for (char* s = value; *s != '\0'; s++)
        hash = (hash << 5) + hash + *s;  // hash = (hash * 33) + *s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo * 32.
    return hash;
}

unsigned int hash_int(void* value) {
    return *(int*)value;
}

unsigned int hash_pointer(void* value) {
    return (size_t)value;  // cast σε sizt_t, που έχει το ίδιο μήκος με έναν pointer
}