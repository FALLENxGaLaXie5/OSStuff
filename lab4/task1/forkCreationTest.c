/*
Joshua Steward
9/28/16
Lab 4 Task 1
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#define NFORKS 100000


void do_nothing() {
	int i;
	int c;
	for(i = 0; i < 20000; i++)
	{
		int c = i + 50000;
	}
}

int main(int argc, char *argv[]) {
	int pid, j, status;
	
	time_t t = time(NULL);
	printf("Start time: %s\n", ctime(&t));
	

	/*
	Will measure the time to create and wait for each process. do_nothing will simply iterate a bunch of times, so we can see how each thread reacts
	
	*/
	for (j=0; j<NFORKS; j++) {
		
		/*** error handling ***/
		if ((pid = fork()) < 0 ) {
			printf ("fork failed with error code= %d\n", pid);
			exit(0);
		}
		
		/*** this is the child of the fork ***/
		else if (pid ==0) {
			do_nothing();
			exit(0);
		}
		
		/*** this is the parent of the fork ***/
		else {
			waitpid(pid, &status, 0);
		}
	}
	t = time(NULL);
	printf("End time: %s\n", ctime(&t));
}  
