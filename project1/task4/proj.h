/**
 * Joshua Steward
 * Project 1 Task 4
 * 11/3/2016
 **/

#ifndef GLOBAL_GUARD
#define GLOBAL_GUARD

#include <math.h>
#include <stdlib.h>
#include <string.h>
//#include <fuse.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BLOCK_SIZE 256
#define MAX_NAME_LENGTH 128
#define DATA_SIZE 254
#define INDEX_SIZE 127

#define GLOBAL_TABLE_SIZE 65521 // prime number for hashing
#define MAX_OPEN_FILES_PER_PROCESS 16



typedef char data_t;
typedef unsigned short index_t;

typedef enum
{
    DIR,
    FIL,
    INDEX,
    DATA
} NODE_TYPE;

typedef struct fs_node
{
    char name[MAX_NAME_LENGTH];
    //fd - the name is the directory name (path), and the size refers to the numberf of files
    time_t creat_t; // creation time
    time_t access_t; // last access
    time_t mod_t; // last modification
    mode_t access; // access rights for the file
    unsigned short owner; // owner ID
    unsigned short size;
    index_t block_ref; // reference to the data or index block - points to the node which is of type index node
    //within the index node are several file descriptor nodes which either refer to a file or subdirectory

} FS_NODE;


typedef struct node
{
    NODE_TYPE type;
    union
    {
        FS_NODE fd; //file descriptor - this means it is a directory node.
        //fd - the name is the directory name (path), and the size refers to the numberf of files
        //  in the directory
        data_t data[DATA_SIZE]; // unsigned chars representing file data
        index_t index[INDEX_SIZE]; //stores where all the blocks of data for a file are located
        //, since a file might span multiple blocks
    } content;
} NODE;


/**
This is a list of all files (not directories) that exist within the file system

Each node in the table contains the following information:
    A reference to the descriptor to that file (fd node)
    A reference to either the data node (for a small file) or the index node for a large file
    The access rights to the file
    The size of the file
    The number of processes that currently have this file in their per process table (Reference count)
    A reference to the next file in the table (not sure this is necessary unless using a linked list)
*/
typedef struct open_file_global_type // elements of the hash table (in-memory "directory")
{
    unsigned short fd; // reference to the file descriptor node
    //FS_NODE fd;
    unsigned short data; // reference to the data or index node (depending on the size)
    mode_t access; // access rights for the file
    unsigned short size; // size of the file
    unsigned short reference_count; // reference count
    struct open_file_global_type *next;
} OPEN_FILE_GLOBAL_TYPE;

// local table

/**
A list of all the files that one process (or instance of running task 4 code) has open

Each node contains the following:
    The access rights to the file
    A pointer to this file's node witin the global file table.
*/
typedef struct open_file_local_type // a node for a local list of open files (per process)
{
    mode_t access_rights; // access rights for this process
    unsigned short global_ref; // reference to the entry for the file in the global table
} OPEN_FILE_LOCAL_TYPE;

extern NODE *memory;

extern OPEN_FILE_GLOBAL_TYPE global_table[GLOBAL_TABLE_SIZE];

int getFreeBit(char currChar);
char toggleBit(char givChar, int posIndex);
int getFreeBlock_ToggleBit();
void init();
void createFile(int dadIndex);
int openFile(char* name, int filePosition);
void readFile(int perProcessIndex);
void writeFile(int perProcessIndex);
void closeFile(int perProcessIndex);
int hashCalculator(char* name);


#endif