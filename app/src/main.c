// C header files
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// POSIX header files
#include <pthread.h>
#include <unistd.h>

// Project header files
#include "ring_buffer.h"
#include "threads.h"

RingBuffer rb;

static volatile bool exitFlag = false;
static const int max_cmd_size = 8; // +++<4 CHARS>;

typedef enum {
    kNorth,
    kSouth,
    kEast,
    kWest, 
    kExit, 
    kInvalid
} CommandType;

static void ParseCommand(const char *str, CommandType *type, int *n) {
    *type = kInvalid;
    *n = 0;
    
}

// producer thread will be here
void *Producer(void *userdata) {
    char c;
    RingBuffer *pRB = (RingBuffer *)userdata;
    while(exitFlag) {
        // Get user input
        c = getchar();

        // Add to the ringbuffer
        while (Push(pRB, c) != 0);
    }

    return NULL;
}

// main will act as the main consumer thread
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

    int sleep_time = atoi(argv[2]);

    // Initialize thread attributes to default
    pthread_attr_t attr;
    ret = pthread_attr_init(&attr);
    if(ret != 0) {
        fprintf(stderr, "Error at pthread_attr_init. Returned %d\n", ret);
        pthread_attr_destroy(&attr);
        exit(EXIT_FAILURE);
    }
    
    pthread_t producer;
//TODO : Complete creation of producer
    char c = 0;
    char cmd[max_cmd_size + 1];
    memset(cmd, 0, max_cmd_size + 1);
    int pos = 0;
    while(exitFlag) {
        sleep(sleep_time);
        if(Pop(&rb, &c) == -1) {
            continue;
        }  
        cmd[pos++] = c;
        if(pos == max_cmd_size + 1) {
            pos = 0;
            // Check if valid command
            CommandType type;
            int n;
            fprintf(stdout, "input> %s\n", cmd);
            ParseCommand(cmd, &type, &n);
            switch(type) {
                case kNorth:
                    fprintf(stdout, "output> Moved north %d steps\n", n);
                    break;
                case kSouth:
                    fprintf(stdout, "output> Moved south %d steps\n", n);
                    break;
                case kWest:
                    fprintf(stdout, "output> Moved west %d steps\n", n);
                    break;
                case kEast:
                    fprintf(stdout, "output> Moved east %d steps\n", n);
                    break;
                case kExit:
                    fprintf(stdout, "output> Exited\n");
                    exitFlag = true;
                    pthread_cancel(producer);
                    break;
                case kInvalid:
                default:
                    fprintf(stdout, "output> Invalid command\n");
                    break;
            }
            
            
        } 
    }

    // Clear all the allocated memory
    pthread_attr_destroy(&attr);
    FreeBuffer(&rb);
    return EXIT_SUCCESS;
}