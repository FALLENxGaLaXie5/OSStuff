/**
 * Joshua Steward
 * Project 1 Task 3
 * 11/3/2016
 **/

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


typedef char data_t;
typedef unsigned short index_t;

int numBlocks = 65536;
int numBits = 8;
int numChars = 65536 / 8;


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

NODE *memory; // allocate 2^16 blocks (in init)
char *bitVector; //allocate space for managing 2^16 blocks (in init)


int getFreeBit(char currChar)
{
    //index will grab the index of the free bit from the right (1s are free space, 0s taken)
    int index = 0;
    if(currChar == 0x00)
    {
        return -1;
    }
    else
    {

        while(!(currChar & 0x01))
        {
            currChar >>= 1;
            index++;
        }
        return index;
    }
}

char toggleBit(char givChar, int posIndex)
{
    givChar ^= 1 << posIndex;
    return givChar;
}

int getFreeBlock_ToggleBit()
{
    int newIndex = 0;
    int i = 0;
    for (i = 0; i < numChars; ++i)
    {
        newIndex = getFreeBit(bitVector[i]);
        if (newIndex > -1)
        {
            bitVector[i] = toggleBit(bitVector[i], newIndex);
            break;
        }
    }
    //i will be the first char with a free space
    // so the block to be assigned memory will be i * 8 (num of bits in a char) +
    return (i * 8) + (newIndex);
}
/**
 * Will create the file system - allocate and initialize all structures and auxiliary data; create the superblock
 *
 *
 */
void init()
{
    bitVector = (char*)malloc(sizeof(char) * numChars);
    memset(bitVector, 0xff, numBlocks/numBits);

    memory = (NODE*)malloc(numBlocks * sizeof(NODE));
    memory[0].type = DIR;
    strcpy(memory[0].content.fd.name,"/");
    memory[0].content.fd.size = 0;
    memory[0].content.fd.owner = -1;
    memory[0].content.fd.creat_t = 0;
    memory[0].content.fd.access = 0;
    memory[0].content.fd.mod_t = 0;
    memory[0].content.fd.access = S_IFDIR | 0755;
    memory[0].content.fd.block_ref = 1;

    //this will allocate for the superblock in the bit vector
    char superChar = toggleBit(bitVector[0],getFreeBit(bitVector[0]));
    bitVector[0] = superChar;

    //allocate for the super index in the bitvector
    char indexBlock = toggleBit(bitVector[0],getFreeBit(bitVector[0]));
    bitVector[0] = indexBlock;

    memory[1].type = INDEX;

    printf("SuperBlock and Super Index created successfully!\n");
}

/**
 * Create a file - allocate on block for meta-level information; set the size to 0,
 * the times to the current time, and the access rights to some default
 *
 * create a fd node for that file filled with the correct default attributes.
 * Note that files will all be empty as of task3. Inserting data happens in task 4
 *
 * size in file descriptor refers to size of file in bytes
 * depending on the size the block reference refers to
 *
 * size <= 254 bytes - a data node containing all of the file's data
 * size > 254 bytes - an index node containing indexes to all data nodes for that file
 */
void createFile(int dadIndex)
{
    if (memory[dadIndex].content.fd.size < INDEX_SIZE && memory[dadIndex].content.fd.access >= 0600)
    {
        int newFileLocation = getFreeBlock_ToggleBit(); //first free block in memory
        printf("Your new file has been created at memory location %d\n", newFileLocation);

        memory[newFileLocation].content.fd.size = 0;
        memory[newFileLocation].content.fd.creat_t = time(0);
        memory[newFileLocation].content.fd.access_t = time(0);
        memory[newFileLocation].content.fd.mod_t = time(0);
        //access rights - default
        memory[newFileLocation].content.fd.access = 0600;
        memory[newFileLocation].type = FIL;

        //setting up index block for new file
        int newFileIndexLoc = getFreeBlock_ToggleBit();
        memory[newFileIndexLoc].type = INDEX;

        //link file's index memory block to its reference. This will be used to access the newly created
        //  file's index block; and therefore contents based on overflow
        memory[newFileLocation].content.fd.block_ref = newFileIndexLoc;
        printf("this is the files index block position %d\n", newFileIndexLoc);

        //add to parent's index node's array and increase parent's size
        //will used accessor to reference on parent index to add this file's location to parent's index array
        memory[memory[dadIndex].content.fd.block_ref].content.index[memory[dadIndex].content.fd.size] = newFileLocation;
        memory[dadIndex].content.fd.size++; //increments parent index size
    }
    else
    {
        printf("File creation failed: either you don't have permissions or dad has taken up too much memory!\n");
    }
}

/**
 * Just like creating a file, but with a different type
 */
void createDirectory(int dadIndex)
{
    if (memory[dadIndex].content.fd.size < INDEX_SIZE && memory[dadIndex].content.fd.access >= 0600)
    {
        int newDirLocation = getFreeBlock_ToggleBit(); //first free block in memory
        printf("Your new directory has been created at memory location %d\n", newDirLocation);

        memory[newDirLocation].content.fd.size = 0;
        memory[newDirLocation].content.fd.creat_t = time(0);
        memory[newDirLocation].content.fd.access_t = time(0);
        memory[newDirLocation].content.fd.mod_t = time(0);
        //access rights - default
        memory[newDirLocation].content.fd.access = 0600;
        memory[newDirLocation].type = DIR;

        //setting up index block for new file
        int newFileIndexLoc = getFreeBlock_ToggleBit();
        memory[newFileIndexLoc].type = INDEX;

        //link file's index memory block to its reference. This will be used to access the newly created
        //  file's index block; and therefore contents based on overflow
        memory[newDirLocation].content.fd.block_ref = newFileIndexLoc;
        printf("this is the new directory's index block position %d\n", newFileIndexLoc);

        //add to parent's index node's array and increase parent's size
        //will used accessor to reference on parent index to add this file's location to parent's index array
        memory[memory[dadIndex].content.fd.block_ref].content.index[memory[dadIndex].content.fd.size] = newDirLocation;
        memory[dadIndex].content.fd.size++; //increments parent index size
    }
    else
    {
        printf("Directory creation failed: either you don't have permissions or dad has taken up too much memory!\n");
    }
}

/**
 * Delete a file, return blocks and clean up your supporting structures; e.g., reset the bits in the bit vector
 * - Get rid of all the data, index, and the file descriptor nodes for a file
 *
 * Will grab the offset from the char because we are counting from the right of each char in bits
 */
void deleteFile(int filePosition)
{
    //delete file node
    //the char in the bit vector with this file is at the position / number of bits in a char
    char currentChar = bitVector[filePosition/numBits];
    //offset from the rightmost bit will be the remainder of this division
    int offset = (filePosition % numBits);
    char newChar = toggleBit(currentChar, offset);
    //sets bit vector position with previous file location to reflect newly flipped bit
    bitVector[filePosition/numBits] = newChar;
    printf("Deleted the file at memory location: %d\n", filePosition);

    //delete it's index block
    //will grab memory block by the position's index block reference in the file descriptor
    filePosition = memory[filePosition].content.fd.block_ref;
    currentChar = bitVector[filePosition/numBits];
    offset = (filePosition % numBits);
    newChar = toggleBit(currentChar, offset);
    bitVector[filePosition/numBits] = newChar;
    printf("Deleted file index block location %d\n", filePosition);
}

/**
 * Delete the files from the directory, and then delete the directory; clean up
 */
void deleteDirectory(int dirPosition)
{
    index_t indexBlockLocation = memory[dirPosition].content.fd.block_ref;

    //iterates through all possible indexes in directory in case some have been deleted
    //this will delete all files from the directory (clean up)
    for(int i = 0; i < INDEX_SIZE; i++)
    {
        index_t currIndex = memory[(int)indexBlockLocation].content.index[i];
        if(currIndex != 0)
        {
            //if it's a file, delete, if directory, call recursively to delete inner directory
            if(memory[(int)currIndex].type == FIL)
            {
                deleteFile(currIndex);
            }
            else if(memory[(int)currIndex].type == DIR)
            {
                deleteDirectory((int)currIndex);
            }
        }
    }

    //delete directory itself
    //same as deleting file
    char dirGiven = bitVector[dirPosition/numBits];
    int bitVectorPosDir = (dirPosition % numBits);
    char newCharDir = toggleBit(dirGiven, bitVectorPosDir);
    bitVector[dirPosition/numBits] = newCharDir;
    printf("Deleted dir at location %d\n", dirPosition);

    //delete directory's index block
    memset(memory[indexBlockLocation].content.index, 0, INDEX_SIZE);
    char dirIndexBlockGiven = bitVector[indexBlockLocation/numBits];
    int bitVectorPosIndex = (indexBlockLocation % numBits);
    char newCharIndex = toggleBit(dirIndexBlockGiven, bitVectorPosIndex);
    bitVector[indexBlockLocation/numBits] = newCharIndex;
    printf("Deleted dir index at location %d\n", indexBlockLocation);
}

/**
 * Obtain file type - file or directory, size, times, access right, owner
 * Print out a fd_node's attributes
 *
 * time_t creat_t; // creation time
    time_t access_t; // last access
    time_t mod_t; // last modification
    mode_t access; // access rights for the file
    unsigned short owner; // owner ID
 */
void obtainFileInfo(int filePos)
{
    printf("Type: %d\nName: %s\nSize: %hu\nOwner: %hu\nCreation time: %ld\nLast Access Time: %ld\nLast Modification: %ld\nAccess Rights: (%3o)\n"
            , memory[filePos].type, memory[filePos].content.fd.name, memory[filePos].content.fd.size, memory[filePos].content.fd.owner
            , memory[filePos].content.fd.creat_t, memory[filePos].content.fd.access_t, memory[filePos].content.fd.mod_t
            ,memory[filePos].content.fd.access);
}


int main(int argc, char *argv[])
{
    init();
    createDirectory(0);
    createDirectory(0);
    obtainFileInfo(2);
    obtainFileInfo(4);
    createFile(2);
    createFile(2);
    createFile(2);
    createFile(4);
    createFile(4);
    createFile(4);
    deleteDirectory(4);
    deleteFile(6);
    //return fuse_main(argc, argv, &hello_oper, NULL);
}