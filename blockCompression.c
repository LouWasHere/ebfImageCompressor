#include <stdio.h>

#include "ebuImage.h"

int averageBlockValues(ebuImage *inputImage, ebuImage *outputImage)
    {
    // initialize pointers for navigating the input array
    int inWidth = 0;
    int inHeight = 0;
    int colsCounted = 0;
    int rowsCounted = 0;
    int blockTotal = 0;
    int pixelsCounted = 0;

    // iterate through the output array, averaging every grid of 3x3 pixels
    for(int outHeight = 0; outHeight < outputImage->height; outHeight++)
        {
        for(int outWidth = 0; outWidth < outputImage->width; outWidth++)
            {
            inWidth = outWidth*3; // start at the top-left corner of the pixel (out coords * 3)
            inHeight = outHeight*3;
            while(inHeight < inputImage->height && rowsCounted < 3) // repeat until image borders are reached or 3 times
                {
                    while(inWidth < inputImage->width && colsCounted < 3)
                        {
                            blockTotal += inputImage->imageData[inHeight][inWidth]; // add total pixel value
                            inWidth++;
                            colsCounted++;
                            pixelsCounted++;
                        }
                    colsCounted = 0;
                    inHeight++;
                    rowsCounted++;
                }
            // after calculating the average of the 3x3 (or whatever size) output value to output array
            rowsCounted = 0;
            outputImage->imageData[outHeight][outWidth] = blockTotal/pixelsCounted; // calculate average
            blockTotal = 0;
            pixelsCounted = 0;
            }
        }
    return 0;
    }