#ifndef MEMORY_H_
#define MEMORY_H_

#define BLOCK_SIZE 256
#define MAX_NAME_LENGTH 128
#define DATA_SIZE 254
#define INDEX_SIZE 127

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

typedef enum
{
   DIRTYPE, 
   FILETYPE, 
   INDEXTYPE,
   DATATYPE
} NODE_TYPE;

typedef struct data_t
{
   int size;
   void *data;
} data_t;

typedef struct fs_node
{
   char name[MAX_NAME_LENGTH];
   time_t creat_t; // creation time
   time_t access_t; // last access
   time_t mod_t; // last modification
   mode_t access; // access rights for the file
   unsigned short owner; // owner ID
   unsigned short size;
   unsigned short block_ref; // reference to the data or index block
} FS_NODE;

typedef struct node
{
   NODE_TYPE type;
   union
   {
      FS_NODE fd;
      data_t data[DATA_SIZE];
      unsigned short index[INDEX_SIZE];
   } content;
} NODE;

// storage blocks
NODE *memory; // allocate 2^16 blocks (in init)
unsigned char *bitvector; // allocate space for managing 2^16 blocks (in init) (size is 8192)

int getFreeBitIndex(unsigned char);
char toggleBit(unsigned char, int);

#endif // MEMORY_H_