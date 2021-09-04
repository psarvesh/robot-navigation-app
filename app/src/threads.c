// C library header files
#include <stdio.h>
#include <stdbool.h>

// Project header files
#include "threads.h"

static volatile bool exitFlag = false;

void *Producer(void *userdata) {

    while(exitFlag) {

    }

    return NULL;
}

void *Consumer(void *userdata) {
    while(exitFlag) {
        
    }
    return NULL;
}