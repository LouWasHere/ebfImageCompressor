#ifndef EBUIMAGE_H
#define EBUIMAGE_H

typedef struct _ebuImage
    {
    unsigned char magicNumber[2];
    unsigned short *magicNumberValue;
    int width;
    int height;
    unsigned char **imageData;
    long numBytes;
    } ebuImage;

#endif