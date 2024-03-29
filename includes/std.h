#ifndef __STD_H_
#define __STD_H_

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#include <stdio.h>


void red_stderr()
{
    fprintf(stderr, ANSI_COLOR_RED);
    fflush(stderr);
}
void reset_stderr()
{
    fprintf(stderr, ANSI_COLOR_RESET);
    fflush(stderr);
}

void red_stdout()
{
    printf(ANSI_COLOR_RED);
}
void green_stdout()
{
    printf(ANSI_COLOR_GREEN);
}
void yellow_stdout()
{
    printf(ANSI_COLOR_YELLOW);
}
void blue_stdout()
{
    printf(ANSI_COLOR_BLUE);
}
void magenta_stdout()
{
    printf(ANSI_COLOR_MAGENTA);
}
void cyan_stdout()
{
    printf(ANSI_COLOR_CYAN);
}
void reset_stdout()
{
    printf(ANSI_COLOR_RESET);
    fflush(stdout);
}
#endif