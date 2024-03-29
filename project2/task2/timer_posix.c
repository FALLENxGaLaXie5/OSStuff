#include <stdio.h>
#include <time.h>
#include <signal.h>
#include "disk.h"

#define COUNTDOWN_VALUE 10

timer_t gTimerid;
int count = COUNTDOWN_VALUE;

void start_timer(void)
{
    struct itimerspec value;

    value.it_value.tv_sec = 1;
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = 1;
    value.it_interval.tv_nsec = 0;

    timer_create (CLOCK_REALTIME, NULL, &gTimerid);

    timer_settime (gTimerid, 0, &value, NULL);

}

void stop_timer(void)
{
    struct itimerspec value;

    value.it_value.tv_sec = 0;
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;

    timer_settime (gTimerid, 0, &value, NULL);
}

void timer_callback(int sig)
{
    srand(time(NULL));

    printf("Caught timer signal: %d ... !!\n", sig);
    count--;
}

/**
int main(int ac, char **av)
{
    (void) signal(SIGALRM, timer_callback);
    start_timer();
    while(count >= 0);
}
 */
