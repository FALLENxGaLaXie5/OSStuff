/*
Joshua Steward
9/29/16
Lab 5 Task 3
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define MAXSIZE 500

int startTimes[MAXSIZE];
int bursts[MAXSIZE];
char type[10];
int time;
int done;
int processUpToBat;
int originalQuantum;
int quantum;
char processName[10];//this is just a placeholder that will be read in to keep track of where we are in the file
int currentCpuProcess;
int currentCpuBurst;
int nextArrivalTime;
int processesRead;
int totalWaitingTime;
double averageWaitingTime;


void readIn(FILE* fp)
{
    processUpToBat = 1;
    char process[10];
    while (fscanf(fp, "%s %d %d", process, &(startTimes[processUpToBat]), &(bursts[processUpToBat])) != EOF)
    {
        processUpToBat++;
    }
}

void printFile()
{
    int i;
    for (i = 1; i < processUpToBat; ++i)
    {
        printf("P%d: %d %d\n", i, startTimes[i], bursts[i]);
    }
}

void rr(FILE* fp)
{
    //if the next arrival time is the current time, reads in the process (which is just reading in the next burst time)
    if (nextArrivalTime == time)
    {
        processesRead++;
        //reads in the burst time and adds it to the end of the process queue along with the next process's arrival time
        fscanf(fp, "%d %s %d",&bursts[processesRead],processName,&nextArrivalTime);

        if (currentCpuProcess == 0)
        {
            currentCpuProcess = processesRead;
            currentCpuBurst = bursts[processesRead];
            bursts[processesRead] = 0;
        }
        //print cpu state information (what process is being executed by the cpu currently,
        // and what time is left on that process)
        //code
        printf("\n\nT%d\n",time);
        printf("CPU: P%d(%d)\n",currentCpuProcess,currentCpuBurst);
        printf("QUEUE: ");
        int i;
        for (i = 1; i <= processesRead; ++i)
        {
            if (bursts[i] != 0)
            {
                printf("P%d(%d) ",i,bursts[i]);
            }
        }
    }

    if (quantum < 1)
    {
        //time allotted to this process has run out, so this process is put back in queue, and next
        //  process up is put into cpu

        int i;
        int found = 0;
        for (i = currentCpuProcess + 1; i <= processesRead; ++i)
        {
            if(bursts[i] > 0)
            {
                bursts[currentCpuProcess] = currentCpuBurst;
                currentCpuProcess = i;
                currentCpuBurst = bursts[i];
                found = 1;
                break;
            }
        }
        if (found != 1)
        {
            int j;
            for (j = 1; j < currentCpuProcess; ++j)
            {
                if (bursts[j] > 0)
                {
                    bursts[currentCpuProcess] = currentCpuBurst;
                    currentCpuProcess = j;
                    currentCpuBurst = bursts[j];
                    break;
                }
            }
        }
        printf("\n\nT%d\n",time);
        printf("CPU: P%d(%d)\n",currentCpuProcess,currentCpuBurst);
        printf("QUEUE: ");
        int j;
        for (j = 1; j <= processesRead; ++j)
        {
            if (bursts[j] != 0)
            {
                printf("P%d(%d) ",j,bursts[j]);
            }
        }


        quantum = originalQuantum;


    }

    if (currentCpuBurst == 0)
    {
        //finally, check if the queue is empty
        int c;
        int add = 0;
        for (c = 1; c <= processesRead; ++c)
        {
            add+=bursts[c];
        }

        if (add == 0 && currentCpuBurst < 1)
        {
            currentCpuProcess = 0;

            printf("\n\nT%d:\n",time);
            printf("CPU: <idle>\n");
            printf("QUEUE: <empty>\n");
        }
        else
        {
            //need to switch to next process in the queue
            //currentCpuProcess++;
            //currentCpuBurst = bursts[currentCpuProcess];
            //bursts[currentCpuProcess] = 0;

            //print stuff



            int i;
            int found = 0;
            for (i = currentCpuProcess + 1; i <= processesRead; ++i)
            {
                if(bursts[i] > 0)
                {
                    //bursts[currentCpuProcess] = currentCpuBurst;
                    currentCpuProcess = i;
                    currentCpuBurst = bursts[i];
                    found = 1;
                    break;
                }
            }
            if (found != 1)
            {
                int j;
                for (j = 1; j < currentCpuProcess; ++j)
                {
                    if (bursts[j] > 0)
                    {
                        //bursts[currentCpuProcess] = currentCpuBurst;
                        currentCpuProcess = j;
                        currentCpuBurst = bursts[j];
                        break;
                    }
                }
            }




            printf("\n\nT%d:\n",time);
            printf("CPU: P%d(%d)\n",currentCpuProcess,currentCpuBurst);
            printf("QUEUE: ");

            int j;
            for (j = 1; j <= processesRead; ++j)
            {
                if (bursts[j] != 0)
                {
                    printf("P%d(%d) ",j,bursts[j]);
                }
            }
        }
    }
}

void fcfs(FILE* fp)
{
    //if the next arrival time is the current time, reads in the process (which is just reading in the next burst time)
    if (nextArrivalTime == time)
    {
        processesRead++;
        //reads in the burst time and adds it to the end of the process queue along with the next process's arrival time
        fscanf(fp, "%d %s %d",&bursts[processesRead],processName,&nextArrivalTime);
        printf("New total waiting time: %d",totalWaitingTime);
        if (currentCpuProcess == 0)
        {
            currentCpuProcess = processesRead;
            currentCpuBurst = bursts[processesRead];
            bursts[processesRead] = 0;
        }
        //print cpu state information (what process is being executed by the cpu currently,
        // and what time is left on that process)
        //code
        printf("\n\nT%d\n",time);
        printf("CPU: P%d(%d)\n",currentCpuProcess,currentCpuBurst);
        printf("QUEUE: ");
        int i;
        for (i = 1; i <= processesRead; ++i)
        {
            if (bursts[i] != 0)
            {
                printf("P%d(%d) ",i,bursts[i]);
            }
        }
    }

    if (currentCpuBurst == 0)
    {
        //finally, check if the queue is empty
        int c;
        int add = 0;
        for (c = 1; c <= processesRead; ++c)
        {
            add+=bursts[c];
        }

        if (add == 0 && currentCpuBurst < 1)
        {
            currentCpuProcess = 0;
            averageWaitingTime = totalWaitingTime/processesRead;
            printf("\n\nT%d:\n",time);
            printf("CPU: <idle>\n");
            printf("QUEUE: <empty>\n");
            printf("AVERAGE WAITING TIME: %lf\n",averageWaitingTime);
        }
        else
        {
            //need to switch to next process in the queue
            currentCpuProcess++;
            currentCpuBurst = bursts[currentCpuProcess];
            bursts[currentCpuProcess] = 0;
            //update total waiting time
            totalWaitingTime+=time;
            //print stuff

            printf("\n\nT%d:\n",time);
            printf("CPU: P%d(%d)\n",currentCpuProcess,currentCpuBurst);
            printf("QUEUE: ");

            int i;
            for (i = 1; i <= processesRead; ++i)
            {
                if (bursts[i] != 0)
                {
                    printf("P%d(%d) ",i,bursts[i]);
                }
            }
        }
    }
}

void sjf(FILE* fp)
{
    //sjf will be slightly different; after the current cpu burst gets sto 0, it will pick the next process
    //  to be executed by the shortest burst time in the queue
    //if the next arrival time is the current time, reads in the process (which is just reading in the next burst time)
    if (nextArrivalTime == time)
    {
        processesRead++;
        //reads in the burst time and adds it to the end of the process queue along with the next process's arrival time
        fscanf(fp, "%d %s %d",&bursts[processesRead],processName,&nextArrivalTime);

        if (currentCpuProcess == 0)
        {
            currentCpuProcess = processesRead;
            currentCpuBurst = bursts[processesRead];
            bursts[processesRead] = 0;
        }
        //print cpu state information (what process is being executed by the cpu currently,
        // and what time is left on that process)
        //code
        printf("\n\nT%d\n",time);
        printf("CPU: P%d(%d)\n",currentCpuProcess,currentCpuBurst);
        printf("QUEUE: ");
        int i;
        for (i = 1; i <= processesRead; ++i)
        {
            if (bursts[i] != 0)
            {
                printf("P%d(%d) ",i,bursts[i]);
            }
        }
    }

    if (currentCpuBurst == 0)
    {
        //finally, check if the queue is empty
        int c;
        int add = 0;
        for (c = 1; c <= processesRead; ++c)
        {
            add+=bursts[c];
        }

        if (add == 0 && currentCpuBurst < 1)
        {
            currentCpuProcess = 0;
            averageWaitingTime = totalWaitingTime/processesRead;
            printf("\n\nT%d:\n",time);
            printf("CPU: <idle>\n");
            printf("QUEUE: <empty>\n");
            printf("AVERAGE WAITING TIME: %lf",averageWaitingTime);
        }
        else
        {
            //need to switch to next process in the queue; sjf will pick the process with the smallest burst

            totalWaitingTime+= time;
            //this will get the next shortest burst in the ready queue
            int j;
            int currentShortestBurst;
            int currentProcessNum;
            for (j = 1; j <= processesRead; ++j)
            {
                if (bursts[j] > 0)
                {
                    currentShortestBurst = bursts[j];
                    currentProcessNum = j;
                    break;
                }
            }
            int k;
            for (k = j + 1; k <= processesRead; ++k)
            {
                if (bursts[k] > 0)
                {
                    if (bursts[k] < currentShortestBurst)
                    {
                        currentShortestBurst = bursts[k];
                        currentProcessNum = k;
                    }
                }
            }


            currentCpuProcess = currentProcessNum;
            currentCpuBurst = currentShortestBurst;
            bursts[currentCpuProcess] = 0;

            //print stuff

            printf("\n\nT%d:\n",time);
            printf("CPU: P%d(%d)\n",currentCpuProcess,currentCpuBurst);
            printf("QUEUE: ");

            int i;
            for (i = 1; i <= processesRead; ++i)
            {
                if (bursts[i] != 0)
                {
                    printf("P%d(%d) ",i,bursts[i]);
                }
            }
        }
    }
}

void srtf(FILE* fp)
{
    //srtf will be slightly different again; after the current cpu burst gets sto 0, it will pick the next process
    //  to be executed by the shortest burst time in the queue
    //  and when a new process arrives, the currentCPUBurst time will be compared to the new process
    //  burst time.
    //  If the new burst time is less than the currentCPUBurstTime, then the currentCPUBurst will become
    //      this new burst, the CPU process will be the new process read in, and the process and burst in the CPU will
    //      be read back into the spot in the queue where it belongs
    //if the next arrival time is the current time, reads in the process (which is just reading in the next burst time)
    if (nextArrivalTime == time)
    {
        processesRead++;
        //reads in the burst time and adds it to the end of the process queue along with the next process's arrival time
        fscanf(fp, "%d %s %d",&bursts[processesRead],processName,&nextArrivalTime);
        if (bursts[processesRead] < currentCpuBurst)
        {
            bursts[currentCpuProcess] = currentCpuBurst;
            currentCpuBurst = bursts[processesRead];
            currentCpuProcess = processesRead;
            bursts[processesRead] = 0;
        }
        if (currentCpuProcess == 0)
        {
            currentCpuProcess = processesRead;
            currentCpuBurst = bursts[processesRead];
            bursts[processesRead] = 0;
        }
        //print cpu state information (what process is being executed by the cpu currently,
        // and what time is left on that process)
        //code
        printf("\n\nT%d\n",time);
        printf("CPU: P%d(%d)\n",currentCpuProcess,currentCpuBurst);
        printf("QUEUE: ");
        int i;
        for (i = 1; i <= processesRead; ++i)
        {
            if (bursts[i] != 0)
            {
                printf("P%d(%d) ",i,bursts[i]);
            }
        }
    }

    if (currentCpuBurst == 0)
    {
        //finally, check if the queue is empty
        int c;
        int add = 0;
        for (c = 1; c <= processesRead; ++c)
        {
            add+=bursts[c];
        }

        if (add == 0 && currentCpuBurst < 1)
        {
            currentCpuProcess = 0;

            printf("\n\nT%d:\n",time);
            printf("CPU: <idle>\n");
            printf("QUEUE: <empty>\n");
        }
        else
        {
            //need to switch to next process in the queue; sjf will pick the process with the smallest burst


            //this will get the next shortest burst in the ready queue
            int j;
            int currentShortestBurst;
            int currentProcessNum;
            for (j = 1; j <= processesRead; ++j)
            {
                if (bursts[j] > 0)
                {
                    currentShortestBurst = bursts[j];
                    currentProcessNum = j;
                    break;
                }
            }
            int k;
            for (k = j + 1; k <= processesRead; ++k)
            {
                if (bursts[k] > 0)
                {
                    if (bursts[k] < currentShortestBurst)
                    {
                        currentShortestBurst = bursts[k];
                        currentProcessNum = k;
                    }
                }
            }


            currentCpuProcess = currentProcessNum;
            currentCpuBurst = currentShortestBurst;
            bursts[currentCpuProcess] = 0;

            //print stuff

            printf("\n\nT%d:\n",time);
            printf("CPU: P%d(%d)\n",currentCpuProcess,currentCpuBurst);
            printf("QUEUE: ");

            int i;
            for (i = 1; i <= processesRead; ++i)
            {
                if (bursts[i] != 0)
                {
                    printf("P%d(%d) ",i,bursts[i]);
                }
            }
        }
    }
}


int main(int argc, char *argv[])
{
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
}