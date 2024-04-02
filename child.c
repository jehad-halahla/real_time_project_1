#include "includes/include.h"
#include <bits/types/sigset_t.h>
#include <stdio.h>
#include <unistd.h>

unsigned energy;
unsigned player_number;
unsigned next_player_number;
#define A 5
#define K 2
#define RAND 14
pid_t pid_of_team1_leader;
pid_t pid_of_team2_leader;
pid_t next_player_pid;
double short_pause_duration();


void signal_handler(int signum) {

    if (signum == SIGUSR1) {

        
        
        sleep(1);
        
        if (player_number != 10) {

            printf("sending ball %d -> %d\n",getpid(), next_player_pid);
            fflush(stdout);
            kill(next_player_pid, SIGUSR1);

        }

        else {

            printf("sending ball %d -> %d\n",getpid(), next_player_pid);
            fflush(stdout);
            kill(next_player_pid, SIGUSR2);
        }

    }

    if (signum == SIGUSR2) {

        // only player 11 receives this signal (for now, testing purposes)

        sleep(1);

        printf("sending ball %d -> %d\n",player_number, pid_of_team1_leader);

        kill(0, SIGQUIT);
        fflush(stdout);

        kill(pid_of_team1_leader, SIGUSR1);
    }
}

int main(int argc, char* argv[]) {


    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);

    // TODO: if argc != numberOfArgs.. 

    srand(getpid());

    char* arguments=malloc(50 * sizeof(char));

    for (int i = 1; i < argc; i++) {

        strcat(arguments, " ");
        strcat(arguments, argv[i]); 
    }

    char player_pid[10];
    sprintf(player_pid, "%d", getpid());
    strcat(arguments, " ");
    strcat(arguments, player_pid);
    printf("Arguments: %s\n", arguments);

    player_number = atoi(argv[1]);
    energy = atoi(argv[2]);
    next_player_number = atoi(argv[3]);
    pid_of_team1_leader = atoi(argv[4]);
    pid_of_team2_leader = atoi(argv[5]);
    next_player_pid = atoi(argv[6]); 


    if (player_number == 11) {

        int fd = open(FIFO1, O_RDONLY);

        if (fd == -1) {
            perror("Error opening FIFO1");
            exit(-1);
        }

        if (read(fd, &pid_of_team1_leader, sizeof(pid_t)) == -1) {
            perror("Error reading from FIFO1");
            exit(-1);
        }

        kill(getppid(),SIGCHLD);

        close(fd);

        printf("%d is the pid of team1 leader from process 11\n", pid_of_team1_leader);
    }

    if (player_number == 5) {

        int fd = open(FIFO1, O_RDONLY);
        if (fd == -1) {
            perror("Error opening FIFO1");
            exit(-1);
        }

        if (read(fd, &next_player_pid, sizeof(pid_t)) == -1) {
            perror("Error reading from FIFO1");
            exit(-1);
        }

        close(fd);

    }

    pause();

    return 0;
}


double short_pause_duration() {
    //returns a short pause duration in milliseconds
    int random_constant = 10 + rand() % 11; // number between 10 and 20
    return 50000*((double)A / pow((double)(energy + random_constant), (double)K));
}


