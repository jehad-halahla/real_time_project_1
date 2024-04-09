#include "includes/include.h"
#include "includes/std.h"
#include <asm-generic/signal-defs.h>

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
bool player_drops_ball();
int gui_pid;

pid_t this_team_leader_pid;
pid_t other_team_leader_pid;
void send_ball_to_next_player();

bool ignore_usr1_usr2 = false;

struct sigaction sa_usr1, sa_usr2, sa_chld, sa_io, ignore_action, empty_action;

int sigchild_number = 0;

//bool next_round_started = true;

union sigval value;

int fd_shm;
// Define a structure to hold the flag
struct shared_data {
    int ignore_signals; // 0 means not ignore, 1 means ignore
};

struct shared_data *shared_mem;

void open_shared_mem();
void send_ball(int next_player_pid, int signum, int next_player_number); 

void signal_handler_usr1(int signum) {


    // only team leaders can receive SIGUSR1, this way they know they have to send the ball to the next player in the team
    // next team a team leader receives the ball, it receives the signal on SIGUSR2 and sends the ball to the other team leader
    if (signum == SIGUSR1) {

        if (shared_mem->ignore_signals == 1) {
            return;
        }

        if (player_number != 10 && player_number != 4) { //all players except player 10 and 4 


            energy = (energy > 20) ? energy - 5 : 20;
            send_ball(next_player_pid, SIGUSR1, next_player_number);
        }

        else {
            energy = (energy > 20) ? energy - 5 : 20;
            send_ball(next_player_pid, SIGUSR2, next_player_number);
        }

    }

    fflush(stdout);

}


void signal_handler_usr2 (int signum) {

    if (signum == SIGUSR2) {

        if (shared_mem->ignore_signals == 1) {
            return;
        }

        // only players 11 ,5  receive this signal (for now, testing purposes)
        if(player_number == 11){
            energy = (energy > 20) ? energy - 5 : 20;
            send_ball(pid_of_team1_leader, SIGUSR1, TEAM1_LEADER);
            kill(getppid(), SIGUSR1); // send a signal to the parent process to change count of balls
        }

        else { //team 1 leader
            energy = (energy > 20) ? energy - 5 : 20;
            send_ball(pid_of_team2_leader, SIGUSR1, TEAM2_LEADER);
            kill(getppid(), SIGUSR2); // send a signal to the parent process to change count of balls
        }
    }

    fflush(stdout);
}

void signal_handler_sigchild(int signum) {


    if (signum == SIGCHLD) {

        sigset_t signal_set;
        int sig;

        // Create a sinnal set containing SIGUSR1 and SIGUSR2
        sigemptyset(&signal_set);
        sigaddset(&signal_set, SIGUSR1);
        sigaddset(&signal_set, SIGUSR2);
        sigaddset(&signal_set, SIGCHLD); // the signal that should unblock the process.

        // Block SIGUSR1 and SIGUSR2
        sigprocmask(SIG_BLOCK, &signal_set, NULL);

        // wait for a SIGCHILD signal only

        // wait for SIGUSR1 or SIGUSR2
        sigwait(&signal_set, &sig);
    }

    fflush(stdout);
}


void dummy_handler(int signum);


int main(int argc, char* argv[]) {
    
    open_shared_mem();

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
    gui_pid = atoi(argv[7]);


    // Set up SIGUSR1 handler
    sa_usr1.sa_handler = signal_handler_usr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("sigaction for SIGUSR1");
        exit(EXIT_FAILURE);
    }

    // Set up SIGUSR2 handler
    sa_usr2.sa_handler = signal_handler_usr2;
    sigemptyset(&sa_usr2.sa_mask);
    sa_usr2.sa_flags = 0;
    if (sigaction(SIGUSR2, &sa_usr2, NULL) == -1) {
        perror("sigaction for SIGUSR2");
        exit(EXIT_FAILURE);
    }

    // Set up SIGCHLD handler
    sa_chld.sa_handler = signal_handler_sigchild;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = 0;
    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        perror("sigaction for SIGCHLD");
        exit(EXIT_FAILURE);
    }

    // normalizing variables for each player
    this_team_leader_pid = (player_number <= 5) ? pid_of_team1_leader : pid_of_team2_leader;
    other_team_leader_pid = (player_number <= 5) ? pid_of_team2_leader : pid_of_team1_leader;

    if (player_number == 5) {

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

    }

    while (1) {
        
        pause();
    }

    return 0;
}


double short_pause_duration() {
    //returns a short pause duration in milliseconds
    int random_constant = 17 + rand() % 4; // number between 17 and 20
    return 7500*((double)A / pow((double)(energy + random_constant), (double)K));
}


void open_shared_mem() {

    // Open the shared memory segment
    fd_shm = shm_open("/my_shared_memory", O_RDONLY, 0666);
    if (fd_shm == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    // Map the shared memory segment into the address space
    shared_mem = mmap(NULL, sizeof(struct shared_data), PROT_READ, MAP_SHARED, fd_shm, 0);
    if (shared_mem == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    close(fd_shm);
}


void send_ball(int next_player_pid, int signum, int next_player_number) {

    my_pause(short_pause_duration());

    #ifdef __GUI__
    usleep(1000);

    if (player_number == 5 && next_player_number == 1) {
        value.sival_int = 511;
    }
    else if (player_number == 11 && next_player_number == 5) {

        value.sival_int = 115;
    }
    else if (player_number == 10 && next_player_number == 11) {
        value.sival_int = 1011;
    }

    else if (player_number == 9 && next_player_number == 10) {
        value.sival_int = 910;
    }

    else {
        value.sival_int = player_number * 10 + next_player_number;
    }
    usleep(1000);

    sigqueue(gui_pid, SIGUSR1, value);

    #endif

    printf("sending ball %d(%d) -> %d(%d), remaining energy is: %d\n", getpid(), player_number, next_player_pid, next_player_number,energy);
    fflush(stdout);
    if (player_drops_ball()) {
        red_stdout();
        printf("Player %d dropped the ball\n", player_number);
        printf("resending ball %d(%d) -> %d(%d), remaining energy is: %d\n", getpid(), player_number, next_player_pid, next_player_number,energy);
        reset_stdout();
        fflush(stdout);
        energy = (energy > 20) ? energy - 3 : 20;
        my_pause(short_pause_duration());
        kill(next_player_pid, signum);
    }
    else{
        kill(next_player_pid, signum);
    }
    
}

bool player_drops_ball() {
    srand(time(NULL)); // Seed the random number generator with the current time
    int random_number = rand() % 100; // Generate a random number between 0 and 99
    // Let's say there's a 5% chance the player will drop the ball
    if (random_number < 5) {
        return true; // Player drops the ball
    } else {
        return false; // Player does not drop the ball
    }
}



