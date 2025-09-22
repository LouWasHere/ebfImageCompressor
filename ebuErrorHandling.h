#include "ebuImage.h"

int badMallocSingleEbu(FILE *, unsigned char *);
int badMallocDoubleEbu(FILE *, unsigned char **);
int badFileNameEbu(FILE *, char *);
int badMagicNumberEbu(ebuImage*, char *);
int badMagicNumberEbc(ebuImage*, char *);
int badDimensionsEbu(FILE *,char*,ebuImage*);
int badDataEbu(int, ebuImage*, FILE *, int, int, char*);
int tooMuchDataEbu(FILE *,char*);
int badPrintOutputEbu(int, FILE *, ebuImage *, char *);
int badWriteOutputEbu(int, FILE *, ebuImage *, char *);