#include <stdio.h>
#include <stdlib.h>

#include "checkArgs.h"
#include "ebfImage.h"
#include "errorHandling.h"
#include "fileReaderWriter.h"

#define SUCCESS 0
#define MISC_ERROR 100


int main(int argc, char **argv)
    { // main
    // check arguments
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
    ebfImage *image;
    image = (ebfImage *)malloc(sizeof(ebfImage));

    // cast magicnumber char array to a short, create and initialise variables used within code
    configureImage(image);

    // open the input file in read mode and read in the image data
    FILE *inputFile = fopen(argv[1], "r");
    status = readFile(argv[1],image,inputFile);
    if(status != SUCCESS)
        {
        return status;
        }
    // open the output file in write mode
    FILE *outputFile = fopen(argv[2], "w");
    status = writeFile(argv,image,outputFile);
    if(status != SUCCESS)
        {
        return status;
        }
    // print final success message and return
    printf("ECHOED\n");
    return SUCCESS;
    } // main()