/*
Joshua Steward
9/28/16
Lab 4 Task 3
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

int **a, **b, **c;
int m, k, n;

void loadMatrix(FILE*, int ***, int, int);


struct v
             {
             	int i;
             	int j;
             };

void *matrixThread(void *cellIndex)
{
    struct v * data = cellIndex;
    int sum = 0;
    int d;
    for (d = 0; d < k; ++d)
    {
        sum += a[data->i][d] * b[d][data->j];
    }
    c[data->i][data->j] = sum;
    pthread_exit(0);
}

void allocateAndLoadMatrices(char *fileName)
{
	FILE *fp = freopen(fileName, "r", stdin);
	if (fp == NULL)
	{
		printf("Oops! This file sucks!\n");
		exit(-1);
	}
	scanf("%d", &m);
	scanf("%d", &k);
	scanf("%d", &n);

	printf("%d\n", m);
	printf("%d\n", k);
	printf("%d\n", n);


	if (m == 0 || n == 0 || k == 0 || n > MAX_SIZE || m > MAX_SIZE || k > MAX_SIZE)
	{
		printf("Not Valid!\n");
		exit(-1);
	}

    //allocate memory for arrays

    a = (int **)malloc(m * sizeof(int *));
    int i;
    for (i = 0; i < m; ++i)
    {
        a[i] = (int *)malloc(k * sizeof(int));
    }
    b = (int **)malloc(k * sizeof(int *));
    for (i = 0; i < k; ++i)
    {
        b[i] = (int *)malloc(n * sizeof(int));
    }
    c = (int **)malloc(m * sizeof(int *));
    for (i = 0; i < m; ++i)
    {
        c[i] = (int *)malloc(n * sizeof(int));
    }

	//reading in the first m lines; matrix a
	int ch;
	int r;
	int d;
	for (r = 0; r < m; r++)
	{
		for (d = 0; d < k; d++)
			{
				scanf("%d", &(a[r][d]));
			}	
	}
	for (r = 0; r < k; r++)
	{
		for (d = 0; d < n; d++)
        {
            scanf("%d", &(b[r][d]));
        }
	}

}


void multiply(int **aMatrix, int **bMatrix, int **cMatrix, int m, int k, int n)
{
    int rc;
    int r;
    int d;
    for (r = 0; r < m; ++r)
    {
        for (d = 0; d < n; ++d)
        {
            struct v * point = (struct v *)malloc(sizeof(struct v));
            point->i = r;
            point->j = d;



            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            rc = pthread_create(&tid, &attr, matrixThread, point);
            if (rc)
            {
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }
    }
}

void displayMatrix(int **matrix, int rows, int columns)
{
	int r;
	int d;
	printf("\n");
	for (r = 0; r < rows; r++)
	{
		for (d = 0; d < columns; d++)
		{
			printf("%d ", matrix[r][d]);
		}
		printf("\n");
	}
}


int main(int argc, char *argv[]) 
{
	allocateAndLoadMatrices(argv[1]);
	displayMatrix(a, m, k);
	displayMatrix(b, k, n);
    multiply(a,b,c,m,k,n);
    displayMatrix(c,m,n);
}