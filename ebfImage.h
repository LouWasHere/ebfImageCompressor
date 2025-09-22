#ifndef EBFIMAGE_H
#define EBFIMAGE_H

typedef struct _ebfImage
    {
    unsigned char magicNumber[2];
    unsigned short *magicNumberValue;
    int width;
    int height;
    unsigned int **imageData;
    long numBytes;
    } ebfImage;

#endif