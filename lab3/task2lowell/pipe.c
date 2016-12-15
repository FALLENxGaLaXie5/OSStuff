/*
Lowell Batacan
9-18-2016
Lab 3 Task 1
*/

#include	<sys/types.h>
#include	<sys/wait.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>

#define MAXLINE 1024
#define	oops(m,x)	{ perror(m); exit(x); }

int main(void)
{
	int		n, fd[2];
	pid_t	pid;
	char	line[MAXLINE];
	char	line2[MAXLINE];
	char f[MAXLINE + 1];

	if (pipe(fd) < 0)
	{
		printf("Cannot create a pipe.");
		abort();
	}

	if ( (pid = fork()) < 0)
	{
		printf("Cannot fork.");
		abort();
	}

	else if (pid == 0) /* child */
	{
		 
		 printf("ENter Child Message\n");
		 scanf("%s", f);
		 printf("%s", f);
		write(fd[1], "Message for the parent\n", 23);
		//n = read(fd[0], line, MAXLINE);
		//printf("%s", line);
		//close(fd[1]);
		
		  sprintf(f, "/dev/fd/%d", fd[0]);
        execlp("xterm", "xterm", "-e", "cat", f, NULL);
        abort();
	}
	else /* parent */
	{
		 printf("ENter Parent Message\n");
		 scanf("%s\n", f);
		 printf("%s\n", f);
		//close(fd[0]);
		write(fd[1], "Message for the child\n", 22);
		
		
		//n = read(fd[0], line2, MAXLINE);
		printf("%s", line2);
		close(fd[1]);
		wait(NULL);
	}
}