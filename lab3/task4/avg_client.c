/*
Joshua Steward
9/28/16
Lab 3 Task 4
*/




#include "messg.h"

MESSG msg_rcvd, msg_send;

STAT stat;
key_t srv_msgid;

int avg = 0;

int main(int argc, char *argv[])
{
	// this is a client, so we need to "point" it to the server
	if (argc < 3)
		oops("Usage: avg_client <port_number> <value>\n", 1);
	

	char buf;
	while (read(pipefd[0], &buf, 1) > 0) // read while EOF
               write(1, &buf, 1);
           write(1, "\n", 1);



	msg_send.priority = DEFAULT_PRIORITY; // a.k.a. type
	
	msg_send.sender = msgget(atoi(argv[1]), 0666 | IPC_CREAT); // create
	if (msg_send.sender < 0)
		oops("CLI: FAILED TO CREATE MESSAGE QUEUE.\n", 1);
	
	srv_msgid = msgget(SRV_PORT, 0666); // just access; assume that the server created it
	if (srv_msgid < 0)
		oops("CLI: FAILED TO GET SERVER MESSAGE QUEUE.\n", 1);
	
	msg_send.num = atof(argv[2]);
	
	// recall: the first part of the message (long) is mandatory
	// the system want size of your part
	// the last '0' tells the system to block
	// alternatively, IPC_NOWAIT could be used if no blocking needed (no in this program)
	stat = msgsnd(srv_msgid, &msg_send, sizeof(MESSG) - sizeof(long), 0);
	if (stat < 0)
		oops("CLI: FAILED TO SEND NUMBER TO SERVER.\n", 1);
	
	stat = msgrcv(msg_send.sender, &msg_rcvd, sizeof(MESSG) - sizeof(long), DEFAULT_PRIORITY, 0);
	if (stat < 0)
	{
		oops("CLI: FAILED TO RECEIVE RESPONSE FROM SERVER.\n", 1);
	}
	else
		printf("CLI: SERVER REPORTS AVERAGE NUMBER: %f\n", msg_rcvd.num);
	
	stat = msgctl(msg_send.sender, IPC_RMID, NULL);
	
	exit(0);
}
