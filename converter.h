//Omer Hadar 2073388331 and Dor Wachner 208277343

#ifndef EX4_1_CONVERTER_H
#define EX4_1_CONVERTER_H

#include "utils.h"
#include "colorImage.h"
#include "grayImage.h"

typedef struct blackWhiteFiles {
    unsigned short k;
    FILE *fp;
    char *fname;
} BLACK_WHITE_FILES;

#define NUMBER_OF_BW_FILES 3

void convertPPMToPGM(char *fname);

void convertPPMToPGM_Bin(char *fname);

void convertPPMToPGMHelper(char *fname, IMAGE_TYPE ppmType, IMAGE_TYPE pgmType);

void insertPixelsDataToGrayArrayFromColorImage(GRAY_IMAGE *g, COLOR_IMAGE *c);

unsigned char getGrayVariety(RGB color);

unsigned char findMaxValueInGrayImagePixels(GRAY_IMAGE *g);

void writeGrayImagePixelsToP2File(FILE *fp,GRAY_IMAGE *g);

void writeGrayImagePixelsToP5File(FILE *fp, GRAY_IMAGE *g);

void convertPGMToBW(char *fname);

void convertPGMToBWHelper(char *fname,IMAGE_TYPE imageType);

void convertPGMToBW_Bin(char *fname);

BLACK_WHITE_FILES *createBlackWhiteFiles();

void initBlackWhiteFiles(char *fname, BLACK_WHITE_FILES *files, IMAGE_TYPE imageType);

void writeHeadersForBlackWhiteFiles(GRAY_IMAGE *grayImage, unsigned char maxGrayValue, BLACK_WHITE_FILES *files,
                                    IMAGE_TYPE imageType);

void closeBlackWhiteFiles(BLACK_WHITE_FILES *files);

void writeBlackWhiteFilesData(GRAY_IMAGE *grayImage, BLACK_WHITE_FILES *files, IMAGE_TYPE imageType);

FILE *createNewPGMFile(char *fname, IMAGE_TYPE pgmType);

#endif //EX4_1_CONVERTER_H
