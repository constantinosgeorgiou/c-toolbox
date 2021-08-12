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