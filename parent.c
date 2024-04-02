#include "includes/include.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/stat.h>
#include <fcntl.h>

pid_t process_pid[2*PLAYERS_PER_TEAM]; 
unsigned player_energy[2*PLAYERS_PER_TEAM]; 
unsigned next_player; /* each process will have a different next player pid,
                       this is used to index the pids array and send the pid to the process when we use execlp. arg[3] */ 

int status, i;

int number_of_processes_waiting_for_read = 0;
//create structs 
team team1;
team team2;
//init the two structs to 0 balls and 0 score

void create_FIFOs();
void assign_initial_energy();
void fork_children();
void init_teams();

int current_round_number = 0;

bool time_up = false;


void signal_handler(int signum) {

    if (signum == SIGUSR1) {

        number_of_processes_waiting_for_read++;
    }


    if (signum == SIGIO) {

        printf("entrered SIGIO from parent\n");

    }




    fflush(stdout);
}


void doOneRound();

void alarm_handler(int signum) {

    time_up = true;
    
    if (team1.number_of_balls < team2.number_of_balls) {
        team1.total_score++;

    }

    else if (team1.number_of_balls > team2.number_of_balls) {
        team2.total_score++;
    }


    // let all children ignore SIGUSR1 and SIGUSR2 signals

    for (int i = 0; i < 2*PLAYERS_PER_TEAM; i++) {
        kill(process_pid[i], SIGCHLD);
    }

    alarm(0);
}

int main() {

    //create the FIFOs
    create_FIFOs();
    fork_children();
    init_teams();

    struct sigaction sa_chld, sa_io, sa_alarm;

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


    // add the alarm handler
    sa_alarm.sa_handler = alarm_handler;
    sigemptyset(&sa_alarm.sa_mask);
    sa_alarm.sa_flags = 0;
    if (sigaction(SIGALRM, &sa_alarm, NULL) == -1) {
        perror("sigaction for SIGALRM");
        exit(EXIT_FAILURE);
    }

    int fd = open(FIFO1, O_WRONLY);

    if (fd == -1) {
        perror("Error opening FIFO1");
        exit(-1);
    }

    int arr[2]  = {process_pid[5], process_pid[6]};


    if (write(fd, arr, sizeof(int) * 2) == -1) {
        perror("Error writing to FIFOR1");
        exit(-1);
    }

    close(fd);


    fd = open(FIFO2, O_WRONLY);

    if (fd == -1) {
        perror("Error opening FIFO2");
        exit(-1);
    }

    if (write(fd, &process_pid[0], sizeof(pid_t)) == -1) {
        perror("Error writing to FIFO2");
        exit(-1);
    }

    close(fd);

    sleep(2);
    //sending the signal to both team leads

    alarm(ROUND_DURATION); // set the alarm for the round duration.
                           //
    do {

        
        
        // do one round of the game

        doOneRound();
        



    } while(current_round_number < 1 /*MAX_NUMBER_OF_ROUNDS*/);


    
    
    return 0;
}



/* FUNCTIONS */
void assign_initial_energy() {

    for (int i = 0; i < 2*PLAYERS_PER_TEAM; i++) {

        player_energy[i] =  MIN_PLAYER_ENERGY + (rand() % (abs(MAX_PLAYER_ENERGY - MIN_PLAYER_ENERGY)));
    }
}

void init_teams() {
    team1.number_of_balls = 0;
    team1.total_score = 0;
    team2.number_of_balls = 0;
    team2.total_score = 0;
    team1.total_rounds_won = 0;
    team2.total_rounds_won = 0;
}

void create_FIFOs()
{

    // CAUTION:
    // If keeping the FIFOs causes problems, we might need to remove the FIFOS if they exist
    // 
    //
    //
    // if the FIFO exists, no problem.
    if ((mkfifo(FIFO1, S_IFIFO | 0777)) == -1 && errno != EEXIST)
    {
        perror("Error Creating Fifo");
        exit(-1);
    }

    // if the FIFO exists, no problem.
    if ((mkfifo(FIFO2, S_IFIFO | 0777)) == -1 && errno != EEXIST)
    {
        perror("Error Creating Fifo");
        exit(-1);
    }
}


void doOneRound() {


    // when a new round starts, children should ignore the previous signals. 
    // To do this, we need to send a signal to to all children to tell them to ignore the SIGUSR1 and SIGUSR2 signals.

   // for (int i = 0; i < 2*PLAYERS_PER_TEAM; i++) {
     //   kill(process_pid[i], SIGCHLD);
   // }

    if (team1.number_of_balls == 0) {
    
        // send a ball to team1 leader (send a signal to the team1 leader)

        team1.number_of_balls++;
        kill(process_pid[5], SIGUSR1);

    }

    if (team2.number_of_balls == 0) {
    
        // send a ball to team2 leader (send a signal to the team2 leader)

        team2.number_of_balls++;
        kill(process_pid[11], SIGUSR1);

    }


}

void fork_children(){

    pid_t team1_lead_pid;
    pid_t team2_lead_pid;
    pid_t next_player_pid;


    char team1_leader_pid_arg[10]="-1";
    char team2_leader_pid_arg[10]="-1";

    char next_player_pid_arg[10]="-1";

    char next_pid_arg[10] = "-1";


    for (i = 2*PLAYERS_PER_TEAM-1; i >= 0; i--) {

        //we will fork a child and execute the child program
        int current_pid;

        if ((current_pid = fork()) == -1) {
            perror("Fork failed.\n");
            exit(1);
        }

        char player_number_arg[4];
        char energy_arg[20];
        char next_player_arg[20]; 

        srand(time(NULL)); 
        assign_initial_energy();

        if (i == 5) {

            team1_lead_pid = current_pid;
            sprintf(team1_leader_pid_arg, "%d", team1_lead_pid);
        }

        else if (i == 11) { 

            team2_lead_pid = current_pid;
            sprintf(team2_leader_pid_arg, "%d", team2_lead_pid);
        }


        if (current_pid == 0) {
            sprintf(player_number_arg, "%d", i);

            sprintf(energy_arg, "%d", player_energy[i]);
            // this is unlikely to be incorrect, but feel free to triple check it, since it can cause serious damage.
            next_player = (i < PLAYERS_PER_TEAM) ? ((i+1) % PLAYERS_PER_TEAM) : 6 + ((i+1) % PLAYERS_PER_TEAM);
            
            sprintf(next_player_arg, "%d",next_player);
            sprintf(next_player_pid_arg, "%d", process_pid[next_player]);

            execlp("./child" ,"child.o",player_number_arg, energy_arg, next_player_arg, team1_leader_pid_arg, team2_leader_pid_arg,next_player_pid_arg, (const char*)NULL);

            perror("execvp failed.\n");
            
            // To kill all child processes (from stack-overflow), might need to be tested. 

            /*
               One way to accomplish this is to deliver some signal that can be caught (not
               SIGKILL). Then, install a signal handler that detects if the current process is
               the parent process or not, and calls _exit() if it is not the parent process.(ibid, et al).
            */

            signal(SIGQUIT, SIG_IGN);
            kill(0, SIGQUIT);
            exit(2);

        } else {            
            process_pid[i] = current_pid;  
        }
    }    
}
