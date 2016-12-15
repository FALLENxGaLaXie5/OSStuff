/*
 *  philosophers.c
 *  pthread_sync
 *
 * Joshua Steward
 * 10/11/16
 * Lab 6 Task3
 *
 *
 *  Created by AJ Bieszczad on 3/4/09.
 *  Copyright 2009 CSUCI. All rights reserved.
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define DEFAULT_NUM_OF_SEATS 5
#define DEFAULT_NUM_OF_TURNS 4

#define DELAY ( (rand() % 5 + 1) * 1000)


pthread_mutex_t *chopstick;
pthread_cond_t  *chopstick_threshold_cv;

int NUM_SEATS, numOfTurns, id;



/*NEW STUFF HERE */
/**************************/
 typedef enum state{
  THINKING, HUNGRY, EATING

 }STATE;

//pthread_cond_t self[NUM_SEATS];
//STATE states[NUM_SEATS];

void *philosopher (void *id);
void test(int id);

pthread_mutex_t monitor_mutex = PTHREAD_MUTEX_INITIALIZER;

 
void pickup(int id)
{
   
    // printf ("Philsopher no. %d gets hungry for the %d time!\n", id, i + 1);
    /*states[id] = HUNGRY;
    test(id);
    if (states[id] != EATING)
    {
        //pthread_mutex_lock(self[id]);
    }
     */
    //else
    //{
        printf ("Philsopher no. %d tries to grab chopstick %d\n", id, id);
        // pthread_mutex_lock (&(chopstick[id]));
        if(pthread_mutex_trylock((chopstick)) == 0)    {
            printf ("Philsopher no. %d has grabbed chopstick %d\n", id, id);
            printf ("Philsopher no. %d tries to grab chopstic %d\n", id, (id+1) % NUM_SEATS);
            //pthread_mutex_lock (&(chopstick[(id+1) % NUM_SEATS]));
            if(pthread_mutex_trylock((chopstick)) == 0)    {
                printf ("Philsopher no. %d grabbed chopstick %d\n", id, (id+1) % NUM_SEATS);
                //eating(id);
            }
        }
    //}

}


void eating(int id)
{
    //if (states[id] == EATING)
    //{
        printf("Philosopher no. %d is eating!\n", id);
    //}
}

void putdown(int id)
{
    //states[i] = THINKING;
    //test left and right neighbors
    //test((id+4) % 5);
    //test((id+1) % 5);
    //if (states[id] != EATING)
    //{
        printf ("Philsopher no. %d stopped eating\n", id);
        pthread_mutex_unlock (&(chopstick[id]));
        printf ("Philsopher no. %d has returned chopstick %d\n", id, id);
        pthread_mutex_unlock (&(chopstick[(id+1) % NUM_SEATS]));
        printf ("Philsopher no. %d has returned chopstick %d\n", id, (id+1) % NUM_SEATS);
    //}
}

void test(int id)
{
   /* if ((states[(id+4) % 5] != EATING) &&
            (states[id] == HUNGRY) &&
            (states[(id + 1) % 5] != EATING))
    {
        states[id] = EATING;
        pthread_mutex_unlock(self[id]);
    }

*/
}


void *philosopher (void *num)
{
   id = (long)num;
   
   printf ("Philsopher no. %d sits at the table.\n", id);
   
   // philosophers arrive at the table at various times
   usleep (DELAY);
   
   int i;
   for (i = 0; i < numOfTurns; i++)
   {

      pickup(id);
     
      // YEEEAAAAH !!!
      printf ("Philsopher no. %d eating\n", id);
      
      usleep (DELAY * 3);
      
     putdown(id);
      
      printf ("Philsopher no. %d finished turn %d\n", id, i + 1);
   }
   
   printf (">>>>>> Philsopher no. %d finished meal. <<<<<<\n", id);
   
   pthread_exit(NULL);
}



 /**********************/

int main (int argc, char **argv)
{
   if (argc < 3)
   {
      printf("Usage: philosophers <number of seats> <number of turns>");
      return 1;      
   }
   
   NUM_SEATS = atoi(argv[1]);
   numOfTurns = atoi(argv[2]);
   printf("NUMBER OF SEATS: %d\n", NUM_SEATS);
   printf("NUMBER OF TURNS: %d\n", numOfTurns);


    //initialization code
    int j;
    for (j = 0; j < NUM_SEATS; ++j)
    {
       // states[j] = THINKING;
    }


   pthread_mutex_init(chopstick,NULL);
   pthread_cond_init (chopstick_threshold_cv,NULL);
   
   chopstick = calloc(NUM_SEATS, sizeof(pthread_mutex_t));
   
   // set the see for random number generator
   srand(time(NULL));
   
   pthread_attr_t attr;
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   
   printf ("Start a dinner for %d diners\n", NUM_SEATS );
   
   pthread_t philosopher_tid[NUM_SEATS];
   
   long i;
   for (i = 0; i < NUM_SEATS; i++)
      pthread_mutex_init (chopstick + i, NULL);
   
   for (i = 0; i < NUM_SEATS; i++)
      pthread_create (&philosopher_tid[i], NULL, philosopher, (void *) i);
   
   for (i = 0; i < NUM_SEATS; i++)
      pthread_join (philosopher_tid[i], NULL);
   
   for (i = 0; i < NUM_SEATS; i++)
      pthread_mutex_destroy (chopstick + i);
   
   printf ("Dinner is no more.\n");
   
   return 0;
}



