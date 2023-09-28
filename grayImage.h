//Omer Hadar 2073388331 and Dor Wachner 208277343

#ifndef GRAYIMAGE_H
#define GRAYIMAGE_H

#include "utils.h"

typedef struct grayImage {
    unsigned short rows, cols;
    unsigned char **pixels;
} GRAY_IMAGE;


GRAY_IMAGE *readPGM(char *fname);

GRAY_IMAGE *readPGMP5(char *fname);

GRAY_IMAGE *readPGMHelper(char *fname, IMAGE_TYPE imageType);

GRAY_IMAGE * createGrayImage();

void createPixelsArrayForGrayImage(GRAY_IMAGE *res);

void insertPixelsDataToGrayArray(FILE *fp, GRAY_IMAGE *res);

void insertPixelsDataToGrayArrayP5(FILE *fp, GRAY_IMAGE *res);

void writeHeaderForPPMFile(FILE *fp, unsigned short maxGrayLevel, unsigned short width, unsigned short height, IMAGE_TYPE imageType);

void writePgmFile(char *fname,GRAY_IMAGE *g,IMAGE_TYPE imageType);

void freeGrayImage(GRAY_IMAGE *c);

#endif //GRAYIMAGE_H
