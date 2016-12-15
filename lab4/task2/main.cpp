/*
Joshua Steward
9/28/16
Lab 4 Task 2
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

//Matrixes
#define MAX_SIZE 1024

typedef struct v
{
    int i;
    int j;
}vT;

int a[MAX_SIZE][MAX_SIZE], b[MAX_SIZE][MAX_SIZE], c[MAX_SIZE][MAX_SIZE];
int m, k, n;

void *matrixThread(void *cellIndex);
void loadMatrices(char *fileName)
{
    FILE *fp = freopen(fileName, "r", stdin);
    if (fp == NULL)
    {
        printf("Oops! This file sucks!\n");
        exit(-1);
    }
    //m is the number of rows in A
    scanf("%d", &m);
    scanf("%d", &k);
    scanf("%d", &n);
    //m = fgetc(fp) - '0';
    //k is the number of columns in A, and rows in B
    //n is the number of columns in B
    printf("%d\n", m);
    printf("%d\n", k);
    printf("%d\n", n);


    if (m == 0 || n == 0 || k == 0 || n > MAX_SIZE || m > MAX_SIZE || k > MAX_SIZE)
    {
        printf("Not Valid!\n");
        exit(-1);
    }

    //reading in the first m lines; matrix a
    int ch;
    int r;
    int c;
    for (r = 0; r < m; r++)
    {
        for (c = 0; c < k; c++)
        {
            scanf("%d", &(a[r][c]));
        }
    }
    for (r = 0; r < k; r++)
    {
        for (c = 0; c < n; c++)
        {
            scanf("%d", &(b[r][c]));
        }
    }
}
void loadMatrix(FILE*, int [][MAX_SIZE], int, int);
void multiply(int [][MAX_SIZE], int[][MAX_SIZE], int[][MAX_SIZE], int, int, int);



void displayMatrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int columns)
{
    int r;
    int c;
    printf("\n");
    for (r = 0; r < rows; r++)
    {
        for (c = 0; c < columns; c++)
        {
            printf("%d ", matrix[r][c]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    loadMatrices(argv[1]);
    displayMatrix(a, m, k);
    displayMatrix(b, k, n);
    //int rc, j;
    //pthread_t tid;
    //pthread_attr_t attr;

    //time_t t = time(NULL);

    //pthread_attr_init(&attr);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    /*
    Will measure the time to create and wait for each thread. do_nothing will simply iterate a bunch of times, so we can see how each thread reacts

    */
    /*
    printf("Start time: %s\n", ctime(&t));

    for (j=0; j<NTHREADS; j++) {
        rc = pthread_create(&tid, &attr, do_nothing, NULL);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }

        // Wait for the thread
        rc = pthread_join(tid, NULL);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }
    }
    */

    //t = time(NULL);
    //printf("End time: %s\n", ctime(&t));
    //
    //pthread_attr_destroy(&attr);
    //pthread_exit(NULL);

}