/*
Joshua Steward
9/28/16
Lab 3 task 3
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include	<string.h>

#define MAX_LINE 80
#define	oops(m,x)	{ perror(m); exit(x); }

