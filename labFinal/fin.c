/*
Joshua Steward
12/15/2016
Lab Final
*/

#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#define MAX_ITEMS 10
#define MAXSIZE 500
//gcc creationEfficiency.c -o creationEfficiency -lpthread


typedef struct
{
    //itemType type;
    char name[10];
    double price;
    int quantity;
}item;

typedef struct
{
    int numCustomers;//only for the first customer in each lane
    item myItems[MAX_ITEMS];
    int size;
}customer;



void *fcfsThread(void *cust)
{
    customer *customers = cust;
    int numCustomers = customers[0].numCustomers;
    printf("Num of customers in this lane is %d\n", numCustomers);

    double totalMoney = 0.0;
    for (int i = 1; i < numCustomers; ++i)
    {
        double custAmount = 0.0;
        //processing over all items in each customer's cart
        for (int j = 0; j < customers[i].size; ++j)
        {
            //processing over amount of individual items individually
            for (int k = 0; k < customers[i].myItems[j].quantity; ++k)
            {
                totalMoney += customers[i].myItems[j].price;
                custAmount += customers[i].myItems[j].price;
            }
        }
        printf("Customer %d finished - total price was $%lf\n", i, custAmount);
    }
    printf("Total for this lane: %lf\n", totalMoney);
}

void *expressThread(void *cust)
{
    customer *customers = cust;

    int numCustomers = customers[0].numCustomers;
    printf("Num of customers in this lane is %d\n", numCustomers);

    double totalMoney = 0.0;

    customer smallestCustomer = customers[1];
    while (numCustomers > 0)
    {
        //find smallest customer cart

        //int firstQuantity = 0;

        int quantity1 = 0;
        for (int j = 0; j < smallestCustomer.size; ++j)
        {
            quantity1 += smallestCustomer.myItems[j].quantity;
        }

        for (int i = 2; i < numCustomers; ++i)
        {
            int quantity = 0;
            for (int j = 0; j < customers[i].size; ++j)
            {
                quantity += customers[i].myItems[j].quantity;
            }
            if(quantity < quantity1)
            {
                smallestCustomer = customers[i];
                quantity1 = quantity;
                //customers[i] = NULL;
                for (int j = i; j < numCustomers; ++j)
                {
                    customers[j] = customers[j + 1];
                }
            }
        }


        double custAmount = 0.0;
        //processing over all items in each customer's cart
        for (int j = 0; j < smallestCustomer.size; ++j)
        {
            //processing over amount of individual items individually
            for (int k = 0; k < smallestCustomer.myItems[j].quantity; ++k)
            {
                totalMoney += smallestCustomer.myItems[j].price;
                custAmount += smallestCustomer.myItems[j].price;
            }
        }
        printf("Customer finished - total price was $%lf\n", custAmount);
        smallestCustomer = customers[1];
        numCustomers--;
    }
    printf("Total for this lane: %lf\n", totalMoney);
}

int main(int argc, char *argv[])
{
    FILE *fp = freopen(argv[1], "r", stdin);
    if (fp == NULL)
    {
        printf("Oops! This file sucks!\n");
        exit(-1);
    }


    int L;
    //printf("Enter # of open lanes:\n");
    scanf("%d",&L);
    printf("Number of open lanes is: %d\n", L);

    //will contain the # of people in each lane
    int lanes[L];

    int rc;
    int numItems;

    for (int i = 0; i < L; ++i)
    {
        //printf("Enter number of carts in lane %d\n", i);
        scanf("%d", &lanes[i]);
        customer *customers = malloc(sizeof(customer) * (lanes[i]) + 1);
        //this will iterate for each lane, for the number of customers per lane
        //first customer will be the "cashier" - will count the number of customers in the lane
        customers[0].numCustomers = lanes[i];
        for (int j = 1; j <= lanes[i]; ++j)
        {
            //create new customer
            customer newCustomer;
            //this will grab the number of items from each customer, then iterate through each item storing it
            scanf("%d",&numItems);
            newCustomer.size = numItems;
            for (int k = 0; k < numItems; ++k)
            {
                item newItem;
                scanf("%s %lf %d", newItem.name, &newItem.price, &newItem.quantity);
                //printf("%s %lf %d\n", newItem.name, newItem.price, newItem.quantity);
                newCustomer.myItems[k] = newItem;
            }
            customers[j] = newCustomer;

            printf("Customer %d\n", j);
            for (int l = 0; l < numItems; ++l)
            {
                printf("%s %lf %d\n", customers[j].myItems[l].name, customers[j].myItems[l].price, customers[j].myItems[l].quantity);
            }

        }

        if (i % 2 == 0)
        {
            //even, so FCFS
            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            rc = pthread_create(&tid, &attr, fcfsThread, customers);
        }
        else
        {
            //odd, so EXPRESS
            pthread_t tid;
            pthread_attr_t attr;
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            rc = pthread_create(&tid, &attr, expressThread, customers);
        }
    }
}