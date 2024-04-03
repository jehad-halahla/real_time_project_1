#include "includes/include.h"
#include <bits/types/sigset_t.h>
#include <stdio.h>
#include <unistd.h>

// include sigignore()
#include <signal.h>
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

pid_t this_team_leader_pid;
pid_t other_team_leader_pid;
void send_ball_to_next_player();

bool ignore_usr1 = false;
bool ignore_usr2 = false;

struct sigaction sa_usr1, sa_usr2, sa_chld, sa_io, ignore_action, empty_action;

int sigchild_number = 0;

void signal_handler(int signum) {

    // only team leaders can receive SIGUSR1, this way they know they have to send the ball to the next player in the team
    // next team a team leader receives the ball, it receives the signal on SIGUSR2 and sends the ball to the other team leader
    if (signum == SIGUSR1) {

        if (ignore_usr1) {
            return;
        }

        if (player_number != 10 && player_number != 4) { //all players except player 10 and 4 

            printf("sending ball %d(%d) -> %d(%d)\n",getpid(),player_number , next_player_pid, next_player_number);
            fflush(stdout);
            sleep(1);
            kill(next_player_pid, SIGUSR1);

        }

        else {
            //for players 10 and 4 to send back to team leads
            printf("sending ball %d(%d) -> %d(%d) --> using signal SIGUSR2\n",getpid(),player_number , next_player_pid, next_player_number);
            fflush(stdout);
            sleep(1);

            kill(next_player_pid, SIGUSR2); //here 11 recieves the ball from 10 and sends it to 5 , 4 recieves the ball from 5 and sends it to 11
        }

    }

    else if (signum == SIGUSR2) {

        if (ignore_usr2) {
            return;
        }

        // only players 11 ,5  receive this signal (for now, testing purposes)
        if(player_number == 11){
            sleep(1);
            printf("sending ball %d(%d) -> %d(%d)\n",getpid(),player_number , pid_of_team1_leader, 5);

            kill(getppid(), SIGUSR1); // send a signal to the parent process to change count of balls

            kill(pid_of_team1_leader, SIGUSR1);
        }
        else{ //team 1 leader
            sleep(1);
            printf("sending ball %d(%d) -> %d(%d)\n",getpid(),player_number , pid_of_team2_leader, 11);

            kill(getppid(), SIGUSR2); // send a signal to the parent process to change count of balls
                                      //
            kill(pid_of_team2_leader, SIGUSR1);

        }
       // kill(pid_of_team1_leader, SIGQUIT);
        fflush(stdout);
    }
        

    else if (signum == SIGCHLD) {

        sigignore(SIGUSR1);
        sigignore(SIGUSR2);

        ignore_usr1 = true;
        ignore_usr2 = true;

        usleep(0.5 * 1000000);

        sigaction(SIGUSR1, &sa_usr1, NULL);
        sigaction(SIGUSR2, &sa_usr2, NULL);

        ignore_usr1 = false;
        ignore_usr2 = false;

        // we should use sigaction to ignore the signals

        /*

        if (sigaction(SIGUSR1, &ignore_action, NULL) == -1) {
            perror("sigaction for SIGUSR1");
            exit(EXIT_FAILURE);
        }

        if (sigaction(SIGUSR2, &ignore_action, NULL) == -1) {
            perror("sigaction for SIGUSR2");
            exit(EXIT_FAILURE);
        }

        */

        pause();
    }


    else if (signum == SIGIO) {

        printf("entrered SIGIO from child\n");

    }

    fflush(stdout);

}


void dummy_handler(int signum);


int main(int argc, char* argv[]) {

    // Set the handler to ignore
    ignore_action.sa_handler = SIG_IGN;
    sigemptyset(&ignore_action.sa_mask);
    ignore_action.sa_flags = 0;

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
   //  printf("Arguments: %s\n", arguments);

    player_number = atoi(argv[1]);
    energy = atoi(argv[2]);
    next_player_number = atoi(argv[3]);
    pid_of_team1_leader = atoi(argv[4]);
    pid_of_team2_leader = atoi(argv[5]);
    next_player_pid = atoi(argv[6]); 


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

    empty_action.sa_handler = dummy_handler;
    sigemptyset(&empty_action.sa_mask);
    empty_action.sa_flags = 0;


    // normalizing variables for each player
    this_team_leader_pid = (player_number <= 5) ? pid_of_team1_leader : pid_of_team2_leader;
    other_team_leader_pid = (player_number <= 5) ? pid_of_team2_leader : pid_of_team1_leader;

    if (player_number == 5) {

        sleep(1);
        int fd = open(FIFO2, O_RDONLY);
        if (fd == -1) {
            perror("Error opening FIFO2");
            exit(-1);
        }

        if (read(fd, &next_player_pid, sizeof(pid_t)) == -1) {
            perror("Error reading from FIFO2");
            exit(-1);
        }


        close(fd);
        
        printf("%d is the pid of next player from process 5\n", next_player_pid);

    }

    if (player_number == 11) {

        int fd = open(FIFO1, O_RDONLY);

        int arr[2];

        if (fd == -1) {
            perror("Error opening FIFO1");
            exit(-1);
        }

        if (read(fd, arr, 2*sizeof(pid_t)) == -1) {
            perror("Error reading from FIFO1");
            exit(-1);
        }

        pid_of_team1_leader = arr[0];
        next_player_pid = arr[1];

        close(fd);

         printf("%d is the pid of team1 leader from process 11\n", pid_of_team1_leader);
         printf("%d is the pid of next player from process 11\n", next_player_pid);
    }

    while (1) {
        
        pause();
    }

    return 0;
}


double short_pause_duration() {
    //returns a short pause duration in milliseconds
    int random_constant = 10 + rand() % 11; // number between 10 and 20
    return 50000*((double)A / pow((double)(energy + random_constant), (double)K));
}

void send_ball_to_next_player() {
    

    if (player_number < 5) {

        printf("Sending: %d(%d) -> %d(%d)\n",getpid(), player_number,next_player_pid ,next_player_number);
        kill(next_player_pid, SIGUSR2);

    }
    
    else  if (player_number == 5) {

        printf("Sending: %d(%d) -> %d(%d)\n",getpid(), player_number,other_team_leader_pid ,next_player_number);
        kill(other_team_leader_pid, SIGUSR1);
    
    }
   

    if (player_number == 10) {
        
        printf("Sending: %d(%d) -> %d(%d)\n",getpid(), player_number,other_team_leader_pid ,next_player_number);
        kill(pid_of_team1_leader, SIGUSR1);
    }

    else if (player_number > 5 && player_number < 11) {

        printf("Sending: %d(%d) -> %d(%d)\n",getpid(), player_number,next_player_pid ,next_player_number);
        kill(next_player_pid, SIGUSR2);
    
    }


}


void dummy_handler(int signum) {
    // do nothing
    return;
}
