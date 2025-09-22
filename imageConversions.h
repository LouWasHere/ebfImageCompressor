#include <stdio.h>

#include "ebfImage.h"
#include "ebuImage.h"

int convertEbfToEbu(ebfImage*, ebuImage*);
int convertEbuToEbf(ebuImage*, ebfImage*);
int convertEbuToEbc(ebuImage*, FILE*);
int convertEbcToEbu(ebuImage*, FILE*, char*);
int convertEbccToEbu(ebuImage*, FILE*, char*);;