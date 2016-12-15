/*
Joshua Steward
9/28/16
Lab 3 task 2
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
	char fsx[MAXLINE];



	if (pipe(fd) < 0)
	{
		printf("Cannot create a pipe.");
	}

	if ( (pid = fork()) < 0)
	{
		printf("Cannot fork.");
	}
	else if (pid == 0) /* child */
	{
		//char buf[MAXLINE];
		//while ( fgets(buf, MAXLINE, stdin) != NULL)
		//{
		//	write(fd[1], buf,23);
		//}
		//write(fd[1], "Message for the parent\n", 23);
		n = read(fd[0], line, MAXLINE);
		printf("%s", line);
		close(fd[1]);
		sprintf(fsx, "/dev/fd/%d", fd[0]);
		execlp("xterm", "xterm", "-e", "cat", fsx, NULL);
	}
	else /* parent */
	{
		char buf[MAXLINE];
		printf("What do you want to say to Child?\n");
		if(fgets(buf, MAXLINE, stdin) != NULL)
		{
			write(fd[1], buf,23);
		}
		
		//write(fd[1], "Message for the child\n", 22);
		n = read(fd[0], line2, MAXLINE);
		printf("%s", line2);
		close(fd[0]);
		wait(NULL);
	}
}