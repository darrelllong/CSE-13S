#include "queue.h"

struct Queue queue_new(void) {
    struct Queue q = {
        .ll = ll_new()
    };
    return q;
}

void queue_drop(struct Queue q) {
    ll_drop(q.ll);
}

size_t queue_len(struct Queue *q) {
    return ll_len(&q->ll);
}

void enqueue(struct Queue *q, int data) {
    ll_add_back(&q->ll, data);
}

bool dequeue(struct Queue *q, int *data) {
    return ll_pop_front(&q->ll, data);
}

void queue_print(struct Queue *q) {
    ll_print(&q->ll);
}
