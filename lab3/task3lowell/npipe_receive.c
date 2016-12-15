#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int c2s, s2c, c=0;
    char buf[10];

    char fifo_name1[] = "/tmp/ajsFIFO";
    char fifo_name2[] = "/tmp/ajsFIFO";
   s2c= open(fifo_name1, O_RDONLY);
    c2s= open(fifo_name2, O_WRONLY);

    // receive messages
    while (1)
    {
        if (read(s2c, &buf, sizeof(char)*10) > 0)
        {
            printf("%s \n", buf);
            c=0;
        }
        sleep(1);
        c++;    
        if (c>6) 
            break;
    }

    printf("client exit successfully");
    return EXIT_SUCCESS;
}  