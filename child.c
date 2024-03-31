#include "includes/include.h"

unsigned energy;
unsigned player_number;
unsigned next_player_number;
#define A 5
#define K 2
#define RAND 14

double short_pause_duration();

int main(int argc, char* argv[]) {

    // TODO: if argc != numberOfArgs.. 

    srand(getpid());

    char* arguments=malloc(50 * sizeof(char));

    for (int i = 1; i < argc; i++) {
        
        strcat(arguments, "  ");
        strcat(arguments, argv[i]); 
     }

        //printf("Child process with PID %d has args: %s\n", getpid(), arguments);


    energy = atoi(argv[2]);
    player_number = atoi(argv[1]);
    next_player_number = atoi(argv[3]);
    printf("short pause duration for child %d is %f and energy is %d\n", getpid(),short_pause_duration(), energy);

    return 0;
}


double short_pause_duration() {
    //returns a short pause duration in milliseconds
    int random_constant = 10 + rand() % 11; // number between 10 and 20
    return 50000*((double)A / pow((double)(energy + random_constant), (double)K));
}
