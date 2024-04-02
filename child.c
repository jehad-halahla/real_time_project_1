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

        
        
        
        if (player_number != 10) {

            printf("sending ball %d(%d) -> %d(%d)\n",getpid(),player_number , next_player_pid, next_player_number);
            fflush(stdout);
            kill(next_player_pid, SIGUSR1);

        }

        else {

            printf("sending ball %d(%d) -> %d(%d)\n",getpid(),player_number , next_player_pid, next_player_number);
            fflush(stdout);
            kill(next_player_pid, SIGUSR2);
        }

    }

    else if (signum == SIGUSR2) {

        // only player 11 receives this signal (for now, testing purposes)


        printf("sending ball %d(%d) -> %d(%d)\n",getpid(),player_number , pid_of_team1_leader, 5);

        kill(0, SIGQUIT);
        fflush(stdout);

        kill(pid_of_team1_leader, SIGUSR1);
    }

    else if (signum == SIGCHLD) {

        printf("entrered SIGCHLD from child\n");

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


            close(fd);
            kill(getppid(),SIGCHLD);

            printf("%d is the pid of team1 leader from process 11\n", pid_of_team1_leader);
        }

    }


    else if (signum == SIGIO) {

        printf("entrered SIGIO from child\n");


        if (player_number == 5) {

            sleep(1);
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
            kill(getppid(),SIGIO);

        }

    }
}

int main(int argc, char* argv[]) {





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

    struct sigaction sa_usr1, sa_usr2, sa_chld, sa_io;

    // Set up SIGUSR1 handler
    sa_usr1.sa_handler = signal_handler;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("sigaction for SIGUSR1");
        exit(EXIT_FAILURE);
    }

    // Set up SIGUSR2 handler
    sa_usr2.sa_handler = signal_handler;
    sigemptyset(&sa_usr2.sa_mask);
    sa_usr2.sa_flags = 0;
    if (sigaction(SIGUSR2, &sa_usr2, NULL) == -1) {
        perror("sigaction for SIGUSR2");
        exit(EXIT_FAILURE);
    }

    // Set up SIGCHLD handler
    sa_chld.sa_handler = signal_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = 0;
    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        perror("sigaction for SIGCHLD");
        exit(EXIT_FAILURE);
    }

    // Set up SIGIO handler
    sa_io.sa_handler = signal_handler;
    sigemptyset(&sa_io.sa_mask);
    sa_io.sa_flags = 0;
    if (sigaction(SIGIO, &sa_io, NULL) == -1) {
        perror("sigaction for SIGIO");
        exit(EXIT_FAILURE);
    }


    pause();

    return 0;
}


double short_pause_duration() {
    //returns a short pause duration in milliseconds
    int random_constant = 10 + rand() % 11; // number between 10 and 20
    return 50000*((double)A / pow((double)(energy + random_constant), (double)K));
}


