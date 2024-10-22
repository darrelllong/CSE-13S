#include "pq.h"

#include "anode.h"

#include <stdio.h>
#include <stdlib.h>

struct PriorityQueue {
    AuthorNode **heap;
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

static void swap(AuthorNode **x, AuthorNode **y) {
    AuthorNode *t = *x;
    *x = *y;
    *y = t;
}

static void heapify(PriorityQueue *pq, uint32_t k) {
    uint32_t largest = k;
    uint32_t L = left(k);
    uint32_t R = right(k);
    if (L < pq->size && anode_cmp(pq->heap[k], pq->heap[L])) {
        largest = L;
    } else {
        largest = k;
    }
    if (R < pq->size && anode_cmp(pq->heap[largest], pq->heap[R])) {
        largest = R;
    }
    if (largest != k) {
        swap(&pq->heap[k], &pq->heap[largest]);
        heapify(pq, largest);
    }
}

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (pq) {
        pq->size = 0;
        pq->capacity = capacity;
        pq->heap = (AuthorNode **) calloc(capacity, sizeof(AuthorNode *));
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
            if ((*pq)->heap[i]) {
                free((*pq)->heap[i]->author);
            }
            anode_delete(&(*pq)->heap[i]);
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

bool enqueue(PriorityQueue *pq, char *author, double dist) {
    AuthorNode *n = anode_create(author, dist);
    if (!pq_full(pq)) {
        uint32_t k = pq->size++;
        while (k > 0 && anode_cmp(pq->heap[parent(k)], n)) {
            pq->heap[k] = pq->heap[parent(k)];
            k = parent(k);
        }
        pq->heap[k] = n;
        return true;
    }
    return false;
}

bool dequeue(PriorityQueue *pq, char **author, double *dist) {
    AuthorNode *n = NULL;
    if (!pq_empty(pq)) {
        n = pq->heap[0];
        pq->heap[0] = pq->heap[--pq->size];
        heapify(pq, 0);
        *author = n->author;
        *dist = n->dist;
        anode_delete(&n);
        return true;
    }
    return false;
}

void pq_print(PriorityQueue *pq) {
    PriorityQueue *temp = pq_create(pq->capacity);

    // Dequeue each node, place into temporary priority queue.
    printf("[");
    while (!pq_empty(pq)) {
        char *author;
        double dist;
        dequeue(pq, &author, &dist);
        enqueue(temp, author, dist);
        printf("(%s, %lf)", author, dist);
        if (pq->size) {
            printf(", ");
        }
    }
    printf("]\n");

    // Restore original queue after printing out contents in order.
    while (!pq_empty(temp)) {
        char *author;
        double dist;
        dequeue(temp, &author, &dist);
        enqueue(pq, author, dist);
    }

    pq_delete(&temp);
}
