// C header files
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// POSIX header files
#include <pthread.h>
#include <unistd.h>
#include <poll.h>
#include <termios.h>

// Project header files
#include "ring_buffer.h"
#include "command.h"

static volatile bool exitFlag = false;    
static char cmd[max_cmd_size + 1]; // +++<4 CHARS>;

// Producer thread will be here
void *Producer(void *userdata) {
    char c;
    RingBuffer *pRB = (RingBuffer *)userdata;
    while(!exitFlag) {
        // Get user input
        c = getchar();
        if(!isspace(c)) {
            // Add to the ringbuffer
            while (Push(pRB, c) != 0);
        }
    }
    return NULL;
}

static void InitStdIO(struct termios *attr) {
    setbuf(stdout, NULL);
    struct termios new_attr;
    tcgetattr(STDIN_FILENO, attr);
    new_attr = *attr;
    new_attr.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
    fprintf(stdout, "input> ");
}

static void RestoreStdIO(struct termios *attr) {
    tcsetattr(STDIN_FILENO, TCSANOW, attr);
} 

// main will act as the main consumer thread
int main(int argc, char** argv) {
    if(argc != 3) {
        fprintf(stderr, "Usage: ./robot <max size> <time>\n");
        fprintf(stderr, "<max size> = Max size of the buffer in characters\n");
        fprintf(stderr, "<time> = Time interval in seconds at which consumer wakes up to read\n");
        exit(EXIT_FAILURE);
    }
    int ret = 0;
    struct termios attr;

    // Turn off Canonical mode
    InitStdIO(&attr);

    // Initialize Buffer
    RingBuffer rb;
    ret = InitBuffer(&rb, atoi(argv[1]));
    if(ret != 0) {
        fprintf(stderr, "Error at InitBuffer. Returned %d\n", ret);
        exit(EXIT_FAILURE);
    }

    // Create producer
    pthread_t producer;
    ret = pthread_create(&producer, NULL, Producer, &rb);
    if(ret != 0) {
        fprintf(stderr, "Could not create Producer. Returned %d\n", ret);
        exit(EXIT_FAILURE);
    }

    // Consumer thread
    int sleep_time = atoi(argv[2]);
    char c = 0;
    int pos = 0;
    memset(cmd, 0, max_cmd_size + 1);
    while(!exitFlag) {
        sleep(sleep_time);
        while(Pop(&rb, &c) == 0) {  
            cmd[pos++] = c;
            if(pos == max_cmd_size) {
                // Check if valid command
                CommandType type;
                int n;
                ParseCommand(cmd, &type, &n);
                if(type == kExit) {
                    exitFlag = true;
                    pthread_cancel(producer);
                    break;
                }
                memset(cmd, 0, max_cmd_size + 1);
                pos = 0; 
            } 
        }
    }
    
    // Clean up
    void *res; // For debugging purposes
    pthread_join(producer, &res);
    FreeBuffer(&rb);
    fprintf(stdout, "\n<terminated>\n");
    RestoreStdIO(&attr);
    return EXIT_SUCCESS;
}
