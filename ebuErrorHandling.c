#include <stdio.h>
#include <stdlib.h>

#include "ebuImage.h"

#define MAGIC_NUMBER 0x7565
#define MAGIC_NUMBER_EBC 0x6365
#define SUCCESS 0
#define BAD_FILE 2
#define BAD_MAGIC_NUMBER 3
#define BAD_DIM 4
#define BAD_MALLOC 5
#define BAD_DATA 6
#define BAD_OUTPUT 7
#define MAX_DIMENSION 262144
#define MIN_DIMENSION 1

int badMallocDoubleEbu(FILE *file, unsigned char **data)
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

int badMallocSingleEbu(FILE *file, unsigned char *data)
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

int badFileNameEbu(FILE *file, char *argv)
    {
    if (!file)
        { // check file pointer
        printf("ERROR: Bad File Name (%s)\n",argv);
        return BAD_FILE;
        } // check file pointer
    else
        {
            return SUCCESS;
        }
    }

int badMagicNumberEbu(ebuImage *image, char *argv)
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

int badMagicNumberEbc(ebuImage *image, char *argv)
    {
    // checking against the casted value due to endienness.
    if (*image->magicNumberValue != MAGIC_NUMBER_EBC)
        { // check magic number
        printf("ERROR: Bad Magic Number (%s)\n",argv);
        return BAD_MAGIC_NUMBER;
        } //check magic number
    else
        {
        return SUCCESS;
        }
    }

int badDimensionsEbu(FILE *file,char *argv,ebuImage *image)
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

int badDataEbu(int check, ebuImage* image, FILE *file, int row, int col, char *argv)
    {
    if (check != image->width || image->imageData[row][col] > 31 || image->imageData[row][col] < 0)
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

int tooMuchDataEbu(FILE *file,char *argv)
    {
    // attempt to read the file once more to check if there is too much data
    unsigned int testInput = 1;
    int check = fread(&testInput,1,1,file);
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

int badPrintOutputEbu(int check, FILE * file, ebuImage * image, char *argv)
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

int badWriteOutputEbu(int check, FILE * file, ebuImage * image, char *argv)
    {
    if (check != image->width)
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