#include <stdio.h>
#include <stdlib.h>
#include "ebfImage.h"

#define MAGIC_NUMBER 0x6265
#define SUCCESS 0
#define BAD_FILE 2
#define BAD_MAGIC_NUMBER 3
#define BAD_DIM 4
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7
#define MAX_DIMENSION 262144
#define MIN_DIMENSION 1

int badFileName(FILE *file, char *argv) // generic check to verify a file exists/has read/write access
    {
    if (!file)
        {
        printf("ERROR: Bad File Name (%s)\n",argv);
        return BAD_FILE;
        }
    else
        {
            return SUCCESS;
        }
    }

int badMagicNumber(ebfImage *image, char *argv) // simple check for magic number validation
    {
    // checking against the casted value due to endienness.
    if (*image->magicNumberValue != MAGIC_NUMBER)
        { // check magic number
        printf("ERROR: Bad Magic Number (%s)\n",argv);
        return BAD_MAGIC_NUMBER;
        } //check magic number
    else
        {
        return SUCCESS;
        }
    }

int badDimensions(FILE *file,char *argv,ebfImage *image) // scan for dimensions and verify they are valid
    {
    // scan for the dimensions
    // and capture fscanfs return to ensure we got 2 values.
    int check = fscanf(file, "%d %d", &image->height, &image->width);
    if (check != 2 || image->height < MIN_DIMENSION || image->width < MIN_DIMENSION || image->height > MAX_DIMENSION || image->width > MAX_DIMENSION)
        { // check dimensions
        // close the file as soon as an error is found
        fclose(file);
        
        // print appropriate error message and return
        printf("ERROR: Bad Dimensions (%s)\n",argv);
        return BAD_DIM;
        } // check dimensions
    else
        {
        return SUCCESS;
        }
    }

int badMallocDouble(FILE *file, unsigned int **data)
    {
    // if malloc is unsuccessful, it will return a null pointer
    if (data == NULL)
        { // check malloc
        fclose(file);
        printf("ERROR: Image Malloc Failed\n");
        return BAD_MALLOC;
        } // check malloc
    else
        {
        return SUCCESS;
        }
    }

int badMallocSingle(FILE *file, unsigned int *data) // version of badMallocDouble that checks single pointers
    {
    // if malloc is unsuccessful, it will return a null pointer
    if (data == NULL)
        { // check malloc
        fclose(file);
        printf("ERROR: Image Malloc Failed\n");
        return BAD_MALLOC;
        } // check malloc
    else
        {
        return SUCCESS;
        }
    }

int badData(int check, ebfImage* image, FILE *file, int row, int col, char *argv) // validate data is within bounds
    {
    if (check != 1 || image->imageData[row][col] > 31 || image->imageData[row][col] < 0)
        { // check inputted data
        // ensure that allocated data is freed before exit.
        free(image->imageData);
        fclose(file);
        printf("ERROR: Bad Data (%s)\n",argv);
        return BAD_DATA;
        } // check inputted data
    else
        {
        return SUCCESS;
        }
    }

int tooMuchData(FILE *file,char *argv)
    {
    // attempt to read the file once more to check if there is too much data
    unsigned int testInput = 1;
    int check = fscanf(file, "%u", &testInput);
    if(check == 1)
        { // check for read success
        // ensure allocated data is freed before exit.
        fclose(file);
        printf("ERROR: Bad Data (%s)\n",argv);
        return BAD_DATA;
        } // check for read success
    else
        {
        return SUCCESS;
        }
    }

int badOutput(int check, FILE * file, ebfImage * image, char *argv)
    {
    if (check == 0) 
        { // check write
        fclose(file);
        free(image->imageData);
        printf("ERROR: Bad Output\n (%s)", argv);
        return BAD_OUTPUT;
        } // check write
    else
        {
        return SUCCESS;
        }
    }