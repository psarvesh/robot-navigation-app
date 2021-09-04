#ifndef __APP_RING_BUFFER__
#define __APP_RING_BUFFER__

#include <pthread.h>

typedef struct RingBuffer RingBuffer;

int InitBuffer(RingBuffer *rb, int max_size);

int Push(RingBuffer *rb, char ch);

int Pop(RingBuffer *rb, char *ch);

void ResetBuffer(RingBuffer *rb);

void FreeBuffer(RingBuffer *rb);

#endif //__APP_RING_BUFFER__