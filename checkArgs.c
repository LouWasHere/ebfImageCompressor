#include <stdio.h>

#include "checkArgs.h"

#define SUCCESS 0
#define BAD_ARGS 1
#define MISC_ERROR 100

int check_arguments(int argc, char **argv)
    {
    // validate that user has entered 2 arguments (plus the executable name)
    // first check if the user has entered no values and correct them on the correct usage if so
    if(argc == 1)
        {
        // remove ./ if it exists
        if(argv[0][0] == '.')
            { 
            argv[0]+=2;
            }
        printf("Usage: %s file1 file2",argv[0]);
        return MISC_ERROR;
        }
    // check arg count
    else if (argc != 3)
        { 
        printf("ERROR: Bad Arguments\n");
        return BAD_ARGS;
        }
    else
        {
            return SUCCESS;
        }
    }