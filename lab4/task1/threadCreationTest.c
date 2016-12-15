/*
Joshua Steward
9/28/16
Lab 4 Task 1
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define NTHREADS 100000

void *do_nothing(void *null) 
{
	int i;
	int c;
	for(i = 0; i < 20000; i++)
	{
		int c = i + 50000;
	}
	pthread_exit(NULL);
}                      

int main(int argc, char *argv[]) 
{
	int rc, j;
	pthread_t tid;
	pthread_attr_t attr;
	
	time_t t = time(NULL);
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	

	/*
	Will measure the time to create and wait for each thread. do_nothing will simply iterate a bunch of times, so we can see how each thread reacts

	*/
	printf("Start time: %s\n", ctime(&t));
			
	for (j=0; j<NTHREADS; j++) {
		rc = pthread_create(&tid, &attr, do_nothing, NULL);
		if (rc) {              
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
		
		/* Wait for the thread */
		rc = pthread_join(tid, NULL);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}
	
	t = time(NULL);
	printf("End time: %s\n", ctime(&t));
	
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
	
}