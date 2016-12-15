/**
 * Joshua Steward
 * Project 1 Task 4
 * 11/3/2016
 **/
#include "proj.h"


OPEN_FILE_GLOBAL_TYPE global_table[GLOBAL_TABLE_SIZE];

OPEN_FILE_LOCAL_TYPE local_table[MAX_OPEN_FILES_PER_PROCESS];



int findOpenLocal()
{
    for (int i = 0; i < MAX_OPEN_FILES_PER_PROCESS; ++i)
    {
        if(local_table[i].global_ref < 1)
        {
            return i;
        }
    }
    return -1;
}

int hashCalculator(char* name)
{
    int tot = 0;
    for (int i = 0; i < strlen(name); ++i)
    {
        tot += (int)name[i];
    }
    return (GLOBAL_TABLE_SIZE % tot);
}

/**
 * IF file is not in global open file table, add to that table.
 * 1. Make sure process has access
 * 2. Add file node to per process file table
 * 3. Update global reference count for that file
 *
 * To hash, pass the name of the file and add up chars, then mod by global table size
 */
int openFile(char* name, int filePosition)
{
    //if process has access to open file
    if (memory[filePosition].content.fd.access >= 0600)
    {
        //get the key by hashing chars in name
        int key = hashCalculator(name);
        if (global_table[key].fd < 1)
        {
            //set fd to the memory position of the file - if less than data size, set it to first index of index block
            //if not, set it to index block reference
            global_table[key].fd = filePosition;
            if (memory[filePosition].content.fd.size <= DATA_SIZE)
            {
                global_table[key].data = memory[filePosition].content.index[0];
            }
            else
            {
                global_table[key].data = memory[filePosition].content.fd.block_ref;
            }


        }
        global_table[key].size = memory[filePosition].content.fd.size;
        int i = findOpenLocal();
        if (i > -1)
        {
            local_table[i].access_rights = memory[filePosition].content.fd.access;
            local_table[i].global_ref = key;
        }
        else
        {
            printf("Reached maximum open file capacity; cannot open any more files!\n");
        }
        //reference count
        global_table[key].reference_count++;
        //set access time to current time
        memory[filePosition].content.fd.access_t = time(0);

        return i;
    }
    else
    {
        return -1;
    }
}

/**
 * For a file in the per process table, get all data nodes belonging to that file
 */
void readFile(int perProcessIndex)
{
    if (local_table[perProcessIndex].access_rights >= 0600)
    {
        OPEN_FILE_GLOBAL_TYPE myGlobal = global_table[(int)local_table[perProcessIndex].global_ref];

        int next = 0;
        while (memory[myGlobal.data].content.index[next] > 0)
        {
            printf("Data Node %d contains: %s\n"
                    , next, memory[memory[myGlobal.data].content.index[next]].content.data);
            next++;
        }
    }
}

/**
 * Essentially take in some data and allocate data nodes for it,
 *      and fill in the index node with the addresses of each of
 *      the new data nodes
 */
void writeFile(int perProcessIndex)
{
    if (local_table[perProcessIndex].access_rights >= 0600)
    {
        OPEN_FILE_GLOBAL_TYPE myGlobal = global_table[(int)local_table[perProcessIndex].global_ref];

        int next = 0;
        while (memory[myGlobal.data].content.index[next] > 0)
        {
            next++;
        }
        //allocate space for new data block
        int newDataLocation = getFreeBlock_ToggleBit();
        memory[newDataLocation].type = DATA;
        memory[myGlobal.data].content.index[next] = newDataLocation;
        //next is now the first free spot in the index node of file
        printf("Enter new data (max 254 chars/bytes):\n");
        char buffer[DATA_SIZE];
        scanf("%s", memory[newDataLocation].content.data);
    }
}

/**
 * Remove from per process table
 * Decrement reference count in global open file table
 * If reference count is now 0, remove from global open file table
 */
void closeFile(int perProcessIndex)
{
    global_table[(int)local_table[perProcessIndex].global_ref].reference_count--;
    if (global_table[(int)local_table[perProcessIndex].global_ref].reference_count < 1)
    {
        global_table[(int)local_table[perProcessIndex].global_ref].fd = 0;
        global_table[(int)local_table[perProcessIndex].global_ref].data = 0;
        global_table[(int)local_table[perProcessIndex].global_ref].access = 0000;
        global_table[(int)local_table[perProcessIndex].global_ref].size = 0;
    }

    local_table[perProcessIndex].access_rights = 0000;
    local_table[perProcessIndex].global_ref = 0;


}

