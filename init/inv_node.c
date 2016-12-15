//Joshua Steward
//Comp 362 
//9/1/16
//InIt

#include "inv_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display(INV_NODE *node)
{
	/*
	Prints on one line:
		* The item name
		* The item price formatted as ($#.##)
		* The item quantity
	*/
	printf("%s costs $%lf dollars, and there are %d in stock.\n", node->name, node->price, node->quantity);
}

INV_NODE *add(INV_NODE *list, INV_NODE *node)
{
	//Adds the "node" parameter to the front of the "list" parameter so that "node->next" points to the list and returns the "node" parameter.
	node->next = list;
    return node;
}

INV_NODE *find(INV_NODE *list, char *name)
{
	//Searches through the list of nodes for a node with a name that matches the parameter "name".
	//If a match is found that node is returned.
	//If a match is not found then NULL is returned.
	INV_NODE * currentNode = list;
	while(strncmp(currentNode->name, name, 5) != 0)
	{
		currentNode = currentNode->next;
	}
    return currentNode;
}

int main(void)
{
	/*
	Create an INV_NODE *inventory
	Start reading the input
	If the line is a store input line:
		* Create an *INV_NODE using that line's information.
		* Add the node the inventory
	If the line is a customer input line:
		* Check that the item in the request is in the inventory 
		if it is not, print "Error item not found".
		* Check that there is enough of that item to satisfy the customer's request,
		if there is not, print "Error insufficient item quantity"
		* Check that the price of that number of items does not exceed the customer's money
		if the customer does not have enough money, print "Error insufficient funds"
		* If none of these error conditions occur:
			** Subtract the inv_node's quantity by the request amount.
			** Print "Success! The customer's change is: " Followed by their formatted remaining money ($#.##)
	Go through the entire inventory list and display each node.
	*/
	INV_NODE *inventory = malloc(sizeof(INV_NODE));
	char *fname;
	scanf("%s",fname);
	FILE* file = fopen(fname, "r");

	if (file == NULL)
	{
		return 0;
	}
	//char* line = NULL;

	char myChar;
    char c;

	do
    {
        myChar = fgetc(file);
        if( feof(file) )
        {
            break;
        }
        else if(myChar == 'I')
        {
        	INV_NODE *newNode = malloc(sizeof(INV_NODE));
        	//read the space; if it's not a space, break out
        	fgetc(file);

        	char * name = malloc(sizeof(char));

            while((c = fgetc(file)) != ' ')
        	{
    			size_t len = strlen(name);
    			char *str2 = malloc(len + 2);
    			strcpy(str2, name);
    			str2[len] = c;
    			str2[len + 1] = '\0';
    			name = str2;
    			free( str2 );
        	}

        	//assign the name of the product, then free the placeholder
        	newNode->name = name;
        	free(name);
        	
        	//skip the next space
        	fgetc(file);
        	//grabbing the price as a string, then converting to a double
        	char * price = malloc(sizeof(char));
        	double d;
            while((c = fgetc(file)) != ' ')
        	{
    			size_t len = strlen(price);
    			char *str2 = malloc(len + 2);
    			strcpy(str2, price);
    			str2[len] = c;
    			str2[len + 1] = '\0';
    			price = str2;
    			free( str2 );
        	}
        	sscanf(price, "%lf", &d);
        	//assign the price of the product
        	newNode->price = d;

        	fgetc(file);

        	//grabbing the number as a string, then converting to a double
        	char * num = malloc(sizeof(char));
        	int i;
            while((c = fgetc(file)) != ' ')
        	{
    			size_t len = strlen(num);
    			char *str2 = malloc(len + 2);
    			strcpy(str2, num);
    			str2[len] = c;
    			str2[len + 1] = '\0';
    			num = str2;
    			free( str2 );
        	}
        	sscanf(num, "%d", &i);

        	//assign number to the new node
        	newNode->quantity = i;
        	//add new node to the front of the inventory
        	add(inventory, newNode);

        	if (fgetc(file) == '\n')
        	{
        		continue;
        	}
        }
        else if(myChar == 'C')
        {
			//skipping space
			fgetc(file);
			//getting name
			char * price1 = malloc(sizeof(char));
			double p;
            while((c = fgetc(file)) != ' ')
        	{
    			size_t len = strlen(price1);
    			char *str2 = malloc(len + 2);
    			strcpy(str2, price1);
    			str2[len] = c;
    			str2[len + 1] = '\0';
    			price1 = str2;
    			free( str2 );
        	}

        	//initialize the node we will use to find the item the customer is looking for
        	INV_NODE * customerItem;
        	//convert price string to double
			sscanf(price1, "%lf", &p);
			//skip space, should be at name now
        	fgetc(file);


        	char * name1 = malloc(sizeof(char));
            while((c = fgetc(file)) != ' ')
        	{
    			size_t len = strlen(name1);
    			char *str2 = malloc(len + 2);
    			strcpy(str2, name1);
    			str2[len] = c;
    			str2[len + 1] = '\0';
    			name1 = str2;
    			free( str2 );
        	}

        	//skip space, should be at the quantity now
        	fgetc(file);
        	char * quan1 = malloc(sizeof(char));
        	int q;
            while((c = fgetc(file)) != ' ')
        	{
    			size_t len = strlen(quan1);
    			char *str2 = malloc(len + 2);
    			strcpy(str2, quan1);
    			str2[len] = c;
    			str2[len + 1] = '\0';
    			quan1 = str2;
    			free( str2 );
        	}

        	sscanf(quan1, "%d", &q);

        	int errors = 0;
        	// now we have p (price), q (quantity), and name1 for the customer

        	//check if item is in inventory
        	if ((customerItem = find(inventory, name1)) == NULL)
        	{
        		printf("Error item not found!\n");
        		errors++;
        	}
        	else/*item was found*/
        	{
        		/*
        	* Check that the item in the request is in the inventory 
		if it is not, print "Error item not found".
		* Check that there is enough of that item to satisfy the customer's request,
		if there is not, print "Error insufficient item quantity"
		* Check that the price of that number of items does not exceed the customer's money
		if the customer does not have enough money, print "Error insufficient funds"
		* If none of these error conditions occur:
			** Subtract the inv_node's quantity by the request amount.
			** Print "Success! The customer's change is: " Followed by their formatted remaining money ($#.##)
        */

        		if (customerItem->quantity < q)
        		{
        			printf("Error insufficient item quantity!\n");
        			errors++;
        		}
        		else
        		{
        			/*Do this if there are enough items in the inventory for this customer*/
        			//checking to see if the price of this number of items does not exceed the customer's money
        			int count;
        			double totalPrice = 0.0;
        			for (count = 0; count < q; count++)
        			{
        			 	totalPrice += customerItem->price;
         			}
         			if (p < totalPrice)
         			{
         			 	printf("Error insufficient funds!\n");
         			 	errors++;
         			} 
        		}
        	}

        	if (errors == 0)
        	{
        		customerItem->quantity-= q;
        		printf("Success! The customer's change is: $");
        	}

        }
    }while(1);


    return 0;
}