#include <stdio.h>
#include <stdlib.h>

#include "checkArgs.h"
#include "ebuImage.h"
#include "ebuFileReaderWriter.h"
#include "imageConversions.h"
#include "ebuErrorHandling.h"

#define SUCCESS 0
#define BAD_FILE 2
#define MISC_ERROR 100
#define MAGIC_NUMBER_EBU 0x6575
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
    // open the output file in write (binary) mode ready for conversion
    FILE *outputFile = fopen(argv[2], "wb");
    status = badFileNameEbu(outputFile,argv[2]);
    if(status != SUCCESS)
        {
        return status;
        }
    // write the header data in one block
    int check = fprintf(outputFile, "ec\n%d %d\n", image1->height, image1->width);
    // and use the return from fprintf to check that we wrote.
    status = badPrintOutputEbu(check,outputFile,image1,argv[2]);
    if(status != SUCCESS)
        {
        return status;
        }
    // convert ebu image data to ebc data and store in ebuImage data array, and write binary data from ebuImage to file
    status = convertEbuToEbc(image1, outputFile);
    if(status != 0)
        {
        return status;
        }
    // print final success message and return
    printf("CONVERTED\n");
    return SUCCESS;
    } // main()