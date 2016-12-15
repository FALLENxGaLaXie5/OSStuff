/*
Joshua Steward
9/28/16
Lab 3 Task 4
*/


#include "messg.h"

MESSG msg_rcvd, msg_send;

STAT stat; // holds response codes from functions
key_t key; // "address" of the process queue

float avg = 0; //

int fd1[2];
char buf1;

int fd2[2];
char buf2;

int fd3[2];
char buf3;

int fd4[2];
char buf4;


pipe(fd1); //creating 1st pipe
pipe(fd2);
pipe(fd3);
pipe(fd4);




void waitForChillun()
{
	printf("I am the parent %d\n",getpid());
    if (wait(NULL) < 0)
    {
      printf("ERROR\n");
    }
		
	printf("Child Complete\n");
}

int main(int argc, char *argv[])
{
	pid_t pid = fork();
	if (pid < 0)
	{
		printf("Fork failed!\n");
	}
	else if (pid == 0)
	{
		//execute client
		printf("I am client #1!\n");
		execlp("./client", "./client", NULL);
	}
	else
	{
		//parent
		pid = fork();
		if (pid < 0)
		{
			printf("Fork failed!\n");
		}
		else if (pid == 0)
		{
			printf("I am client #2!\n");
			execlp("./client", "./client", NULL);
		}
		else
		{
			//parent
			pid = fork();
			if (pid < 0)
			{
				printf("Failed fork!\n");
			}
			else if (pid == 0)
			{
				printf("I am client #3!\n");
				execlp("./client", "./client", NULL);
			}
			else
			{
				pid = fork();
				if (pid < 0)
				{
					printf("Fork failed!\n");
				}
				else if (pid == 0)
				{
					printf("I am client #4!\n");
					execlp("./client", "./client", NULL);
				}
				else
				{
					write(fd4[0], argv[1], strlen(argv[1]));
					waitForChillun();
				}
				write(fd3[0], argv[1], strlen(argv[1]));
				waitForChillun();
			}
				write(fd2[0], argv[1], strlen(argv[1]));
			waitForChillun();
		}

	write(fd1[0], argv[1], strlen(argv[1]));

	msg_send.priority = DEFAULT_PRIORITY; // or "type"; can be used for selective reception
	if (argc > 1)
		key = atoi(argv[1]);
	else 
		key = SRV_PORT;
	
	msg_send.sender = msgget(key, 0666 | IPC_CREAT); // 0666 will be explained later in the course
	if (msg_send.sender < 0)
		oops("SRV: FAILED TO CREATE MESSAGE QUEUE.\n",1);
	
	while (TRUE) // check the defines in the h file
	{
		// recall: the first part of the message (long) is mandatory
		// the system want size of your part
		// the last '0' tells the system to block
		// alternatively, IPC_NOWAIT could be used if no blocking needed (no in this program)
		stat = msgrcv(msg_send.sender, &msg_rcvd, sizeof(MESSG) - sizeof(long), DEFAULT_PRIORITY, 0);
		if (stat < 0)
		{
			printf("SRV: FAILED TO RECEIVE MESSAGE FROM CLIENT.\n");
			continue;
		}
		else
			printf("SRV: CLIENT %d REPORTS: %f\n", msg_rcvd.sender, msg_rcvd.num);
		
		avg = (avg + msg_rcvd.num )/2;
		
		msg_send.num = avg;
		stat = msgsnd(msg_rcvd.sender, &msg_send, sizeof(MESSG) - sizeof(long), 0);
		if (stat < 0)
			printf("SRV: FAILED TO SEND RESPONSE TO CLIENT.\n");
	}
	
	stat = msgctl(msg_send.sender, IPC_RMID, NULL);
	
	printf("SRV: AVERAGE NUMBER: %f\n", avg);
	
	exit(0);

	waitForChillun();
	}
}
