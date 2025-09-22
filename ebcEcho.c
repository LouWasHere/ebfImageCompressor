#include <stdio.h>
#include <stdlib.h>

#include "checkArgs.h"
#include "ebuImage.h"
#include "ebuErrorHandling.h"
#include "ebuFileReaderWriter.h"
#include "imageConversions.h"

#define SUCCESS 0
#define BAD_FILE 2
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
    ebuImage *image;
    image = (ebuImage *)malloc(sizeof(ebuImage));

    // cast magicnumber char array to a short, create and initialise variables used within code
    configureEbuImage(image);

    // open the input file in read mode and read in the image data
    FILE *inputFile = fopen(argv[1], "r");
    status = readEbcFile(argv[1],image,inputFile);
    if(status != SUCCESS)
        {
        return status;
        }
    // open the output file in write mode
    FILE *outputFile = fopen(argv[2], "wb");
    status = badFileNameEbu(outputFile,argv[2]);
    if(status != SUCCESS)
        {
        return status;
        }
    // write the header data in one block
    int check = fprintf(outputFile, "ec\n%d %d\n", image->height, image->width);
    // and use the return from fprintf to check that we wrote.
    status = badPrintOutputEbu(check,outputFile,image,argv[2]);
    if(status != SUCCESS)
        {
        return status;
        }
    // write compressed data again to echoed file
    status = convertEbuToEbc(image, outputFile);
    if(status != 0)
        {
        return status;
        }
    // print final success message and return
    printf("ECHOED\n");
    return SUCCESS;
    } // main()