#include "ADTOrderedSet.h"

struct ordered_set {
    CompareFunc compare;
    DestroyFunc destroy_key;
    DestroyFunc destroy_value;
    int size;

    OrderedSetNode first;
    OrderedSetNode last;

    OrderedSetNode header;
};

struct ordered_set_node {
    OrderedSetNode top;
    OrderedSetNode bottom;
    OrderedSetNode next;
    OrderedSetNode previous;

    void* value;
};