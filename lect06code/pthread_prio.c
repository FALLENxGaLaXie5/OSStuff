#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define PRIORITY 3

void *runner(void *param) 
{
	printf("Hello world from: %d\n", getpid());
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	pthread_attr_t attr;

	// get the default attributes
	pthread_attr_init(&attr);

   // specialize attributes
   struct sched_param schedparam;
   schedparam.sched_priority = PRIORITY;

   pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
   pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
   pthread_attr_setschedpolicy(&attr, SCHED_RR);
   pthread_attr_setschedparam(&attr, &schedparam);
   pthread_create(&tid, &attr, &runner, &argv);
   pthread_join(tid, NULL);
   pthread_attr_destroy(&attr);
}


