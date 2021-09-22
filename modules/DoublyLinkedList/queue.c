#include "queue.h"

#include "list.h"
#include <stdlib.h>

struct queue {
  List data;
};

Queue queue_create(DestroyFunc destroy_value) {
  Queue queue = malloc(sizeof(*queue));
  if (queue == NULL)
    return NULL;

  queue->data = list_create(destroy_value);
  if (queue->data == NULL)
    return NULL;

  return queue;
}

void queue_destroy(Queue queue) {
  list_destroy(queue->data);
  free(queue);
}

void queue_enqueue(Queue queue, void *value) {
  list_insert(queue->data, list_last(queue->data), value);
}

void queue_dequeue(Queue queue) {
  list_remove(queue->data, list_first(queue->data));
}

size_t queue_size(Queue queue) { return list_size(queue->data); }

bool queue_is_empty(Queue queue) {
  return list_size(queue->data) != 0 ? false : true;
}

void *queue_front(Queue queue) {
  return list_node_value(queue->data, list_first(queue->data));
}

void *queue_back(Queue queue) {
  return list_node_value(queue->data, list_last(queue->data));
}

DestroyFunc queue_set_destroy_value(Queue queue, DestroyFunc destroy_value) {
  return list_set_destroy_value(queue->data, destroy_value);
}