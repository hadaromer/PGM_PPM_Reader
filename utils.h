//Omer Hadar 2073388331 and Dor Wachner 208277343

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define MAX_VALUE 255
#define MIN_VALUE 0

typedef unsigned char BYTE;

#define isDigit(x) ((x)>='0' && (x)<='9')

typedef unsigned char IMAGE_TYPE;
#define P6 '6'
#define P5 '5'
#define P3 '3'
#define P2 '2'

typedef unsigned short IMG_POS[2];

/* IMAGES */

unsigned short getNextNumber(FILE *fp, char *c);

void skipComments(FILE *fp, char *c);

void readHeader(FILE *fp, char *c, unsigned short *width, unsigned short *height, unsigned short *maxDepth);

void setImageDimmensions(unsigned short width, unsigned short *imageWidth, unsigned short height, unsigned short *imageHeight);

/* END IMAGES */

/* FILES */

void checkFileOpen(const char *fname, const FILE *fp);

void validateFile(FILE *fp, char *c, char expected);

/* END FILES */

/* MEMORY CHECKS */
void checkMemoryAllocation(void *ptr);

void copyPosition(IMG_POS src, IMG_POS dest);

/* END MEMORY CHECKS */

/* STRINGS */

char * getNewName(char *old,char *extension,unsigned short extensionSize);

unsigned short getStringLen(char *old);

/* END STRINGS */

#endif //UTILS_H
