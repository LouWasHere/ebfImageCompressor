#include "ebfImage.h"

#include <stdio.h>

int badFileName(FILE *, char *);
int badMagicNumber(ebfImage*, char *);
int badDimensions(FILE *,char*,ebfImage*);
int badMallocSingle(FILE *, unsigned int *);
int badMallocDouble(FILE *, unsigned int **);
int badData(int, ebfImage*, FILE *, int, int, char*);
int tooMuchData(FILE *,char*);
int badOutput(int, FILE *, ebfImage *, char *);