/*
 * Joshua Steward
 * 10/13/2016
 * Lab 7 Task 1
 */

#include "stdio.h"
#include <string.h>

int adj_mat[50][50];
//this will be used to check nodes after they're visited in the same cyclic run through;
//  otherwise, checking can create infinite loops
int checkVisit[50];
//flag will signify if deadlocked or not
int flag = 0;

void zeroVisit(int numVert)
{
    for (int r = 0; r < numVert; ++r)
    {
        checkVisit[r] = 0;
    }
}

/*
 * Recursively implemented cycle check.
 * c - previous column, now the row parameter
 * numVert - number of vertices
 * oR - original row at beginning of potential cycle
 * oC - original column at beginning of potential cycle
 *
 * Algorithm - For each column in current row, checks if it is 1, and checks against beginning of potential cycle
 *                  If true, marks as deadlocked
 *             if current column is already marked as visited, continues to next column
 *             if 1, checks it as visited and calls function with column as new row
 *             else, keeps iterating to next column
 */
void checkForCycle(int c, int numVert, int oR, int oC)
{
    for (int col = 0; col < numVert; col++)
    {
        if(adj_mat[c][col] == 1 && c == oR && col == oC)
        {
            flag = 1;
            printf("CYCLE DETECTED!!!! ALERT ALERT ALERT!\n");
            return;
        }
        else if(checkVisit[col] == 1)
        {
            continue;
        }
        else if (adj_mat[c][col] == 1)
        {
            checkVisit[col] = 1;
            checkForCycle(col, numVert, oR, oC);
        }
    }
}


void printAdjMatrix(int num)
{
    for (int r = 0; r < num; ++r)
    {
        for (int c = 0; c < num; ++c)
        {
            printf("%d ",adj_mat[r][c]);
        }
        printf("\n");
    }
}



int main(int argc, char *argv[])
{
    /*
     * Reads in vertices, skipping beginning characters and using a 10 char buffer
     *      to read in until commas
     */
    char verticesRead[100][100];
    freopen("input.txt", "r", stdin);
    int i = 0;
    scanf("V={");
    do
    {
        scanf(" %10[^,}]",verticesRead[i]);
        i++;
    }while (getc(stdin) == ',');
    int j;
    for (j = 0; j < i; j++)
    {
        printf("Vert %d: %s\n", j, verticesRead[j]);
    }
    //i will be the number of vertices

    /*
     * Reads in edges, skipping initial characters then using 10 character buffer on edges and
     *      storing in the adjacency matrix concurrently. Stores by comparing to the vertices, and if
     *          equal, sets that spot in adjacency matrix to 1. Rest of spots are 0 by default
     */
    scanf("\nE={");

    char vert1[100];
    char vert2[100];
    while (getc(stdin) == '{')
    {
        scanf("%10[^,]",vert1);
        scanf(", %10[^}]",vert2);
        scanf("}, ");
        printf("%s - %s\n",vert1,vert2);
        int r;
        for(r = 0; r < i; r++)
        {
            if (strcmp(verticesRead[r], vert1) == 0)
            {
                break;
            }
        }
        int c;
        for(c = 0; c < i; c++)
        {
            if(strcmp(verticesRead[c], vert2) == 0)
            {
                break;
            }
        }
        //r is the index of that edge now
        //c is the infex of the edge connected to
        adj_mat[r][c] = 1;
    }
    printf("\n");
    printAdjMatrix(i);


    /*
     * For every edge in matrix, first zeros out visited array then checks for a cycle
     */

    for (int k = 0; k < i; ++k)
    {
        for (int l = 0; l < i; ++l)
        {
            if (adj_mat[k][l] == 1)
            {
                if(flag == 0)
                {
                    printf("Checking for cycle from %d, %d\n", k,l);
                    zeroVisit(i);
                    checkForCycle(l,i,k,l);
                }
            }
        }
    }

    if (flag == 0)
    {
        printf(">> NO DEADLOCK <<\n");
    }
    else
    {
        printf(">> DEADLOCKED <<\n");
    }
}