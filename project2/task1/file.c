/**
 * Joshua Steward
 * Project 2 Task 1
 * 11/29/2016
 **/
#include "disk.h"

disk_t disk;
/**
This function translates a logical to a physical address.
**/
int log_to_phys(int logaddr, physaddr_t *phaddr)
{
    int head = logaddr/(NUM_OF_SECTS * NUM_OF_CYLS);
    int loc = logaddr % (NUM_OF_SECTS * NUM_OF_CYLS);
    int cyl = loc/NUM_OF_SECTS;
    int sect = loc % NUM_OF_SECTS;

    if (cyl < NUM_OF_CYLS && head < NUM_OF_HEADS && sect < NUM_OF_SECTS)
    {
        phaddr->cyl = cyl;
        phaddr->head = head;
        phaddr->sect = sect;
        return 1;
    }
    else
        return 0;
}

/**
This function translates a physical to a logical address.
**/
//LBA = (C × HPC + H) × SPT + (S - 1)
int phys_to_log(physaddr_t *phaddr)
{
    return (phaddr->cyl * NUM_OF_HEADS * phaddr->head) * NUM_OF_SECTS + (phaddr->sect - 1);
}

/**
This function copies num_of_sectors sectors from the disk to the buffer starting with sector pointed to by logical_block_num. You should validate the parameters before serving the request.
**/
int read(int logical_block_num, int num_of_sectors, void **buffer)
{
    printf("Beginning to read...\n");
    physaddr_t myAdd;
    //myAdd now points to the physical address
    for (int i = 0; i < num_of_sectors; i++)
    {
        if (log_to_phys(logical_block_num % MAX_LOGICAL_SECTOR, &myAdd))
        {
            strcat(*buffer, disk[myAdd.head][myAdd.cyl][myAdd.sect]);
        }
        else
        {
            printf("Log to phys didn't return correctly!\n");
            return 0;
        }
        logical_block_num++;
    }
    printf("Reading finished!\n");
    printf("%s\n", (char*)*buffer);
    return 1;
}

/**
This function copies num_of_sectors sectors from the buffer to the disk starting at sector pointed to by logical_block_num. Here, you also should validate the parameters.
**/
int write(int logical_block_num, int num_of_sectors, void *buffer)
{
    printf("Beginning to write...\n");
    int offset = 0;
    physaddr_t myAdd;
    for (int i = 0; i < num_of_sectors; i++)
    {
        if (log_to_phys(logical_block_num % MAX_LOGICAL_SECTOR, &myAdd))
            memcpy(disk[myAdd.head][myAdd.cyl][myAdd.sect], buffer + offset, SECT_SIZE);
        else
        {
            printf("Log to phys didn't return correctly!\n");
            return 0;
        }
        logical_block_num++;
        offset+=SECT_SIZE;
    }
    printf("Write done!\n");
    return 1;
}

char* randstring(size_t length)
{
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char *randomString = NULL;
    if (length)
    {
        randomString = malloc(sizeof(char) * (length +1));
        if (randomString)
        {
            for (int n = 0; n < length;n++)
            {
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }
            randomString[length] = '\0';
        }
    }
    return randomString;
}

int main(int argc, char *argv[])
{
    int i, n, lengthOfString, address = rand() % MAX_LOGICAL_SECTOR;
    time_t t;

    n = 5;


    /* Intializes random number generator */
    srand((unsigned) time(&t));


    printf("Would you like to write a message to memory, or generate a random message to put in memory?\n");
    printf("(w) to write, anything else will generate random string)\n");

    if (getc(stdin) == 'w')
    {
        char str[SECT_SIZE];
        printf("Enter your message:(control+D to finish message)\n");
        //scanf("[%s/n]", str);
        //scanf("%s",str);
        scanf("%20[^\\n]",str);
        lengthOfString = strlen(str);
        printf("%s\n\n", str);

        printf("My first address: %d\n", address);



        int numOfSectors = (strlen(str)/SECT_SIZE) + 1;
        if (strlen(str) == SECT_SIZE)
            numOfSectors--;

        printf("Num of sectors: %d\n", numOfSectors);
        write(address, numOfSectors, (void *)str);
        char *readIn = malloc(sizeof(char) * NUM_OF_SECTS * SECT_SIZE);
        read(address, numOfSectors, (void **)&readIn);

        printf("Message Read: \"%s\"\n\n",readIn);
    }
    else
    {
        //char *myString = "Hi there buddy!";
        printf("Enter the length of the random string you want:\n");

        scanf("%d",&lengthOfString);

        char *myString = randstring(lengthOfString);
        printf("%s", myString);
        int numOfSectors = (strlen(myString)/SECT_SIZE) + 1;
        if (strlen(myString) == SECT_SIZE)
            numOfSectors--;


        write(address, numOfSectors, (void *)myString);
        char *readIn = malloc(sizeof(char) * NUM_OF_SECTS * SECT_SIZE);
        read(address, numOfSectors, (void **)&readIn);

        printf("This was read in from memory address %d: \"%s\"\n\n",address,readIn);
    }
}