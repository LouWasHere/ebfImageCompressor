#include <stdio.h>
#include <stdlib.h>
#include "checkArgs.h"
#include "ebuErrorHandling.h"
#include "ebuFileReaderWriter.h"
#include "ebuImage.h"
#include "imageConversions.h"

#define SUCCESS 0
#define MISC_ERROR 100

int main(int argc, char** argv)
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
    status = readEbcFile(argv[1],image1,inputFile);
    if(status != SUCCESS)
        {
        return status;
        }
    // write binary data from ebuImage to file
    FILE *outputFile = fopen(argv[2], "wb");
    status = writeEbuFile(argv,image1,outputFile);
    if(status != 0)
        {
        return status;
        }
    // print final success message and return
    printf("CONVERTED\n");
    return SUCCESS;
    } // main()