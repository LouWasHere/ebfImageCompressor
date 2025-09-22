#include <stdio.h>

#include "ebuImage.h"

void configureEbuImage(ebuImage*);
int readEbuFile(char *,ebuImage *,FILE *);
int writeEbuFile(char **, ebuImage *, FILE *);
int allocateEbuArray(ebuImage *);
int readEbcFile(char *,ebuImage*,FILE*);
int readEbccFile(char *,ebuImage*,FILE*);