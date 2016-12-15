#include	<stdio.h>
#include	<unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
   int i = 0;
   //while (i == 0)
    //  ;
   printf("%s\n", argv[2]);
   printf("Process %s pid is: %d\n", argv[1], getpid());
   exit(0);
}