#include <stdlib.h>
#include <stdio.h>

#include "ebfImage.h"
#include "ebuImage.h"

#define SUCCESS 0

int freeAndReturn(ebfImage* image1, ebfImage* image2) // free images and return a successful comparison
    {
    free(image1->imageData);
    free(image2->imageData);
    printf("DIFFERENT\n");
    return SUCCESS;
    }

int freeAndReturnEbu(ebuImage* image1, ebuImage* image2) // ebu struct version of freeAndReturn
    {
    free(image1->imageData);
    free(image2->imageData);
    printf("DIFFERENT\n");
    return SUCCESS;
    }

int compareEbf(ebfImage *image1, ebfImage *image2) // compare two ebf structs
    {
    // start with magic number values
    if (*image1->magicNumberValue != *image2->magicNumberValue)
        {
        return freeAndReturn(image1,image2);
        }
    // check dimensions
    if ((image1->height != image2->height) || (image1->width != image2->width))
        {
        return freeAndReturn(image1,image2);
        }
    // and check the pixel values
    for (int row=0; row<image1->height; row++)
        {
        for (int col=0; col<image1->width; col++)
            {
            if(image1->imageData[row][col]!=image2->imageData[row][col])
                {
                return freeAndReturn(image1,image2);
                }
            }
        
        }
    // free allocated memory before exit
    free(image1->imageData);
    free(image2->imageData);

    // if we have not exited on different data, must be identical
    printf("IDENTICAL\n");
    return SUCCESS;
    }

int compareEbu(ebuImage *image1, ebuImage *image2) // compare two ebu structs
    {
    // start with magic number values
    if (*image1->magicNumberValue != *image2->magicNumberValue)
        {
        return freeAndReturnEbu(image1,image2);
        }
    // check dimensions
    if ((image1->height != image2->height) || (image1->width != image2->width))
        {
        return freeAndReturnEbu(image1,image2);
        }
    // and check the pixel values
    for (int row=0; row<image1->height; row++)
        {
        for (int col=0; col<image1->width; col++)
            {
                if(image1->imageData[row][col]!=image2->imageData[row][col])
                {
                return freeAndReturnEbu(image1,image2);
                }
            }
        
        }
    // free allocated memory before exit
    free(image1->imageData);
    free(image2->imageData);

    // if we have not exited on different data, must be identical
    printf("IDENTICAL\n");
    return SUCCESS;
    }