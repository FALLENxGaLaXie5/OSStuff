#include	<sys/signal.h>
#include	<stdio.h>
#include	<unistd.h>

static void	sig_usr(int);	/* one handler for both signals */

int
main(void)
{
	printf("Running with pid = %d\n", getpid());
	fflush(stdout);
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
		fprintf(stderr, "can't catch SIGUSR1");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
		fprintf(stderr, "can't catch SIGUSR2");
	for ( ; ; )
		pause();
}

static void
sig_usr(int signo)		/* argument is signal number */
{
	if (signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else
		fprintf(stderr, "received signal %d\n", signo);
}
