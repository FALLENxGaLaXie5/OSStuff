/*
Joshua Steward
9/28/16
Lab 3 task 3
*/

#include	"npipe.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char** argv) 
{
	/*int fd;
    char * myfifo = "/tmp/myfifo";
    char buf[MAX_LINE];

    // open, read, and display the message from the FIFO 
    fd = open(myfifo, O_RDONLY);
    read(fd, buf, MAX_LINE);
    printf("Received: %s\n", buf);
    close(fd);

    return 0;
*/
	
	char line[MAX_LINE];
	int pipe;
	
	// open a named pipe
	pipe = open("/tmp/myFIFO", O_RDONLY);
	
   // set the mode to blocking (note '~')
   int flags;
   flags &= ~O_NONBLOCK;
   //flags |= O_NONBLOCK;
   fcntl(pipe, F_SETFL, flags);
   
	// read the data from the pipe
	read(pipe, line, MAX_LINE);
	
	printf("Received line: %s\n", line);
	
	// close the pipe
	close(pipe);
	return 0;
	
}