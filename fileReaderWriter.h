#include <stdio.h>

#include "ebfImage.h"

void configureImage(ebfImage*);
int readFile(char *,ebfImage *,FILE *);
int writeFile(char **, ebfImage *, FILE *);
int allocateArray(ebfImage *);