#ifndef __ROUNDED_QUEUE_H__
#define __ROUNDED_QUEUE_H__

#define MAX_ELEMENT (20)

typedef struct {
    int head;
    int tail;
    int counter;
    int arr[MAX_ELEMENT];
} rounded_queue_t;

int is_empty(rounded_queue_t* q) {
    if (q == NULL) 
        return 1;

    return q->counter == 0;
}

int* get_top(rounded_queue_t* q) {
    if (q == NULL || q->counter == 0)
        return NULL;

    return &q->arr[q->head];
}

int* enqueue(rounded_queue_t* q, int element) {
    if (q == NULL || q->counter == MAX_ELEMENT)
        return NULL;
    
    q->counter++;
    q->arr[q->tail] = element;

    int* val = &q->arr[q->tail];
    q->tail = (q->tail + 1) % MAX_ELEMENT;

    return val;
}

int dequeue(rounded_queue_t* q) {
    if (q == NULL || q->counter == 0)
        return -1;

    int val = q->arr[q->head];
    q->counter--;
    q->head = (q->head + 1) % MAX_ELEMENT;

    return val;
}

#endif /* __ROUNDED_QUEUE_H__ */