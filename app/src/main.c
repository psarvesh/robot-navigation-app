// C header files
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// POSIX header files
#include <pthread.h>

// Project header files
#include "ring_buffer.h"
#include "threads.h"

RingBuffer rb;

int main(int argc, char** argv) {
    int ret = 0;

    if(argc != 3) {
        fprintf(stdout, "Usage: ./robot <max size> <time>\n");
        fprintf(stdout, "<max size> = Max size of the buffer in characters\n");
        fprintf(stdout, "<time> = Time interval in seconds at which consumer wakes up to read\n");
        exit(EXIT_FAILURE);
    }

    // Initialize Buffer
    ret = InitBuffer(&rb, atoi(argv[1]));
    if(ret != 0) {
        fprintf(stderr, "Error at InitBuffer. Returned %d\n", ret);
        exit(EXIT_FAILURE);
    }

    int consumer_secs = atoi(argv[2]);

    // Initialize thread attributes to default
    pthread_attr_t attr;
    ret = pthread_attr_init(&attr);
    if(ret != 0) {
        fprintf(stderr, "Error at pthread_attr_init. Returned %d\n", ret);
        pthread_attr_destroy(&attr);
        exit(EXIT_FAILURE);
    }
    


    // Clear all the allocated memory
    pthread_attr_destroy(&attr);
    FreeBuffer(&rb);
    return EXIT_SUCCESS;
}