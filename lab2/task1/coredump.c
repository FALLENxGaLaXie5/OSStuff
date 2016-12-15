/*
Joshua Steward
9/6/2016
Lab 2 Task 1
*/
#include <stdio.h>
int offensiveFunc(int, int);

int main(void)
{
   printf("The next line will cause core dump.\n");
   printf("ERROR: %d\n", offensiveFunc(3.0, 1.0));
   return 0;
}

int offensiveFunc(int x, int y)
{
   int z = x / y;
   return z;
}
