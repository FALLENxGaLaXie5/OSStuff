/*
Joshua Steward
9/29/16
Lab 5 Task 2
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

int count = 10000;

typedef enum {
    PRIORITY_HIGH = 99,//round robin scheduler
    PRIORITY_MEDIUM = 50,//first come first serve scheduler
    PRIORITY_LOW = 1//default(OTHER) scheduler
} PRIORITY;


void *runner(void *param)
{
    PRIORITY *priority = param;
    int count;
    for (count = 100; count > 0; --count)
    {

    }
    pthread_t self = pthread_self();

    if (*(priority) == PRIORITY_HIGH)
    {
        printf("My HIGH Priority Thread TID is: %ld\n", self);
    }
    else if (*(priority) == PRIORITY_MEDIUM)
    {
        printf("My MEDIUM Priority Thread TID is: %ld\n", self);
    }
    else if (*(priority) == PRIORITY_LOW)
    {
        printf("My LOW Priority Thread TID is: %ld\n", self);
    }
    else
    {
        printf("Error");
    }
    // skipping manual cleaning here, since the process exits anyway

    free(priority);
    return 0;
}

int main(int argc, char *argv[])
{
    int numOfEachThread = 3;
    while (getchar() != 'q')
    {
        int i;
        for (i = 0; i < numOfEachThread; ++i)
        {
            PRIORITY myPriority = PRIORITY_HIGH;
            struct sched_param schedParam;
            schedParam.sched_priority = myPriority;

            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
            pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
            pthread_attr_setschedpolicy(&attr, SCHED_RR);
            pthread_attr_setschedparam(&attr, &schedParam);
            pthread_create(&tid, &attr, &runner, (void *)myPriority);
            pthread_join(tid, NULL);
            pthread_attr_destroy(&attr);
        }
        for (i = 0; i < numOfEachThread; ++i)
        {
            PRIORITY myPriority = PRIORITY_MEDIUM;
            struct sched_param schedParam;
            schedParam.sched_priority = myPriority;


            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
            pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
            pthread_attr_setschedpolicy(&attr, SCHED_RR);
            pthread_attr_setschedparam(&attr, &schedParam);
            pthread_create(&tid, &attr, &runner, (void *)myPriority);
            pthread_join(tid, NULL);
            pthread_attr_destroy(&attr);
        }
        for (i = 0; i < numOfEachThread; ++i)
        {
            PRIORITY myPriority = PRIORITY_LOW;
            struct sched_param schedParam;
            schedParam.sched_priority = myPriority;

            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
            pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
            pthread_attr_setschedpolicy(&attr, SCHED_RR);
            pthread_attr_setschedparam(&attr, &schedParam);
            pthread_create(&tid, &attr, &runner, (void *)myPriority);
            pthread_join(tid, NULL);
            pthread_attr_destroy(&attr);
        }
        sleep(1);
    }
    exit(0);
}