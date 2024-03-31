#include "includes/include.h"
#include <stdlib.h>
#include <string.h>

unsigned energy;
unsigned player_number;
unsigned next_player_number;
#define A 5
#define K 5
#define RAND 14

int short_pause_duration();

int main(int argc, char* argv[]) {

    // TODO: if argc != numberOfArgs.. 

    char* arguments=malloc(50 * sizeof(char));

    for (int i = 1; i < argc; i++) {
        
        strcat(arguments, "  ");
        strcat(arguments, argv[i]); 
     }

        printf("Child process with PID %d has args: %s\n", getpid(), arguments);


    energy = atoi(argv[2]);
    player_number = atoi(argv[1]);
    next_player_number = atoi(argv[3]);
    

    return 0;
}


int short_pause_duration() {

    return A / pow((double)(energy + RAND), (double)K);
}
