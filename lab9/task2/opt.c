/*
 *Joshua Steward
 *Lab 9 Task 2
 *11/1/16
 */


//OPT

#include "replace.h"

// the number of elements in the page list
int newVictim = 0;
int currentVictim = 0;
int potentialVictim =0;

//keeps track of table data
int numberOfFaults = 0;
int boolCompare = 0;
int currOldest;


// this is the sequence of pages to serve
int *currString;
int len;
int ind[100]; // to save location of which page wont be used for longest

// instantiating the page table array,
// a bool to let us know if it's full, a current count of pages
int *pageTable;
int numberPages;
int full = 0;


void displayO(void);
void insertO(int, int);
int search(int);
int findNew(int);
void update(int,int);

/*
 * insert pages from a reference string into a page table and measure
 * the page fault rate
 */
int testOPT(int n, int *ref, int refStringLen)
{
	numberPages = n;
	currString = ref;
	len = refStringLen;
	
	pageTable = calloc(numberPages, sizeof(int));
	
	// we use an impossible page reference of -1 as an initial value
	memset(pageTable, -1, numberPages * sizeof(int));
	newVictim = 0;
	
	#if DEBUGLEVEL > 2
		printf("   ");
	#endif
	   
	#if DEBUGLEVEL > 0
		displayO();
	#endif
   
	//inserting page
	for (int i = 0; i < len; ++i)
    {
			insertO(currString[i], i);
			#if DEBUGLEVEL > 2
				printf("{%3d}", currString[i]);
			#endif
	      
			#if DEBUGLEVEL > 0
				displayO();
			#endif
	}
   
	#if DEBUGLEVEL > 2
		printf("      ");
	#endif
	   
	#if DEBUGLEVEL > 0
		displayO();
	#endif
   
	return numberOfFaults;
}

/**
 **	attempt to insert
 **/
void insertO(int page, int pageIndex)
{
	int searchVal = search(page);
	if(searchVal ==-1 && full >= numberPages)
	{
		int theVictim = findNew(pageIndex);
		//update page table since we found the victim we want to replace with
		update(theVictim,page);
	}
	else if (searchVal == -1)
    {
		pageTable[newVictim] = page;
		full++;
		newVictim++;
		//findNewVictim(pageIndex);
		#if DEBUGLEVEL > 2
			printf("*");
		#endif
	}
	else
    {
		pageTable[newVictim] = page;
		numberOfFaults++;
		full++;
		newVictim++;
		#if DEBUGLEVEL > 2
			printf(" ");
		#endif
		//the search value found was in table so we can just skip and go on to the next one
    }
}

int findNew(int indexSym)
{
    //algorithm for optimum performance
    //initialization
	int skip = 0;
	int notFound = 1;
	int k = 0;
	
    //increment index
	indexSym++;

	for(int i = 0; i < numberPages; ++i)
	{
		notFound = 1;
		for(int j = indexSym; j < len && notFound == 1;j++)
		{
			if(currString[j] == pageTable[i])
			{
				notFound = 0;
				ind[k] = j;
                //saving value to compare to
				k++;
			}
		}
		if(notFound == 1)
		{
            //found victim frame
			currentVictim = i;
			boolCompare = 1;
		}
	}
	if(boolCompare == 0)
	{
		currOldest = ind[0];
		currentVictim = k;
		for(k = 1; k < (sizeof(ind) / sizeof(int));k++)
		{
			if(ind[k]> currOldest) // if the index location that we saved is bigger than the oldest one
			{
				currOldest = ind[k]; // then its the current oldest so that page wont be used for a long time
				currentVictim = k;
                // so at location k that is our victim
				
			}
			
		}
		
	}

	return currentVictim;

	

}
/**
 * Updates the victim frame with the page number given
 */
void update(int theVictimIndex,int pageNum)
{
	int replace;
	int found = 0;
    //will loop through the page table until we find the index
	for(int i = 0; i < numberPages && found == 0; ++i)
	{
		if(i == theVictimIndex)
		{	
			pageTable[i] = pageNum; 
			found = 1;
		}
	}
	
	
}

/**
 * This will search for the given page number in the list
 * It will either return the index of the page # or -1 if it wasnt found
 */
int search(int page)
{
	for (int i = 0; i < numberPages; ++i)
    {
        if (page == pageTable[i])
        {
            return i;
        }
    }
	return -1;
}

void displayO()
{
	for (int i = 0; i < numberPages; i++)
    {
		#if DEBUGLEVEL > 1
			printf("%s", ((i == newVictim) ? " ->" : "   "));
		#endif
		printf("[%3d]", pageTable[i]);
	}
	printf("\n");
}


