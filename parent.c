#include "includes/include.h"
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
    
    for (i = 0; i < 2 * PLAYERS_PER_TEAM; i++) {
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

        if (current_pid == 0) {

            sprintf(player_number_arg, "%d", i);
            sprintf(energy_arg, "%d", player_energy[i]);
            // this is unlikely to be incorrect, but feel free to triple check it, since it can cause serious damage.
            next_player = (i < PLAYERS_PER_TEAM) ? ((i+1) % PLAYERS_PER_TEAM) : 6 + ((i+1) % PLAYERS_PER_TEAM);
            sprintf(next_player_arg, "%d",next_player);

            execlp("./child" ,"child.o",player_number_arg, energy_arg, next_player_arg, (const char*)NULL) ;

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

