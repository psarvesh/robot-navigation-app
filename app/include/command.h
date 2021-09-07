#ifndef __APP_COMMAND__
#define __APP_COMMAND__

// C header files
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define max_cmd_size  8  

typedef enum {
    kNorth,
    kSouth,
    kEast,
    kWest, 
    kExit, 
    kInvalid
} CommandType;

void ParseCommand(const char *str, CommandType *type, int *n) {
    
    *type = kInvalid;
    *n = 0;
    
    if(strlen(str) != max_cmd_size) {
        fprintf(stdout, "\noutput> Invalid command\ninput> ");
    } else if(strcmp(str, "+++EXIT;") == 0) { // Special case
        *type = kExit;
        fprintf(stdout, "\noutput> Exited");
    } else if(!(str[0] == '+' && str[1] == '+' && str[2] == '+' && str[3] == 'M' && 
            (str[4] == 'N' || str[4] == 'S' || str[4] == 'E' || str[4] == 'W') && 
            isdigit(str[5]) && isdigit(str[6]) && str[max_cmd_size - 1] ==';')) {
        // Invalid case
        fprintf(stdout, "\noutput> Invalid command\ninput> ");
    } else {
        // Valid command
        *n = (10 * (str[5] - '0')) + (str[6] - '0');
        switch (str[4]) {
            case 'N' : 
                *type = kNorth;
                fprintf(stdout, "\noutput> Moved north %d steps\ninput> ", *n);
                break;
            case 'S' : 
                *type = kSouth;
                fprintf(stdout, "\noutput> Moved south %d steps\ninput> ", *n);
                break;
            case 'E' : 
                *type = kEast; 
                fprintf(stdout, "\noutput> Moved east %d steps\ninput> ", *n);
                break;
            case 'W' : 
                *type = kWest; 
                fprintf(stdout, "\noutput> Moved west %d steps\ninput> ", *n);
                break;
            default:
                // Should not reach here
                *type = kInvalid; 
                fprintf(stdout, "\noutput> Invalid command\ninput> ");
                break;
        }
    }
}

#endif // __APP_COMMAND__