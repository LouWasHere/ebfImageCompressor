#include <stdio.h>
#include <stdlib.h>

#include "checkArgs.h"
#include "ebfImage.h"
#include "errorHandling.h"
#include "fileReaderWriter.h"
#include "compareImages.h"

#define SUCCESS 0
#define BAD_ARGS 1
#define BAD_FILE 2
#define BAD_MAGIC_NUMBER 3
#define BAD_DIM 4
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7
#define MISC_ERROR 100
#define MAGIC_NUMBER 0x6265
#define MAX_DIMENSION 262144
#define MIN_DIMENSION 1

int main(int argc, char **argv)
    { // main
    // chech arguments
    int status = check_arguments(argc, argv);
    if(status != SUCCESS)
        {
        if(status == MISC_ERROR)
            {
            return SUCCESS;
            }
        else
            {
            return status;
            }
        }
    // Initlaize Image struct to handle data regarding the image
    ebfImage *image1;
    image1 = (ebfImage *)malloc(sizeof(ebfImage));

    // cast magicnumber char array to a short, create and initialise variables used within code
    configureImage(image1);

    // open the input file in read mode and read in the image data
    FILE *inputFile1 = fopen(argv[1], "r");
    status = readFile(argv[1],image1,inputFile1);
    if(status != SUCCESS)
        {
        return status;
        }
    // Initlaize Image struct to handle data regarding the image
    ebfImage *image2;
    image2 = (ebfImage *)malloc(sizeof(ebfImage));

    // cast magicnumber char array to a short, create and initialise variables used within code
    configureImage(image2);

    // open the input file in read mode and read in the image data
    FILE *inputFile2 = fopen(argv[2], "r");
    status = readFile(argv[2],image2,inputFile2);
    if(status != SUCCESS)
        {
        return status;
        }
    // compare the data from the two files:
    status = compareEbf(image1,image2);
    if(status != SUCCESS)
        {
        return status;
        }
    } // main()
