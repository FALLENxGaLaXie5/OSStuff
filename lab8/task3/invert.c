//Joshua Steward
//10/25/16
//Lab 8 Task 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invertTabPage.h"

void initInverted(struct invTablePage invTable[100], int memSize, int frameSize){
/*
* allocates and initializes the table for the given sizes of the memory and frame
 * Uses set memory array instead of using dynamic allocation
* to ensure that the library is thread-safe, the table should be a struct that
* includes data such as page size and the number of pages along the translation
* table (that can be a 2-dimensional array, or a one-dimensional array of structs)
*/
	int rPage;
    int pages = 1;
	//will run in increments of each frame size until it hits the maximum memory size
	//initializing the table
	for(int i = 0; i < (memSize/frameSize); i++)
	{
		invTable[i].pageIndex = 0;
		invTable[i].pageNum = 0;
	}


	for(int i = 0; i < (memSize/frameSize); i++)
	{
		printf("%d ", i);

		invTable[i].memorySize = memSize;
		invTable[i].frameSize = frameSize;

		printf("Current page- %d, ", invTable[i].pageNum); //checks if current page being read is empty
		if(invTable[i].pageIndex == 0)
		{
			rPage = rand() % (memSize/frameSize); //generate a random spot to put new page
			if(invTable[rPage].pageNum == 0) //checks if page at randomized spot is empty
			{
			invTable[rPage].pageIndex = rPage; //creates page index at random spot
			invTable[rPage].pid = rand() % memSize; //creates a random pid
			invTable[rPage].pageNum = pages; //creates the sequential pages
			printf("Page # %d Created at %d\n", invTable[rPage].pageNum, invTable[rPage].pageIndex);
			pages++;
			}
		}
		printf("\n" );

	}
	
}

int translate(struct invTablePage invTable[100], int pid, int page, int offset, int index){
/*
* translates a logical address <pid, page, offset> into a physical address using data from invTable page
* if the address is invlid it returns -1
* if there is no entry for the pid and the page number in the table, it has to be added
* if there is no room in the table for the new entry, then the oldest entry has to be removed
*/
	int phys; //physical address
	int invalid = -1;

	printf("Pid: %d - Page number: %d, - ", pid, page);
	invTable[index].physicalAdd = index * invTable[index].frameSize + offset - 1;
	phys = invTable[index].physicalAdd;

	printf("Physical Address: %d - ", phys);
	//if it is a valid location, then we insert the entry into memory
	if(phys < invTable[0].memorySize)
	{
		printf("Valid; adding to physical memory\n\n");
	}
	else
	{
		printf("INVALID LOCATION\n");
	}

	return phys;

}

void releaseInverted(struct invTablePage invTable[100])
{
/*
* releases the inverted table and sets the reference to NULL
*/
	//release all values in page and set to 0
	int memSize = invTable[0].memorySize;
	int frameSize = invTable[0].frameSize;
	for(int i=0; i < (memSize/frameSize); i++)
	{
		invTable[i].pid = 0; 
		invTable[i].pageNum = 0;
		invTable[i].pageIndex = 0;
		invTable[i].timeStamp = 0;
		invTable[i].memorySize = 0;
		invTable[i].frameSize = 0;
	}
	//set memory to null
	invTable = NULL;
}