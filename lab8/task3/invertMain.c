/*
 Joshua Steward
 10/25/16
 Lab 8 task 3
 */

#include <stdio.h>
#include <stdlib.h>
#include "invertTabPage.h"

extern void initInverted(struct invTablePage *invertTable, int memorySize, int frameSize);
extern int translate(struct invTablePage *invTable, int pid, int page, int offset, int index);
extern void releaseInverted(struct invTablePage *invertTable);

int main(int argc, char **argv)
{
	//assuming set computer architecture
	int memorySize = 40;
	int frameSize = 4;
	int pagesize = frameSize;
	//with a 12 bit CPU, we offset it by the pagesize
	int bitCPU = 12;
	int offset =  bitCPU - pagesize;

	//this will initialize an array of structure pages
	struct invTablePage invertTable[memorySize/frameSize];
	initInverted(invertTable, memorySize, frameSize);

	for(int i=0; i < (memorySize/frameSize); i++)
	{
		if(invertTable[i].pageNum != 0)
		{
			//translate randomly generated addresses into physical equivalents from static library
			translate(invertTable, invertTable[i].pid, invertTable[i].pageNum, offset, invertTable[i].pageIndex);
			invertTable[i].timeStamp = i;
		}
	}
	releaseInverted(invertTable);
}