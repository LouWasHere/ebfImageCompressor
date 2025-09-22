#include <stdio.h>
#include <stdlib.h>

#include "errorHandling.h"
#include "ebfImage.h"

#define SUCCESS 0

void configureImage(ebfImage *image)
    {
    // cast magicnumber char array to a short
    image->magicNumberValue = (unsigned short *)image->magicNumber;

    // create and initialise variables used within code
    image->width = 0;
    image->height = 0;
    image->imageData = NULL;
    image->numBytes = 0;
    }

int allocateArray(ebfImage *image, FILE *file) // allocate memory and create 2 dimensional array
    {
    // allocate memory for pointers to each row
    unsigned int **wholeArray = (unsigned int **)malloc(image->height * sizeof(unsigned int *));
    int status = badMallocDouble(file, wholeArray);
    if(status != SUCCESS)
        {
        return status;
        }
    // allocate memory for all data storage at once
    unsigned int *dataBlock = (unsigned int *)malloc(image->height*image->width*sizeof(unsigned int));
    status = badMallocSingle(file, dataBlock);
    if(status != SUCCESS)
        {
        return status;
        }
    // loop to set up pointers for each row
    for(int row = 0; row < image->height; row++)
        {
        wholeArray[row] = dataBlock + row * image->width;
        }
    // assign and return whole array
    image->imageData = wholeArray;
    return SUCCESS;
    }

int readFile(char * arg, ebfImage * image, FILE *inputFile)
    {
    // check file name
    int status = 0;
    status = badFileName(inputFile,arg);
    if(status != SUCCESS)
        {
        return status;
        }
    // get first 2 characters which should be magic number
    image->magicNumber[0] = getc(inputFile);
    image->magicNumber[1] = getc(inputFile);
    status = badMagicNumber(image,arg);
    if(status != SUCCESS)
        {
        return status;
        }
    // verify that the dimensions of the image are valid
    status = badDimensions(inputFile,arg,image);
    if(status != SUCCESS)
        {
        return status;
        }

    // caclulate total size and allocate memory for array
    image->numBytes = image->height * image->width;
    status = allocateArray(image,inputFile);
    if(status != SUCCESS)
    {
        return status;
    }
    // check for an error in memory allocation
    status = badMallocDouble(inputFile, image->imageData);
    if(status != SUCCESS)
        {
        return status;
        }
    // begin reading the image
    for (int row = 0; row < image->height; row++)
        { // reading in each row
        for (int col = 0; col < image->width; col++)
            { // reading in each column
            int check = fscanf(inputFile, "%u", &image->imageData[row][col]);
            // validate that we have captured 1 pixel value
            status = badData(check,image,inputFile,row,col,arg);
            if(status != SUCCESS)
                {
                return status;
                }
            }
        } // reading in
    // check for too much data
    status=tooMuchData(inputFile,arg);
    if(status != SUCCESS)
        {
        return status;
        }

    // now we have finished using the inputFile we should close it
    fclose(inputFile);
    return SUCCESS;
    }

int writeFile(char ** argv, ebfImage * image, FILE * outputFile)
    {
    // check file name
    int status = 0;
    status = badFileName(outputFile,argv[2]);
    if(status != SUCCESS)
        {
        return status;
        }
    // write the header data in one block
    int check = fprintf(outputFile, "eb\n%d %d\n", image->height, image->width);

    // and use the return from fprintf to check that we wrote.
    status = badOutput(check,outputFile,image,argv[2]);
    if(status != SUCCESS)
        {
        return status;
        }
    // iterate though the array and print out pixel values
    for (int row = 0; row < image->height; row++)
        {
        for (int col = 0; col < image->width; col++)
            {
            if(row+1 == image->height && col+1 == image->width)
                { // if we are at the end of the file, do not write a new line
                check = fprintf(outputFile, "%u", image->imageData[row][col]);
                }
            else
                { // if we are at the end of a row ((current+1)%width == 0) then write a newline, otherwise a space.
                check = fprintf(outputFile, "%u%c", image->imageData[row][col], (col+1 == image->width) ? '\n' : ' ');
                }
            status = badOutput(check,outputFile,image,argv[2]);
            if(status != SUCCESS)
                {
                return status;
                }
            }
        }
    // free allocated memory before exit
    free(image->imageData);

    // close the output file before exit
    fclose(outputFile);
    return SUCCESS;
    }