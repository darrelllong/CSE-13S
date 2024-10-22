#pragma once

#include "ll.h"

struct Queue {
    struct LinkedList ll;
};

struct Queue queue_new(void);

void queue_drop(struct Queue q);

size_t queue_len(struct Queue *q);

void enqueue(struct Queue *q, int data);

bool dequeue(struct Queue *q, int *data);

void queue_print(struct Queue *q);
