/*
Joshua Steward
12/15/2016
Lab Final
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define MAXSIZE 500


typedef enum
{
    pear,
    peach,
    apple,
    corn,
    invalid
}itemType;

typedef struct
{

    int quantity;
}cart;

typedef struct
{
    //itemType type;
    char name[10];
    double price;
    int quantity;
}item;





int main(int argc, char *argv[])
{
    int numOfLanes;
    printf("Enter # of open lanes:\n");
    scanf("%d",&numOfLanes);
    printf("Number of open lanes is: %d\n", numOfLanes);

    //will contain the # of people in each lane
    int lanes[numOfLanes];


    for (int i = 0; i < numOfLanes; ++i)
    {
        printf("Enter number of carts in lane %d\n", i);
        scanf("%d", &lanes[i]);
    }

    item newItem;
    char* itemName;
    printf("Enter your item: (name price quantity)\n");
    //scanf("%s %lf %d", itemName, newItem.price, newItem.quantity);
    scanf("%s %lf %d", newItem.name, &newItem.price, &newItem.quantity);

    /**
    if (strcmp(itemName, "pear") == 0)
    {
        newItem.type = pear;
    }
    else if (strcmp(itemName, "peach") == 0)
    {
        newItem.type = peach;
    }
    else if(strcmp(itemName, "apple") == 0)
    {
        newItem.type = apple;
    }
    else if (strcmp(itemName, "corn") == 0)
    {
        newItem.type = corn;
    }
    else
    {
        newItem.type = invalid;
    }

     */


    printf("Your item: %s %lf %d\n", newItem.name, newItem.price, newItem.quantity);


    /*
    FILE *fp = freopen(argv[1], "r", stdin);
    if (fp == NULL)
    {
        printf("Oops! This file sucks!\n");
        exit(-1);
    }
    scanf("%s", type);
    if (scanf("%d", &originalQuantum) <= 0 && strcmp(type, "RR") == 0)
    {
        printf("No quantum! Round Robin Scheduling Must have a quantum!\n");
        exit(-1);
    }
    quantum = originalQuantum;
    //this will be the policy number corresponding to either fcfs, sjf, rr, etc.
    printf("Original Defined Quantum is: %d\n", originalQuantum);
    printf("Quantum is: %d\n", quantum);
    int policy;
    if (strcmp(type, "RR") == 0)
    {
        policy = 0;
        fscanf(fp, "%s %d", processName, &nextArrivalTime);
    }
    else if(strcmp(type, "FCFS") == 0)
    {
        policy = 1;
        fscanf(fp, "%s %d", processName, &nextArrivalTime);
    }
    else if(strcmp(type, "SJF") == 0)
    {
        policy = 2;
        fscanf(fp, "%s %d", processName, &nextArrivalTime);
    }
    else if (strcmp(type, "SRTF") == 0)
    {
        policy = 3;
        fscanf(fp, "%s %d", processName, &nextArrivalTime);
    }
    else
    {
        printf("What kind of scheduling are you using?!\n");
        exit(-1);
    }
    processesRead = 0;
    currentCpuProcess = 0;
    done = 0;
    time = 0;
    processUpToBat = 1;
    totalWaitingTime = 0;
    while (done == 0)
    {
        //each iteration will be one time iteration
        switch (policy)
        {
            case 0:
                rr(fp);
                quantum--;
                break;
            case 1:
                fcfs(fp);
                break;
            case 2:
                sjf(fp);
                break;
            case 3:
                srtf(fp);
                break;
            default:
                break;
        }
        if (currentCpuProcess == 0)
        {
            done++;
        }
        currentCpuBurst--;
        time++;
    }
     */
}