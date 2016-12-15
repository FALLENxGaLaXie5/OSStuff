/*
Joshua Steward
9/8/16
Lab 2 Task 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int checkIfHelp(char* helpArgument)
{
    //check if argument is -h
    char argName[10];
    strcpy(argName, "-h");
    return strcmp(argName, helpArgument);
}

int checkIfUtIsSub(char* utilityArgument)
{
    //check if utility name is sub
    char utilityName[10];
    strcpy(utilityName, "sub.c");
    return strcmp(utilityName, utilityArgument);
}

int checkIfForceIn(char* utilityArgument)
{
    //check if argument is -f
    char argNameForce[10];
    strcpy(argNameForce, "-f");
    return strcmp(argNameForce, utilityArgument);
}

int checkIfForceOut(char* utilityArgument)
{
    char argNameForce[10];
    strcpy(argNameForce, "-o");
    return strcmp(argNameForce, utilityArgument);
}

int checkIfInputRedirection(char* utilityArgument)
{
    char argNameForce[10];
    strcpy(argNameForce, "<");
    return strcmp(argNameForce, utilityArgument);
}

int checkIfOutputRedirection(char* utilityArgument)
{
    char argNameForce[10];
    strcpy(argNameForce, ">");
    return strcmp(argNameForce, utilityArgument);
}

char* getPathName(char* fullPath, int num)
{
    int numPassed = 0;
    int i = 0;
    char c[2];
    char *string;

    string = (char*) calloc(1, sizeof(char));
    do
    {
        c[0] = fullPath[i];
        c[1] = '\0';
        if (c[0] == '/')
        {
            numPassed++;
        }
        if(c[0] != '\n')
        {
            strcat(string, c);
        }
    }while(numPassed < num);

    return string;
}

char* getFileFromPath(char* fullPath)
{
    char* copy;
    strcpy(copy, fullPath);
    const char s[2] = "/";
    char* token;
    token = strtok(copy, s);
    while(token != NULL)
    {
        token = strtok(NULL, s);
    }
    return token;
}

int getNumOfDel(char* fullPath)
{
    int num = 0;
    int len = strlen(fullPath);
    char c;
    int i;
    for (i = 0; i < len; i++)
    {
        c = fullPath[i];
        if (c == '/')
        {
            num++;
        }
    }
    return num;
}

int main(int argc, char* argv[])
{
    //if there are 2 arguments, the name of the argument is sub, and the argument is -h
    if (argc == 2 && checkIfUtIsSub(argv[0]) == 0 && checkIfHelp(argv[1]) == 0)
    {
        printf("Usage: sub fromChar toChar srcFile destFile\n");
    }
    else if (argc == 7 && checkIfUtIsSub(argv[0]) == 0)
    {
        //this either means it is using regular invocation with a src file and dest file, 
        //or, input and output redirection

        //if force input, then it is a regular force input and force output invocation
        if (checkIfForceIn(argv[3]) == 0 && checkIfForceOut(argv[5]))
        {
            char* firstArgument = argv[1];
            char* secondArgument = argv[2];
            char replaceMe = firstArgument[0];
            char replaceWith = secondArgument[0];
            
            char* srcFileName;
            srcFileName = argv[4];
            
            FILE *inputFile;
            inputFile = fopen(srcFileName, "r");
            
            char* outputFileName;
            outputFileName = argv[6];
            
            FILE *outputFile;
            outputFile = fopen(outputFileName, "w");
            
            int c;
            
            while (1)
            {
                c = fgetc(inputFile);
                if (feof(inputFile))
                {
                    break;
                }
                else if (c == replaceMe)
                {
                    fputc(replaceWith, outputFile);
                }
                else if (c != replaceMe)
                {
                    fputc(c, outputFile);
                }
            }
            fclose(inputFile);
            fclose(outputFile);

        }
        else if (checkIfInputRedirection(argv[3]) == 0 && checkIfOutputRedirection(argv[5]) == 0)
        {
            //if input redirection with 7 arguments and output is redirected, do this

            char* firstArgument = argv[1];
            char* secondArgument = argv[2];
            char replaceMe = firstArgument[0];
            char replaceWith = secondArgument[0];
            int s;
            char* srcFileName;
            srcFileName = getFileFromPath(argv[4]);
            
            s = chdir(getPathName(argv[4], getNumOfDel(argv[4])));
            FILE *inputFile;
            inputFile = fopen(srcFileName, "r");
            
            char* outputFileName;
            outputFileName = getFileFromPath(argv[6]);


            s = chdir(getPathName(argv[6], getNumOfDel(argv[6])));
            FILE *outputFile;
            outputFile = fopen(outputFileName, "w");
            
            int c;
            
            while (1)
            {
                s = chdir(getPathName(argv[4], getNumOfDel(argv[4])));
                c = fgetc(inputFile);
                if (feof(inputFile))
                {
                    break;
                }
                else if (c == replaceMe)
                {
                    s = chdir(getPathName(argv[6], getNumOfDel(argv[6])));
                    fputc(replaceWith, outputFile);
                }
                else if (c != replaceMe)
                {
                    s = chdir(getPathName(argv[6], getNumOfDel(argv[6])));
                    fputc(c, outputFile);
                }
            }

            s = chdir(getPathName(argv[4], getNumOfDel(argv[4])));
            fclose(inputFile);
            s = chdir(getPathName(argv[6], getNumOfDel(argv[6])));
            fclose(outputFile);
        }
    }
    else if (argc == 5 && checkIfUtIsSub(argv[0]) == 0)
    {
        //5 arguments

        if (checkIfForceIn(argv[3]) == 0)
        {
            //if the src file is forced, then the standard output is used as destination
            char* firstArgument = argv[1];
            char* secondArgument = argv[2];
            char replaceMe = firstArgument[0];
            char replaceWith = secondArgument[0];
            
            char* srcFileName;
            srcFileName = argv[4];
            
            FILE *inputFile;
            inputFile = fopen(srcFileName, "r");
            
            
            int c;
            
            while (1)
            {
                c = fgetc(inputFile);
                if (feof(inputFile))
                {
                    break;
                }
                else if (c == replaceMe)
                {
                    fputc(replaceWith, stdout);
                }
                else if (c != replaceMe)
                {
                    fputc(c, stdout);
                }
            }
            fclose(inputFile);
        }
        else if (checkIfForceOut(argv[3]) == 0)
        {
            //if the dest file is forced, then the standard input is used as the source

            char* firstArgument = argv[1];
            char* secondArgument = argv[2];
            char replaceMe = firstArgument[0];
            char replaceWith = secondArgument[0];
            
            char* outputFileName;
            outputFileName = argv[4];
            
            FILE *outputFile;
            outputFile = fopen(outputFileName, "w");
            
            int c;
            
            while (1)
            {
                c = fgetc(stdin);
                if (feof(stdin))
                {
                    break;
                }
                else if (c == replaceMe)
                {
                    fputc(replaceWith, outputFile);
                }
                else if (c != replaceMe)
                {
                    fputc(c, outputFile);
                }
            }

            fclose(outputFile);
        }
        else if (checkIfInputRedirection(argv[3]) == 0)
        {
            //input is redirected from stdin to a specified file path, and copied to standard output
            char* firstArgument = argv[1];
            char* secondArgument = argv[2];
            char replaceMe = firstArgument[0];
            char replaceWith = secondArgument[0];
            
            char* srcFileName;
            srcFileName = getFileFromPath(argv[4]);


            int s = chdir(getPathName(argv[4], getNumOfDel(argv[4])));
            FILE *inputFile;
            inputFile = fopen(srcFileName, "r");
            
            
            int c;
            
            while (1)
            {
                c = fgetc(inputFile);
                if (feof(inputFile))
                {
                    break;
                }
                else if (c == replaceMe)
                {
                    fputc(replaceWith, stdout);
                }
                else if (c != replaceMe)
                {
                    fputc(c, stdout);
                }
            }
            fclose(inputFile);
        }
    }
    else if (argc == 3 && checkIfUtIsSub(argv[0]) == 0)
    {
        //3 arguments, so data is read from stdin and copied to standard output
        
        char* firstArgument = argv[1];
        char* secondArgument = argv[2];
        char replaceMe = firstArgument[0];
        char replaceWith = secondArgument[0];
        
        int c;
        
        while (1)
        {
            c = fgetc(stdin);
            if (feof(stdin))
            {
                break;
            }
            else if (c == replaceMe)
            {
                fputc(replaceWith, stdout);
            }
            else if (c != replaceMe)
            {
                fputc(c, stdout);
            }
        }
    }
}