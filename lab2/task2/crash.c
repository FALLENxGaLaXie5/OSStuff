/*
Joshua Steward
9/8/16
Lab 2 Task 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct name
{
    char *first, *last;
} NAME;

typedef NAME        *NAME_PTR;

void get_name(NAME_PTR);

int main(void)
{
    NAME_PTR me = malloc(sizeof(NAME_PTR));
    //me->first = (char *)malloc(sizeof(char));
    //me->last = (char *)malloc(sizeof(char));
    //me = (NAME_PTR) malloc(sizeof(NAME_PTR));
    get_name(me);
    printf("Hello %s %s!\n", me->first, me->last);
    free(me);
}

void get_name(NAME_PTR ptr)
{
    ptr->first = (char *)malloc(sizeof(char));
    ptr->last = (char *)malloc(sizeof(char));
    printf("What's your first name?\n");
    scanf("%s", ptr->first);
    //fgets(ptr->first, 10, stdin);
    printf("What's your last name?\n");
    scanf("%s", ptr->last);
    //fgets(ptr->first, 10, stdin);

}