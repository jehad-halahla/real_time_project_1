#include "includes/include.h"
#include <stdio.h>

unsigned energy;
unsigned player_number;
unsigned next_player_number;
#define A 5
#define K 2
#define RAND 14
pid_t pid_of_team_leader;

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
    pid_of_team_leader = atoi(argv[4]);

    printf("%d is the pid of the team leader\n", pid_of_team_leader);

    energy = atoi(argv[2]);
    player_number = atoi(argv[1]);
    next_player_number = atoi(argv[3]);

    //    printf("short pause duration for child %d is %f and energy is %d\n", getpid(),short_pause_duration(), energy);

    int fd;
    int arr[2];

    if (player_number == 0) {


        if ((fd = open(FIFO1, O_RDONLY)) == -1) {
            perror("error in openning FIFO");
            exit(2);
        }

        if (read(fd, arr, sizeof(int) * 2) == -1) {
            perror("an error happend in writing to the fifo file\n");
            exit(3);
        }

        for (int i = 0; i < 2; i++) {
            printf("%d ", arr[i]);
        }


        close(fd);

    }




    return 0;
}


double short_pause_duration() {
    //returns a short pause duration in milliseconds
    int random_constant = 10 + rand() % 11; // number between 10 and 20
    return 50000*((double)A / pow((double)(energy + random_constant), (double)K));
}
