#ifndef __INCLUDE_H_
#define __INCLUDE_H_

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>

#define PLAYERS_PER_TEAM 6

// typedef struct player{
//     unsigned int pid;
//     unsigned int energy;
//     unsigned int score;
// } player;

typedef struct team {
    unsigned int number_of_balls;
    unsigned int total_score; // incremented each round when balls are less than another team's balls.
} team;


#endif

#ifndef GAME_DURATION
#define GAME_DURATION 1
#endif

#ifndef MAX_PLAYER_ENERGY
#define MAX_PLAYER_ENERGY 40 // test value, might change later.
#endif

#ifndef MIN_PLAYER_ENERGY
#define MIN_PLAYER_ENERGY 10 // test value, might change later.
#endif

#define FIFO1 "/tmp/FIFO1" //for team 1
#define FIFO2 "/tmp/FIFO2" // for team 2


#define UISIG 7

#define TEAM1_LEADER 0 // index of the team leader of team 1 in the pids array that is maintained by the parent.
#define TEAM2_LEADER PLAYERS_PER_TEAM // index of the team leader of team 2.
                                      //
#define MAX_PAUSE_DURATION_MILLIS 1000
#define MIN_PAUSE_DURATION_MILLIS 100
