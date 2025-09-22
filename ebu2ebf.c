#include <stdio.h>
#include <stdlib.h>

#include "checkArgs.h"
#include "ebfImage.h"
#include "ebuImage.h"
#include "fileReaderWriter.h"
#include "ebuFileReaderWriter.h"
#include "imageConversions.h"

#define SUCCESS 0
#define BAD_FILE 2
#define MISC_ERROR 100
#define MAGIC_NUMBER_EBU 0x6575
#define MAGIC_NUMBER_EBF 0x6265
#define MAX_DIMENSION 262144
#define MIN_DIMENSION 1

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
    // Initialize ebuImage struct to handle data regarding the image
    ebuImage *image1;
    image1 = (ebuImage *)malloc(sizeof(ebuImage));

    // cast magicnumber char array to a short, create and initialise variables used within code
    configureEbuImage(image1);

    // open the input file in read mode and read in the image data
    FILE *inputFile = fopen(argv[1], "r");
    status = readEbuFile(argv[1],image1,inputFile);
    if(status != SUCCESS)
        {
        return status;
        }
    // Initialize ebuImage struct to run conversion into
    ebfImage *image2;
    image2 = (ebfImage *)malloc(sizeof(ebfImage));

    // cast magicnumber char array to a short, create and initialize variables used within code
    configureImage(image2);

    // open the output file in write (binary) mode ready for conversion
    FILE *outputFile = fopen(argv[2], "w");

    // convert ebf image data to ebu data and store in ebuImage data array
    status = convertEbuToEbf(image1, image2);
    if(status != 0)
        {
        return status;
        }
    // write binary data from ebuImage to file
    status = writeFile(argv,image2,outputFile);
    if(status != 0)
        {
        return status;
        }
    // print final success message and return
    printf("CONVERTED\n");
    return SUCCESS;
    } // main()