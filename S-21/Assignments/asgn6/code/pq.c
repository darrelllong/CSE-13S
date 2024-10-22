#include "pq.h"
#include "node.h"
#include <stdio.h>
#include <stdlib.h>

struct PriorityQueue {
    Node **heap;
    uint32_t size;
    uint32_t capacity;
};

static inline uint32_t parent(uint32_t k) {
    return (k - 1) >> 1;
}

static inline uint32_t left(uint32_t k) {
    return (k << 1) + 1;
}

static inline uint32_t right(uint32_t k) {
    return (k << 1) + 2;
}

static void swap(Node **x, Node **y) {
    Node *t = *x;
    *x      = *y;
    *y      = t;
}

static void heapify(PriorityQueue *pq, uint32_t k) {
    uint32_t largest = k;
    uint32_t L       = left(k);
    uint32_t R       = right(k);
    if (L < pq->size && node_cmp(pq->heap[k], pq->heap[L])) {
        largest = L;
    } else {
        largest = k;
    }
    if (R < pq->size && node_cmp(pq->heap[largest], pq->heap[R])) {
        largest = R;
    }
    if (largest != k) {
        swap(&pq->heap[k], &pq->heap[largest]);
        heapify(pq, largest);
    }
}

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    if (pq) {
        pq->size     = 0;
        pq->capacity = capacity;
        pq->heap     = (Node **)calloc(capacity, sizeof(Node *));
        if (!pq->heap) {
            free(pq);
            pq = NULL;
        }
    }
    return pq;
}

void pq_delete(PriorityQueue **pq) {
    if (*pq && (*pq)->heap) {
        for (uint32_t i = 0; i < (*pq)->size; i += 1) {
            node_delete(&(*pq)->heap[i]);
        }
        free((*pq)->heap);
        free(*pq);
        *pq = NULL;
    }
}

uint32_t pq_size(PriorityQueue *pq) {
    return pq->size;
}

bool pq_empty(PriorityQueue *pq) {
    return pq->size == 0;
}

bool pq_full(PriorityQueue *pq) {
    return pq->size == pq->capacity;
}

bool enqueue(PriorityQueue *pq, Node *n) {
    if (!pq_full(pq)) {
        uint32_t k = pq->size++;
        while (k > 0 && node_cmp(pq->heap[parent(k)], n)) {
            pq->heap[k] = pq->heap[parent(k)];
            k           = parent(k);
        }
        pq->heap[k] = n;
        return true;
    }
    return false;
}

bool dequeue(PriorityQueue *pq, Node **n) {
    if (!pq_empty(pq)) {
        *n          = pq->heap[0];
        pq->heap[0] = pq->heap[--pq->size];
        heapify(pq, 0);
        return true;
    }
    return false;
}

void pq_print(PriorityQueue *pq) {
    Node *n             = NULL;
    PriorityQueue *temp = pq_create(pq->capacity);

    // Dequeue each node, place into temporary priority queue.
    printf("[");
    while (!pq_empty(pq)) {
        dequeue(pq, &n);
        enqueue(temp, n);
        node_print(n);
        if (pq->size) {
            printf(", ");
        }
    }
    printf("]\n");

    // Restore original queue after printing out contents in order.
    while (!pq_empty(temp)) {
        dequeue(temp, &n);
        enqueue(pq, n);
    }

    pq_delete(&temp);
}
