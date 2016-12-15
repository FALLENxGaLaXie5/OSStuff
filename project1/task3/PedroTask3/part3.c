#include "memory.h"

void init()
{
   bitvector = malloc(sizeof(char) * 65536/8);
   memset(bitvector, 0xff, 65536/8);

   memory = malloc(sizeof(NODE) * 65536);
   
   memory[0].type = DIRTYPE;
   strcpy(memory[0].content.fd.name,"/");
   memory[0].content.fd.creat_t = 0;
   memory[0].content.fd.access_t = 0;
   memory[0].content.fd.mod_t = 0;
   memory[0].content.fd.access = S_IFDIR | 0755; //still have to set DONT FORGET
   memory[0].content.fd.owner = -1;
   memory[0].content.fd.size = 0;
   memory[0].content.fd.block_ref = 1;

   //allocate for root
   char root = toggleBit(bitvector[0],getFreeBitIndex(bitvector[0]));
   bitvector[0] = root;

   //allocate for index
   char index = toggleBit(bitvector[0],getFreeBitIndex(bitvector[0]));
   bitvector[0] = index;

   memory[1].type = INDEXTYPE;
}

int getFreeBitIndex(unsigned char given)
{
   int i = 0;
   if(given == 0x00)
   {
      return -1;
   }
   else
   {
      while(!(given & 1))
      {
         given >>= 1;
         i++;
      }
      return i;
   }
}

char toggleBit(unsigned char giv, int pos)
{
   giv ^= 1 << pos;
   return giv;
}

int findFreeBlock()
{
   int i = 0; 
   int givenIndex = 0;
   for(i = 0; i < 65536/8; i++)
   {
      //printf("givenIndex: %d\n", bitvector[i]);
      givenIndex = getFreeBitIndex(bitvector[i]);
      if(givenIndex != -1)
      {

         char newChar = toggleBit(bitvector[i], givenIndex);
         bitvector[i] = newChar;
         break;
      }
   }
   return (i * 8) + (givenIndex);
}

void createFile(int parentsIndex) 
{
   if(memory[parentsIndex].content.fd.size < 127 && memory[parentsIndex].content.fd.access >= 0600) //set correct permission level
   {
      int location = findFreeBlock();
      printf("created file at index in mem at %d\n", location);
      memory[location].type = FILETYPE;
      memory[location].content.fd.creat_t = time(0);
      memory[location].content.fd.size = 0;
      memory[location].content.fd.access = 0600;

      //set up files index block
      int forIndex = findFreeBlock();
      memory[forIndex].type = INDEXTYPE;

      //link file's memory block to its ref..
      memory[location].content.fd.block_ref = forIndex;
      printf("this is the files index block position %d\n", forIndex);

      //add to parent's index node's array and increase parent's size
      memory[memory[parentsIndex].content.fd.block_ref].content.index[memory[parentsIndex].content.fd.size] = location;
      memory[parentsIndex].content.fd.size++;
   }
   else
   {
      printf("Couldn't create file- Check if max size met or permissions\n");
   }

}

void createDir(int parentsIndex) 
{
   if(memory[parentsIndex].content.fd.size < 127 && memory[parentsIndex].content.fd.access >= 0600) //set correct permission level
   {
      int location = findFreeBlock();
      printf("created dir at index in mem at %d\n", location);
      memory[location].type = DIRTYPE;
      memory[location].content.fd.creat_t = time(0);
      memory[location].content.fd.size = 0;
      memory[location].content.fd.access = 0600;

      //set up directorie's index block
      int forIndex = findFreeBlock();
      memory[forIndex].type = INDEXTYPE;

      //link directory's memory block to its ref..
      memory[location].content.fd.block_ref = forIndex;

      //add to parent's index node's array and increase parent's size
      memory[memory[parentsIndex].content.fd.block_ref].content.index[memory[parentsIndex].content.fd.size] = location;
      memory[parentsIndex].content.fd.size++;
   }
   else
   {
      printf("Couldn't create directory- Check if max size met or permissions\n");
   }
}

void deleteFile(int position)
{
   //delete file node
   char current = bitvector[position/8];
   int bitVectorPos = (position % 8);
   char newChar = toggleBit(current, bitVectorPos);
   bitvector[position/8] = newChar;
   printf("Deleted file at location %d\n", position);

   //delete it's index block
   position = memory[position].content.fd.block_ref;
   current = bitvector[position/8];
   bitVectorPos = (position % 8);
   newChar = toggleBit(current, bitVectorPos);
   bitvector[position/8] = newChar;
   printf("Deleted file index block location %d\n", position);
}

void deleteDir(int position)
{
   short indexBlockLocation = memory[position].content.fd.block_ref;

   //delete files from directory
   int i = 0;
   for(i = 0; i < INDEX_SIZE; i++)
   {
      short currIndex = memory[indexBlockLocation].content.index[i];
      if(currIndex != 0)
      {
         if(memory[currIndex].type == FILETYPE)
         {
            deleteFile(currIndex);
         }
         else if(memory[currIndex].type == DIRTYPE)
         {
            deleteDir(currIndex);
         }
      }
   }

   //delete directory
   char dirGiven = bitvector[position/8];
   int bitVectorPosDir = (position % 8);
   char newCharDir = toggleBit(dirGiven, bitVectorPosDir);
   bitvector[position/8] = newCharDir;
   printf("Deleted dir at location %d\n", position);

   //delete directorie's index block
   memset(memory[indexBlockLocation].content.index, 0, INDEX_SIZE);
   char dirIndexBlockGiven = bitvector[indexBlockLocation/8];
   int bitVectorPosIndex = (indexBlockLocation % 8);
   char newCharIndex = toggleBit(dirIndexBlockGiven, bitVectorPosIndex);
   bitvector[indexBlockLocation/8] = newCharIndex;
   printf("Deleted dir index at location %d\n", indexBlockLocation);
}

NODE obtainInfo(int position)
{
   return memory[position];
}

int main()
{
   init();
   createDir(0);
   createFile(2);
   createFile(2);
   createFile(2);
   createFile(2);
   createFile(2);
   createFile(2);
   createFile(2); //8
   deleteFile(4);
   createFile(2);
   createFile(2);
   printf("after this we delete the dir\n");
   deleteDir(2);
   createDir(0);
   createDir(2);
   createFile(6);
   createFile(6);
   createFile(6);
   createFile(2);
   deleteDir(2);


   return 0;
}
