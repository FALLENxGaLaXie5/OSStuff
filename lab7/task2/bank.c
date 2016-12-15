/*
 *  bank.c
 *
 *Joshua Steward
 * 10/18/16
 * Lab 7 Task 2
 *
 *
 *  Created by AJ Bieszczad on 3/11/09.
 *  Copyright 2009 CSUCI. All rights reserved.
 *
 *  A bank has customers who can borrow money from a number of accounts.
 *  Then, they have to repay the funds.
 *
 */

#include "bank.h"

int numOfCustomers;        // the number of customers of the bank
int numOfAccounts;         // the number of accounts offered by the bank

int*available;    // the amount available of each customer
int**maximum;     // the maximum demand of each customer
int**allocation;  // the amount currently allocated to each customer
int**need;     // the remaining needs of each customer

/*
 Initialize the bank for a number of accounts and customers.
 */
void initBank(int avail[], int n, int m)
{
   // numOfAccounts is the number of accounts offered by the bank
   numOfCustomers = n;
   numOfAccounts = m;

   // initialize the accounts array
   available = calloc( numOfAccounts, sizeof(int) );
   arraycpy(available, avail, numOfAccounts);

   // create the array for storing the maximum demand by  each customer
   maximum = calloc( numOfCustomers, sizeof(int*) );
   allocation = calloc( numOfCustomers, sizeof(int*) );
   need = calloc( numOfCustomers, sizeof(int*) );
}

/*
 This function adds a customer to the bank system.
 It records its maximum fund demand with the bank.
 */
void addBankCustomer(int customerNum, int maxDemand[])
{
   maximum[customerNum] = calloc( numOfAccounts, sizeof(int) );
   arraycpy(maximum[customerNum], maxDemand, numOfAccounts);

   allocation[customerNum] = calloc( numOfAccounts, sizeof(int) );
   // we start with zero allocated

   need[customerNum] = calloc( numOfAccounts, sizeof(int*) );
   arraycpy(need[customerNum], maxDemand, numOfAccounts);
}

/*
 Outputs the state of the bank; i.e., funds in each account of each customer
 */

void displayBankState()
{
   printf("\n%13s", "Available = ");
   display(available, numOfAccounts);

   printf("\n%13s", "Allocation = ");
   for (int i = 0; i < numOfCustomers; i++)
      display(allocation[i], numOfAccounts);

   printf("\n%13s", "Max = ");
   for (int i = 0; i < numOfCustomers; i++)
      display(maximum[i], numOfAccounts);

   printf("\n%13s", "Need = ");
   for (int i = 0; i < numOfCustomers; i++)
      display(need[i], numOfAccounts);

   printf("\n");
}

/**
 Determines whether granting a request for loan results in leaving
 the system in a safe state or not.
 */
BOOLEAN isSafeState (int customerNum, int request[])
{

   // TBD
    int *availCopy = calloc( numOfAccounts, sizeof(int) );
    arraycpy(availCopy, available, numOfAccounts);

    int ** allocCopy = calloc( numOfCustomers, sizeof(int*) );
    for (int j = 0; j < numOfCustomers; ++j)
    {
        allocCopy[j] = calloc( numOfAccounts, sizeof(int) );
    }

    sub(availCopy,request,numOfAccounts);
    for(int i = 0; i < numOfAccounts; i++)
    {
        if (availCopy[i] < 0)
        {
            return FALSE;
        }
    }
    for(int i = 0; i < numOfAccounts; i++)
    {
        allocCopy[customerNum][i] -= request[i];
        if (allocCopy[customerNum][i] < 0)
        {
            return FALSE;
        }
    }
    //if this is reached, process isn't automatically denied
    //must then determine if new state is safe
   return TRUE;
}

/*
 Make a request for a loan.
 */

BOOLEAN borrow(int customerNum, int funds[])
{
   BOOLEAN ret = isSafeState(customerNum, funds);

   if (ret == TRUE)
   {
      // if it is safe, allocate the funds to customer customerNum
      sub(available, funds, numOfAccounts);
      add(allocation[customerNum], funds, numOfAccounts);
      sub2(need[customerNum], maximum[customerNum], allocation[customerNum], numOfAccounts);
   }

   return ret;
}

/*
 Repay a loan.
 */
BOOLEAN repay(int customerNum, int funds[])
{
   BOOLEAN ret = lessOrSame(funds, allocation[customerNum], numOfAccounts);
   if (ret == TRUE)
   {
      add(available, funds, numOfAccounts);
      sub(allocation[customerNum], funds, numOfAccounts);
      sub2(need[customerNum], maximum[customerNum], allocation[customerNum], numOfAccounts);
   }
   
   return ret;
}