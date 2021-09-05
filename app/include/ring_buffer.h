#ifndef __APP_RING_BUFFER__
#define __APP_RING_BUFFER__

// C header files
#include <stdatomic.h>

// POSIX system headers
#include <pthread.h>
typedef struct  {
    char *buffer;
    int capacity;
    atomic_int head, tail; // Should be atomic to ensure partial reads do not affect execution
} RingBuffer;

int InitBuffer(RingBuffer *rb, int max_size);

int Push(RingBuffer *rb, char ch);

int Pop(RingBuffer *rb, char *ch);

void ResetBuffer(RingBuffer *rb);

void FreeBuffer(RingBuffer *rb);

#endif //__APP_RING_BUFFER__