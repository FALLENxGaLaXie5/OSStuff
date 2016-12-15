/*
Joshua Steward
9/28/16
Lab 3 task 3
*/




#include	"npipe.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
/*
int main(int argc, char** argv) 
{
	int fd;
	char line[MAX_LINE];
    char * myfifo = "/tmp/myfifo";

    //create the FIFO (named pipe) 
    mkfifo(myfifo, 0666);
    fd = open(myfifo, O_WRONLY);

		// get a line to send
	printf("Enter line: ");
	fgets(line, MAX_LINE, stdin);
	
		
	// actually write out the data and close the pipe
	write(fd, line, strlen(line));
    // write "Hi" to the FIFO 
//    close(fd);

    // remove the FIFO //
    unlink(myfifo);

    return 0;
	/*
	char line[MAX_LINE];
	int pipe;
	
	int stat;
	if (stat = mkfifo("/tmp/ajsFIFO", 0600) < 0)
		oops("Cannot make FIFO", stat);
	
	// open a named pipe
	pipe = open("/tmp/ajsFIFO", O_WRONLY);
	
	while(1)
	{
		// get a line to send
		printf("Enter line: ");
		fgets(line, MAX_LINE, stdin);
		
		if (strcmp(line, "quit\n") == 0)
			break;
		
		// actually write out the data and close the pipe
		write(pipe, line, strlen(line));
	}
	// close the pipe
	close(pipe);
	return 0;
	
}
*/

int main(int argc, char  **argv)
{
	int fd1, fd2, c; 
    char fifo1[] = "/tmp/myFIFO";
    char fifo2[] = "/tmp/myFIFO";
    char msg[80];
    char buffer[10];
    struct stat str;
    // if no fifos, create a fifo
    if (stat(fifo1, &str) != 0)
    {
    	mkfifo(fifo1, 0600);
    } 
    if (stat(fifo2, &str) != 0)
    {
    	mkfifo(fifo2, 0600);
    }

   	fd1= open(fifo1, O_WRONLY);
    fd2= open(fifo2, O_RDONLY);

    // start sending messages, with 5s interval
    for (c=0; c<2; c++)
    {
        printf("Message # is%d \n", c);
        printf("Enter Message Here:\n");
        scanf("%s \n", msg);

        //strcpy(msg, "Message #"); 
       // strcpy(msg, itoa(i, buf, 10));
        //strcpy(msg, "\0"); 

        write(fd1, msg, strlen(msg)+1);

        //sleep(5);
    }

    // delete fifos
    unlink(fifo1);
    unlink(fifo2);
    printf("server exit successfully");
    return EXIT_SUCCESS;
}