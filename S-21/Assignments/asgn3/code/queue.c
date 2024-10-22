#include "queue.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct Queue {
    uint32_t head;
    uint32_t tail;
    uint32_t size;
    uint32_t capacity;
    int64_t *items;
};

Queue *queue_create(uint32_t capacity) {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (q) {
        q->head = q->tail = 0;
        q->capacity = capacity;
        q->items = (int64_t *)calloc(capacity, sizeof(int64_t));
        if (!q->items) {
            free(q);
            q = NULL;
        }
    }
    return q;
}

void queue_delete(Queue **q) {
    if (*q && (*q)->items) {
        free((*q)->items);
        free(*q);
        *q = NULL;
    }
}

bool queue_empty(Queue *q) {
    return q->size == 0;
}

bool queue_full(Queue *q) {
    return q->size == q->capacity;
}

uint32_t queue_size(Queue *q) {
    return q->size;
}

bool enqueue(Queue *q, int64_t x) {
    if (!queue_full(q)) {
        q->size += 1;
        q->items[q->tail] = x;
        q->tail = (q->tail + 1) % q->capacity;
        return true;
    }
    return false;
}

bool dequeue(Queue *q, int64_t *x) {
    if (!queue_empty(q)) {
        q->size -= 1;
        *x = q->items[q->head];
        q->head = (q->head + 1) % q->capacity;
        return true;
    }
    return false;
}

void queue_print(Queue *q) {
    printf("[");
    for (uint32_t i = 0; i < q->size; i += 1) {
        printf("%" PRId64, q->items[(q->head + i) % q->capacity]);
        if (i + 1 != q->size) {
            printf(", ");
        }
    }
    printf("]\n");
}
