//Omer Hadar 2073388331 and Dor Wachner 208277343

#ifndef COMPRESSER_H
#define COMPRESSER_H
#include "utils.h"
#include "colorImage.h"
#include "grayImage.h"

void validateGrayLevel(unsigned char maxGrayLevel);

void saveCompressed(char *fname, GRAY_IMAGE *image,unsigned char maxGrayLevel);

unsigned short log2(unsigned char c);

void convertCompressedImageToPGM(char *fname);

unsigned short getValueForMask(unsigned short maxValue);

void writeHeaderCompressedFile(const GRAY_IMAGE *image, unsigned char maxGrayLevel, FILE *fp);

void shiftingValuesAndWriteToFile(unsigned short numberBitsRequired, unsigned short *remainingBits,
                                  BYTE *c, unsigned char temp, FILE *fp);

void
readHeaderOfCompressedFile(FILE *binFile, unsigned short *maxGrayLevel, unsigned short *width, unsigned short *height);

void readBinaryFileAndWritePgm(unsigned short numberBitsRequired, unsigned short *remainingBits,
                               unsigned short valueForMask, BYTE *c, FILE *binFile, FILE *fp);

#endif //COMPRESSER_H
