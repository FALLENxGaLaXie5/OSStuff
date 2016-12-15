#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[])
{       

	printf("YOOOOO REACHED\n");
    int s2c, c2s, i; 
    char fifo_name1[] = "/tmp/ajsFIFO";
    char fifo_name2[] = "/tmp/ajsFIFO";
    char msg[80], buf[10];
    struct stat st;
	printf("YOOOOO REACHED\n");
    // if no fifos, create 'em
    if (stat(fifo_name1, &st) != 0)
        mkfifo(fifo_name1, 0600);
    if (stat(fifo_name2, &st) != 0)
        mkfifo(fifo_name2, 0600);
    	printf("YOOOOO REACHED\n");

   	s2c= open(fifo_name1, O_WRONLY);
    c2s= open(fifo_name2, O_RDONLY);

    	printf("YOOOOO REACHED\n");

    // start sending messages, with 5s interval
    for (i=0; i<2; i++)
    {
        printf("Message #%d \n", i);
        printf("Enter Message\n");
        scanf("%s \n", msg);

        //strcpy(msg, "Message #"); 
       // strcpy(msg, itoa(i, buf, 10));
        //strcpy(msg, "\0"); 

        write(s2c, msg, strlen(msg)+1);

        //sleep(5);
    }

    // delete fifos
    unlink(fifo_name1);
    unlink(fifo_name2);
    printf("server exit successfully");
    return EXIT_SUCCESS;
}