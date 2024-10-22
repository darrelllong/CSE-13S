#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>
#include <stdint.h>

#define Item uint32_t

typedef struct Queue Queue;

Queue *queue_create(uint32_t capacity);

void queue_delete(Queue **q);

bool queue_empty(Queue *q);

bool queue_full(Queue *q);

uint32_t queue_size(Queue *q);

bool enqueue(Queue *q, Item x);

bool dequeue(Queue *q, Item *x);

void queue_print(Queue *q);

#endif
