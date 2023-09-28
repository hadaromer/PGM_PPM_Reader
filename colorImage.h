//Omer Hadar 2073388331 and Dor Wachner 208277343

#ifndef COLORIMAGE_H
#define COLORIMAGE_H

#include "utils.h"

typedef struct rgb {
    unsigned char r, g, b;
} RGB;

typedef struct colorImage {
    unsigned short rows, cols;
    RGB **pixels;
} COLOR_IMAGE;

COLOR_IMAGE *readPPM(char *fname);

COLOR_IMAGE *readPPM_P6(char *fname);

COLOR_IMAGE *readPPMHelper(char *fname,IMAGE_TYPE imageType);

COLOR_IMAGE *createColorImage();

void createPixelsArrayForColorImage(COLOR_IMAGE *res);

void insertPixelsDataToRgbArray(FILE *fp, COLOR_IMAGE *res);

void insertPixelsDataToRgbArrayP6(FILE *fp, COLOR_IMAGE *res);

void freeColorImage(COLOR_IMAGE *c);

#endif //COLORIMAGE_H
