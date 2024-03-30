#ifndef __INCLUDE_H_
#define __INCLUDE_H_

#include <stdio.h>
#include <sys/types.h>
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

#define PLAYERS_PER_TEAM 6

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

