/*
Joshua Steward
9/19/2016
Lab 3 Task 1

*/


#include	<errno.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#define oops(m) ({fprintf(stderr, "ERROR: %s\n", m); exit(-1);})


void waitForChild()
{
	printf("I am the parent %d\n",getpid());
    if (wait(NULL) < 0)
    {
      printf("-1 from wait(NULL) with errno = %d\n", errno);
    }
		
	printf("Child Complete\n");
}

void executeProcess(char* name)
{
	if (execlp("./iam","iam", "Hello Parent!",name,NULL) < 0)
	{
   	   oops("Execlp Failed!");
    }
}


int main()
{
	char  myName[10];
	strcpy(myName, "1");
   pid_t pid = fork();
	/* fork a child process */

   printf("I am root! My Name is %s\n", myName);

	if (pid < 0)
	{
		/* error occurred */
		oops("Fork Failed!");
	} 
	else if (pid == 0) 
	{ /* child process */
		printf("I am the child %d\n",getpid());
		
		pid_t llChild = fork();
		if (llChild < 0)
		{
			oops("Fork failed!");
		}
		else if (llChild == 0)
		{
			printf("I am the child %d\n",getpid());
			printf("Nice job! You're at the bottom of the tree!\n");
			strcat(myName,".1.1");
			//printf("%s\n", myName);
			executeProcess(myName);
		}
		else
		{
			pid_t lrChild = fork();
			if (lrChild < 0)
			{
				oops("Fork failed!");
			}
			else if (lrChild == 0)
			{
				printf("I am the child %d\n",getpid());
				printf("Nice job! You're at the bottom of the tree!\n");
				strcat(myName,".1.2");
				//printf("%s\n", myName);
				executeProcess(myName);
			}
			else
			{
				waitForChild();
			}
			waitForChild();
		}
		strcat(myName,".1");
		//printf("%s\n", myName);
		executeProcess(myName);
	}
	else 
	{ /* parent process */
		/* parent will wait for the child to complete */
		pid_t rightChild = fork();

		if (rightChild < 0)
		{
			oops("Fork failed!");
		}
		else if (rightChild == 0)
		{
			//I am the right child of the root
			pid_t rlChild = fork();
			if (rlChild < 0)
			{
				oops("Fork failed!");
			}
			else if (rlChild == 0)
			{
				//right child of root, left child of that node
				printf("I am the child %d\n",getpid());
				printf("Nice job! You're at the bottom of the tree!\n");
				strcat(myName,".2.1");
				//printf("%s\n", myName);
				executeProcess(myName);
			}
			else
			{
				pid_t rrChild = fork();
				if (rrChild < 0)
				{
					oops("Fork failed!");
				}
				else if (rrChild == 0)
				{
					printf("I am the child %d\n",getpid());
					printf("Nice job! You're at the bottom of the tree!\n");
					strcat(myName,".2.2");
					//printf("%s\n", myName);
					executeProcess(myName);
				}
				else
				{
					waitForChild();
				}
				waitForChild();
			}
			strcat(myName, ".2");
			//printf("%s\n", myName);
			executeProcess(myName);
		}
		else
		{
			waitForChild();
		}
		waitForChild();
	}
}