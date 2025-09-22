#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "checkArgs.h"
#include "ebuImage.h"
#include "ebuFileReaderWriter.h"
#include "imageConversions.h"
#include "ebuErrorHandling.h"
#include "blockCompression.h"

#define SUCCESS 0
#define BAD_FILE 2
#define MISC_ERROR 100
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
    // initialize data structures
    ebuImage *image1;
    image1 = (ebuImage *)malloc(sizeof(ebuImage));
    configureEbuImage(image1);

    // read in ebc data from opened file
    FILE *inputFile = fopen(argv[1], "r");
    status = readEbccFile(argv[1],image1,inputFile);
    if(status != SUCCESS)
        {
        return status;
        }
    // output decompressed ebu array as ebc
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