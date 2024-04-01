#include "includes/include.h"
#include <stdio.h>

unsigned energy;
unsigned player_number;
unsigned next_player_number;
#define A 5
#define K 2
#define RAND 14
pid_t pid_of_team_leader;
int players_pids[2*PLAYERS_PER_TEAM];


void signal_handler(int signum){
    //this function is for recieving the ball from previous player

    //for team leader we must distinguish where the signal came from.

    //it can come from USR1 or USR2 indicating that the ball is passed to the team leader.
}
double short_pause_duration();

int main(int argc, char* argv[]) {

    // TODO: if argc != numberOfArgs.. 

    srand(getpid());

    char* arguments=malloc(50 * sizeof(char));

    for (int i = 1; i < argc; i++) {
        
        strcat(arguments, "  ");
        strcat(arguments, argv[i]); 
     }

    pid_of_team_leader = atoi(argv[4]);
    player_number = atoi(argv[1]);
    energy = atoi(argv[2]);
    next_player_number = atoi(argv[3]);

    //printf("%d --> %d\n", player_number, next_player_number);

    int fd;

    if (player_number > -1) {

        if ((fd = open(FIFO1, O_RDONLY)) == -1) {
            perror("error in openning FIFO");
            exit(2);
        }

        if (read(fd, players_pids, sizeof(int) * 2 * PLAYERS_PER_TEAM) == -1) {
            perror("an error happend in writing to the fifo file\n");
            exit(3);
        }
        

        for (int i = 0; i < 2* PLAYERS_PER_TEAM; i++) {
            printf("%d ", players_pids[i]);
        }
        printf("\n");


        close(fd);
          
        if ((fd = open(FIFO1, O_WRONLY)) == -1) {
            perror("error in FIFO open");
        }
    
        if ((write(fd, players_pids, 2*PLAYERS_PER_TEAM*sizeof(int))) == -1) {

            perror("error in writing");
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
