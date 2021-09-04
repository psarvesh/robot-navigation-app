// C header files
#include <stdlib.h>
#include <stdbool.h>

// Project header files
#include "ring_buffer.h"

int InitBuffer(RingBuffer *rb, int max_size) {
    rb->buffer = malloc((max_size + 1) * sizeof(char));
    if(rb->buffer == NULL) 
        return -1;
    rb->capacity = max_size + 1;
    ResetBuffer(rb);
    return 0;
}

int Push(RingBuffer *rb, char ch) {
    if(rb == NULL || ((rb->head + 1) % rb->capacity == rb->tail)) {
        return -1;
    }

    rb->buffer[rb->head] = ch;
    rb->head = (rb->head + 1) % rb->capacity;
    return 0;
}

int Pop(RingBuffer *rb, char *ch){
    if(rb == NULL || ch == NULL || (rb->head == rb->tail)) {
        return -1;
    }

    *ch = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->capacity;
    return 0;
}

void ResetBuffer(RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
}

void FreeBuffer(RingBuffer *rb) {
    free(rb->buffer);
}