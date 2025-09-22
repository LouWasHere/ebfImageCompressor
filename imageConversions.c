#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ebfImage.h"
#include "ebuImage.h"
#include "ebuErrorHandling.h"
#include "ebuFileReaderWriter.h"
#include "fileReaderWriter.h"
#include "errorHandling.h"

#define SUCCESS 0
#define MISC_ERROR 100
#define BAD_DATA 6
#define SIZE_OF_CHAR 8 // full size of char
#define SIZE_OF_INT 5 // compressed size

int convertEbfToEbu(ebfImage* image1, ebuImage* image2) // translate ebf data to binary format
    {
    // allocate the array for the ebuImage
    image2->height = image1->height;
    image2->width = image1->width;

    // allocate 2d array for the pixel data
    int status = allocateEbuArray(image2);
    if(status != SUCCESS)
        {
        return status;
        }
    // cast data unsigned char
    for(int row = 0; row < image1->height; row++)
        {
        for(int col = 0; col < image1->width; col++)
            {
            image2->imageData[row][col] = (unsigned char)(image1->imageData[row][col]);
            }
        }
    return SUCCESS;
    }

int convertEbuToEbf(ebuImage* image1, ebfImage* image2) // translate binary data to plain ebf
    {
    // allocate the array for the ebuImage
    image2->height = image1->height;
    image2->width = image1->width;
    int status = allocateArray(image2);
    if(status != SUCCESS)
        {
        return status;
        }

    // cast data unsigned int
    for(int row = 0; row < image1->height; row++)
        {
        for(int col = 0; col < image1->width; col++)
            {
            image2->imageData[row][col] = (unsigned int)(image1->imageData[row][col]);
            }
        }
    return SUCCESS;
    }

int convertEbuToEbc(ebuImage* image, FILE* outputFile) // compress ebu binary using bit packing
    {
    // set up buffers and counters
    unsigned char writeBuffer = 0x00000000;
    unsigned char readBuffer = 0x00000000;
    int filledBits = 0;
    int remainingReadBits = 0;
    int leftShift, rightShift = 0;
    int byteCounter = 0;

    // iterate through the input array
    for(int row = 0; row < image->height; row++)
        {
        for(int col = 0; col < image->width; col++)
            {
            readBuffer = image->imageData[row][col]; // place input data into buffer
            remainingReadBits = SIZE_OF_INT; // set remaining bits to read to the size of the int (by default 5 bits)
            while(remainingReadBits > 0) // continue until no bits are left to read
                {
                if(filledBits < (SIZE_OF_INT)-1) // if output byte can be filled with the contents of the input buffer
                    {
                    leftShift = ((SIZE_OF_CHAR - SIZE_OF_INT) - filledBits) + (SIZE_OF_INT - remainingReadBits); // calculate left shift needed
                    writeBuffer = writeBuffer | readBuffer << leftShift; // apply shift to read buffer
                    filledBits += remainingReadBits; // increment filled bits by the bits added
                    remainingReadBits -= remainingReadBits; // all read bits used, so set to zero
                    }
                else // otherwise
                    {
                    rightShift = SIZE_OF_INT - (SIZE_OF_CHAR - filledBits); // calculate right shift required
                    writeBuffer = writeBuffer | readBuffer >> rightShift; // apply shift to read buffer
                    remainingReadBits -= (SIZE_OF_INT - rightShift); // decrement remaining read bits by the bits taken
                    filledBits += (SIZE_OF_INT-rightShift); // increment filled bits by the bits taken from input
                    }
                if(filledBits == SIZE_OF_CHAR) // if output byte is full
                    {
                    fwrite(&writeBuffer, 1, 1, outputFile); // write to file
                    byteCounter++; // increment byteCounter (for debug)
                    filledBits = 0; // reset filled bits
                    writeBuffer = 0x00000000; // reset write buffer
                    }
                }
            }
        }
    return SUCCESS;
    }

int convertEbcToEbu(ebuImage* image, FILE* inputFile, char* arg) // unpack ebc to ebu binary
    {
    // initialize buffers and counters
    int bitsNeeded = SIZE_OF_INT;
    int bitsLeft = 0;
    unsigned char inBuffer = 0;
    unsigned char outBuffer = 0;
    fseek(inputFile,1,SEEK_CUR);

    // iterate through output array, streaming bytes from input file
    for(int row = 0; row < image->height; row++)
        {
        for(int col = 0; col < image->width; col++)
            {
            bitsNeeded = SIZE_OF_INT; // reset bits needed for each output byte to size of int (5 default)
            outBuffer = 0; // clear output buffer
            while(bitsNeeded > 0) // while out-byte still has space
                {
                if(bitsNeeded <= bitsLeft && bitsLeft != 0) // if out-byte can be filled JUST using current in-byte
                    {
                    unsigned char mask = pow(2,bitsNeeded)-1; // create mask using clever maths
                    mask = mask << (bitsLeft - bitsNeeded); // shift mask to capture MSB
                    outBuffer = inBuffer & mask; // capture values
                    image->imageData[row][col] |= outBuffer >> (bitsLeft - bitsNeeded); // append values to output char after shifting to where they are needed
                    bitsLeft -= bitsNeeded; // reduce bits remaining by bits taken
                    bitsNeeded -= bitsNeeded; // no further bits needed
                    }
                else if (bitsLeft != 0)
                    {
                    unsigned char mask = pow(2,bitsLeft)-1; // create mask
                    outBuffer = inBuffer & mask; // no need to shift mask since will always be LSB
                    image->imageData[row][col] |= outBuffer << (bitsNeeded - bitsLeft); // shift buffer to MSB available position in output
                    bitsNeeded -= bitsLeft; // reduce bits needed by bits taken (all available)
                    bitsLeft -= bitsLeft; // no bits left
                    }
                if(bitsLeft == 0) // if at any point bitsLeft is zero, load next bit.
                    {
                    int status = fread(&inBuffer,1,1,inputFile); // read in next byte to the inputbuffer
                    if(status != 1 && row != image->height-1 && col != image->width-1) // check data was read correctly
                        {
                        // if there was an error reading then we assume there is bad data and close
                        free(image->imageData);
                        fclose(inputFile);
                        printf("ERROR: Bad Data (%s)\n",arg);
                        return BAD_DATA;
                        }
                    bitsLeft = SIZE_OF_CHAR; // after reading new bit, reset bits left counter to the size of the byte
                    }
                }
            }
        }
     // check if there is excess data
    int status=tooMuchDataEbu(inputFile,arg);
    if(status != SUCCESS)
        {
        return status;
        }
    return SUCCESS;
    }

int convertEbccToEbu(ebuImage* image, FILE* inputFile, char* arg) // unpack ebc to ebu binary
    {
    // initialize buffers and counters
    int bitsNeeded = SIZE_OF_INT;
    int bitsLeft = 0;
    unsigned char inBuffer = 0;
    unsigned char outBuffer = 0;
    fseek(inputFile,1,SEEK_CUR);

    // iterate through output array, streaming bytes from input file
    for(int row = 0; row < image->height; row+=3)
        {
        for(int col = 0; col < image->width; col+=3)
            {
            bitsNeeded = SIZE_OF_INT; // reset bits needed for each output byte to size of int (5 default)
            outBuffer = 0; // clear output buffer
            while(bitsNeeded > 0) // while out-byte still has space
                {
                if(bitsNeeded <= bitsLeft && bitsLeft != 0) // if out-byte can be filled JUST using current in-byte
                    {
                    unsigned char mask = pow(2,bitsNeeded)-1; // create mask using clever maths
                    mask = mask << (bitsLeft - bitsNeeded); // shift mask to capture MSB
                    outBuffer = inBuffer & mask; // capture values
                    for(int blockRow = 0; blockRow+row < image->height && blockRow < 3; blockRow++)
                        {
                        for(int blockCol = 0; blockCol+col < image->width && blockCol < 3; blockCol++)
                            {
                            image->imageData[row+blockRow][col+blockCol] |= outBuffer >> (bitsLeft - bitsNeeded); // append values to output char after shifting to where they are needed
                            }
                        }
                    bitsLeft -= bitsNeeded; // reduce bits remaining by bits taken
                    bitsNeeded -= bitsNeeded; // no further bits needed
                    }
                else if (bitsLeft != 0)
                    {
                    unsigned char mask = pow(2,bitsLeft)-1; // create mask
                    outBuffer = inBuffer & mask; // no need to shift mask since will always be LSB
                    for(int blockRow = 0; blockRow+row < image->height && blockRow < 3; blockRow++)
                        {
                        for(int blockCol = 0; blockCol+col < image->width && blockCol < 3; blockCol++)
                            {
                            image->imageData[row+blockRow][col+blockCol] |= outBuffer << (bitsNeeded - bitsLeft); // shift buffer to MSB available position in output
                            }
                        }
                    bitsNeeded -= bitsLeft; // reduce bits needed by bits taken (all available)
                    bitsLeft -= bitsLeft; // no bits left
                    }
                if(bitsLeft == 0) // if at any point bitsLeft is zero, load next bit.
                    {
                    int status = fread(&inBuffer,1,1,inputFile); // read in next byte to the inputbuffer
                    if(status != 1 && row != image->height-1 && col != image->width-1) // check data was read correctly
                        {
                        // if there was an error reading then we assume there is bad data and close
                        free(image->imageData);
                        fclose(inputFile);
                        printf("ERROR: Bad Data (%s)\n",arg);
                        return BAD_DATA;
                        }
                    bitsLeft = SIZE_OF_CHAR; // after reading new bit, reset bits left counter to the size of the byte
                    }
                }
            }
        }
     // check if there is excess data
    int status=tooMuchDataEbu(inputFile,arg);
    if(status != SUCCESS)
        {
        return status;
        }
    return SUCCESS;
    }