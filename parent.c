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


//create structs 
team team1;
team team2;
//init the two structs to 0 balls and 0 score

void create_FIFOs();
void assign_initial_energy();
void fork_children();
void init_teams();

int main() {
    //create the FIFOs
    create_FIFOs();
    fork_children();
    init_teams();
    int fd;
    // if a team does not have a ball, the parent process will pass a ball to its team leader.
    // 
    //
    //

    int arr[2] = {2,3};
    
    if ((fd = open(FIFO1, O_WRONLY)) == -1) {
        perror("error in FIFO open");
    }

    if ((write(fd, arr, 2*sizeof(int))) == -1) {

        perror("error in writing");
    }
    close(fd);
    
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

void fork_children(){

    pid_t team1_lead_pid;
    pid_t team2_lead_pid;

    char team1_leader_pid_arg[10];
    char team2_leader_pid_arg[10];

    pid_t next_player_pid;
    char next_player_pid_arg[10];

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

        // printf("%d is the pid of team leader in parent code\n", (i <= 5) ? team1_lead_pid : team2_lead_pid);

        // sprintf(next_player_arg, "%d",  :  process_pid[i+1]);

        if (current_pid == 0) {

            sprintf(player_number_arg, "%d", i);
            sprintf(energy_arg, "%d", player_energy[i]);
            // this is unlikely to be incorrect, but feel free to triple check it, since it can cause serious damage.
            next_player = (i < PLAYERS_PER_TEAM) ? ((i+1) % PLAYERS_PER_TEAM) : 6 + ((i+1) % PLAYERS_PER_TEAM);
            sprintf(next_player_arg, "%d",next_player);

            execlp("./child" ,"child.o",player_number_arg, energy_arg, next_player_arg,(i <= 5) ? team1_leader_pid_arg : team2_leader_pid_arg,/*current_pid_arg,*/ (const char*)NULL);

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

