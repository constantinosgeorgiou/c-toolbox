#include "queue.h"

#include "ADTBidirectionalList.h"

struct queue {
    BList data;
};

Queue queue_create(DestroyFunc destroy_value) {
    Queue queue = malloc(sizeof(*queue));
    if (queue == NULL) return NULL;

    queue->data = blist_create(destroy_value);
    if (queue->data == NULL) return NULL;

    return queue;
}

void queue_destroy(Queue queue) {
    blist_destroy(queue->data);
    free(queue);
}

void queue_enqueue(Queue queue, void* value) {
    blist_insert(queue->data, blist_last(queue->data), value);
}

void queue_dequeue(Queue queue) {
    blist_remove(queue->data, blist_first(queue->data));
}

size_t queue_size(Queue queue) { return blist_size(queue->data); }

bool queue_is_empty(Queue queue) {
    return blist_size(queue->data) != 0 ? false : true;
}

void* queue_front(Queue queue) {
    return blist_node_value(queue->data, blist_first(queue->data));
}

void* queue_back(Queue queue) {
    return blist_node_value(queue->data, blist_last(queue->data));
}

DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value) {
    return blist_set_destroy_value(queue->data, destroy_value);
}