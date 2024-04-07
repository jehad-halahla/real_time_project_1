#ifndef __INCLUDE_H_
#define __INCLUDE_H_

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <bits/types/sigset_t.h>
#include "functions.c"

#define PLAYERS_PER_TEAM 6

typedef struct player {
     pid_t pid;
     unsigned int energy;
    
} player;

typedef struct team {
    unsigned int number_of_balls;
    unsigned int total_score; // incremented each round when balls are less than another team's balls.
                            
} team;

#endif


#ifndef MAX_PLAYER_ENERGY
#define MAX_PLAYER_ENERGY 100 // test value, might change later.
#endif


#ifndef MIN_PLAYER_ENERGY
#define MIN_PLAYER_ENERGY 50 // test value, might change later.
#endif

#define MAX_NUM_BALLS 10  // Number of balls

#define FIFO1 "/tmp/FIFO1" //for team 1
#define FIFO2 "/tmp/FIFO2" // for team 2
#define GUI_FIFO "/tmp/FIFO_GUI" // for the gui process

#define SIGUI 7

#define TEAM1_LEADER 5
#define TEAM2_LEADER 11


#define SPEED 0.01f
