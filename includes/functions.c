#ifndef time_h
#include <time.h>
#endif

#ifndef errno_h
#include <errno.h>
#endif

void my_pause(double seconds){
    struct timespec ts, rem;
    ts.tv_sec = (time_t) seconds;
    ts.tv_nsec = (long) ((seconds - ts.tv_sec) * 1e9);

    while (nanosleep(&ts, &rem) == -1 && errno == EINTR) {
        ts = rem;
    }
}
