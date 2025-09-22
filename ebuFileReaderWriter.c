#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ebuErrorHandling.h"
#include "ebuImage.h"
#include "imageConversions.h"

#define SUCCESS 0
#define BAD_DATA 6

void configureEbuImage(ebuImage *image)
    {
    // cast magicnumber char array to a short
    image->magicNumberValue = (unsigned short *)image->magicNumber;

    // create and initialise variables used within code
    image->width = 0;
    image->height = 0;
    image->imageData = NULL;
    image->numBytes = 0;
    }

int allocateEbuArray(ebuImage *image, FILE *file) // allocate memory and create 2 dimensional array
    {
    // allocate memory for pointers to each row
    image->imageData = (unsigned char **)malloc(image->height * sizeof(unsigned char *));
    int status = badMallocDoubleEbu(file, image->imageData);
    if(status != SUCCESS)
        {
        return status;
        }
    // allocate memory for all data storage at once
    unsigned char *dataBlock = (unsigned char *)malloc(image->height*image->width*sizeof(unsigned char));
    status = badMallocSingleEbu(file, dataBlock);
    if(status != SUCCESS)
        {
        return status;
        }
    // loop to set up pointers for each row
    for(int row = 0; row < image->height; row++)
        {
        image->imageData[row] = dataBlock + row * image->width;
        }
    // assign and return whole array
    return SUCCESS;
    }

int readEbuFile(char * arg, ebuImage *image, FILE *inputFile)
    {
    int status = 0;
    status = badFileNameEbu(inputFile,arg);
    if(status != SUCCESS)
        {
        return status;
        }
    // get first 2 characters which should be magic number
    image->magicNumber[0] = getc(inputFile);
    image->magicNumber[1] = getc(inputFile);
    status = badMagicNumberEbu(image,arg);
    if(status != SUCCESS)
        {
        return status;
        }
    // verify that the dimensions of the image are valid
    status = badDimensionsEbu(inputFile,arg,image);
    if(status != SUCCESS)
        {
        return status;
        }
    // caclulate total size and allocate memory for array
    image->numBytes = image->height * image->width;
    status = allocateEbuArray(image,inputFile);
    if(status != SUCCESS)
    {
        return status;
    }
    // check for an error in memory allocation
    status = badMallocDoubleEbu(inputFile, image->imageData);
    if(status != SUCCESS)
        {
        return status;
        }
    // begin reading the image
    fseek(inputFile, 1, SEEK_CUR); // remove newline
    for (int row = 0; row < image->height; row++)
        {
        int check = fread(image->imageData[row], 1, image->width, inputFile);
        // validate that we have captured 1 valid pixel value
        for(int col = 0; col < image->width; col++)
            {
            status = badDataEbu(check, image, inputFile,row ,col, arg);
            if(status != SUCCESS)
                {
                return status;
                }
            }
        }
    status=tooMuchDataEbu(inputFile,arg);
    if(status != SUCCESS)
        {
        return status;
        }
    // now we have finished using the inputFile we should close it
    fclose(inputFile);
    return SUCCESS;
    }

int readEbcFile(char* arg, ebuImage* image, FILE* inputFile)
    {
    int status = 0;
    status = badFileNameEbu(inputFile,arg);
    if(status != SUCCESS)
        {
        return status;
        }
    // get first 2 characters which should be magic number
    image->magicNumber[0] = getc(inputFile);
    image->magicNumber[1] = getc(inputFile);
    status = badMagicNumberEbc(image,arg);
    if(status != SUCCESS)
        {
        return status;
        }
    // verify that the dimensions of the image are valid
    status = badDimensionsEbu(inputFile,arg,image);
    if(status != SUCCESS)
        {
        return status;
        }
    // caclulate total size and allocate memory for array
    image->numBytes = image->height * image->width;
    status = allocateEbuArray(image,inputFile);
    if(status != SUCCESS)
    {
        return status;
    }
    // check for an error in memory allocation
    status = badMallocDoubleEbu(inputFile, image->imageData);
    if(status != SUCCESS)
        {
        return status;
        }
    // begin reading file
    status = convertEbcToEbu(image,inputFile,arg);
    if(status != SUCCESS)
        {
            return status;
        }
    return SUCCESS;
    }

int readEbccFile(char* arg, ebuImage* image, FILE* inputFile)
    {
    int status = 0;
    status = badFileNameEbu(inputFile,arg);
    if(status != SUCCESS)
        {
        return status;
        }

    // get first 2 characters which should be magic number
    image->magicNumber[0] = getc(inputFile);
    image->magicNumber[1] = getc(inputFile);
    status = badMagicNumberEbc(image,arg);
    if(status != SUCCESS)
        {
        return status;
        }

    // verify that the dimensions of the image are valid
    status = badDimensionsEbu(inputFile,arg,image);
    if(status != SUCCESS)
        {
        return status;
        }  

    // caclulate total size and allocate memory for array
    image->numBytes = image->height * image->width;
    status = allocateEbuArray(image,inputFile);
    if(status != SUCCESS)
    {
        return status;
    }

    // check for an error in memory allocation
    status = badMallocDoubleEbu(inputFile, image->imageData);
    if(status != SUCCESS)
        {
        return status;
        }
    
    // begin reading file
    status = convertEbccToEbu(image,inputFile,arg);
    if(status != SUCCESS)
        {
            return status;
        }
    return SUCCESS;
    }

int writeEbuFile(char ** argv, ebuImage * image, FILE * outputFile)
    {
    int status = 0;
    status = badFileNameEbu(outputFile,argv[2]);
    if(status != SUCCESS)
        {
        return status;
        }
    // write the header data in one block
    int check = fprintf(outputFile, "eu\n%d %d\n", image->height, image->width);
    // and use the return from fprintf to check that we wrote.
    status = badPrintOutputEbu(check,outputFile,image,argv[2]);
    if(status != SUCCESS)
        {
        return status;
        }
    // iterate though the array and print out pixel values
    int totalbytes = 0;
    for(int row = 0; row < image->height; row++)
        {
        check = fwrite(image->imageData[row], 1, image->width, outputFile);
        totalbytes += check;
        status = badWriteOutputEbu(check,outputFile,image,argv[2]);
        if(status != SUCCESS)
            {
            return status;
            }
        }
    // free allocated memory before exit
    free(image->imageData);
    // close the output file before exit
    fclose(outputFile);
    return SUCCESS;
    }