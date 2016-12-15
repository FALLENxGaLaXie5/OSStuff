/*
Joshua Steward
9/27/16
Lab 5 Task 1
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
    FILE *fp = freopen(argv[1], "r", stdin);
    if (fp == NULL)
    {
        printf("Oops! This file sucks!\n");
        exit(-1);
    }
    double nextTau, alpha, prevAve, tn, total = 0;
    int count = 1;
    scanf("%lf", &alpha);
    scanf("%lf", &prevAve);
    nextTau = prevAve;
    total = nextTau;
    printf("This average will be: %lf\n", nextTau);
    //scans in current bursts and averages them, then implements exponential averaging formula
        //to predict what the next burst will be
    while (scanf("%lf", &tn) > 0)
    {
        count++;
        total+=tn;
        nextTau = (alpha * tn) + ((1 - alpha) * prevAve);

        printf("The next average will be: %lf\n", nextTau);

        prevAve = total/count;
    }
}