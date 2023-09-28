//Omer Hadar 2073388331 and Dor Wachner 208277343

#include "compresser.h"

void
insertPixelsDataToCompressedFile(const GRAY_IMAGE *image, unsigned char maxGrayLevel, unsigned short numberBitsRequired,
                                 FILE *fp);

/*
 * saves a compressed file by a given gray image and new max gray level
 * *fname - the name for the file
 * *image - pointer to the gray image
 * maxGrayLevel - the new max gray level for compressed image
 */
void saveCompressed(char *fname, GRAY_IMAGE *image, unsigned char maxGrayLevel) {
    unsigned short numberBitsRequired;

    validateGrayLevel(maxGrayLevel);
    FILE *fp;

    numberBitsRequired = log2(maxGrayLevel) + 1; // calc how many bits needed for max gray value
    fp = fopen(fname, "wb");
    checkFileOpen(fname, fp);
    writeHeaderCompressedFile(image, maxGrayLevel, fp);

    insertPixelsDataToCompressedFile(image, maxGrayLevel, numberBitsRequired, fp);

    fclose(fp); // close the file
}

/*
 * inserts the data to new compressed image
 * *image - the given gray image
 * maxGrayLevel - the new max gray level
 * numberBitsRequired - number of bits that required to store a pixel
 * *fp - pointer to the file
 */
void
insertPixelsDataToCompressedFile(const GRAY_IMAGE *image, unsigned char maxGrayLevel, unsigned short numberBitsRequired,
                                 FILE *fp) {
    unsigned char temp;
    unsigned short remainingBits = 8;

    BYTE c = 0;
    for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {
            temp = image->pixels[i][j];
            temp = temp * maxGrayLevel / 255; // calculate new compressed value
            shiftingValuesAndWriteToFile(numberBitsRequired, &remainingBits, &c, temp, fp);
        }
    }
}

/*
 * the function shifts the values to store them in the minimum required bits
 * numberBitsRequired - number of bits that required to store a pixel
 * *remainingBits - pointer to how many bits are remaining in current byte
 * *c - pointer to the value that will be stored in the file
 * temp - the value to store
 * *fp - pointer to the compressed file
 */
void shiftingValuesAndWriteToFile(unsigned short numberBitsRequired, unsigned short *remainingBits,
                                  BYTE *c, unsigned char temp, FILE *fp) {
    if (*remainingBits - numberBitsRequired >= 0) {
        *c = *c | (temp << (*remainingBits - numberBitsRequired)); // add the bits to correct position in the byte
        *remainingBits -= numberBitsRequired;
    } else {
        (*c) = (*c) | (temp >> (numberBitsRequired - *remainingBits));
        fwrite(c, sizeof(BYTE), 1, fp); // write the byte
        (*c) = 0;
        // now we will add the "overflow" of remaining bits
        *remainingBits = 8 - (numberBitsRequired - *remainingBits);
        *c = *c | (temp << *remainingBits);
    }
}

/*
 * writes a header for compressed file
 * *image - the given gray image
 * maxGrayLevel - the new max gray level
 * *fp - pointer to the compressed file
 */
void writeHeaderCompressedFile(const GRAY_IMAGE *image, unsigned char maxGrayLevel, FILE *fp) {
    fwrite(&image->rows, sizeof(unsigned short), 1, fp);
    fwrite(&image->cols, sizeof(unsigned short), 1, fp);
    fwrite(&maxGrayLevel, sizeof(unsigned char), 1, fp);
}

/*
 * validates that the given max gray level is ok
 * maxGrayLevel - the given max gray level
 */
void validateGrayLevel(unsigned char maxGrayLevel) {
    if (maxGrayLevel > 127) {
        printf("Invalid max gray level (max is 127)");
        return;
    }
}

/*
 * returns the log2 of a value
 * c - the value (as unsigned char)
 */
unsigned short log2(unsigned char c) {
    unsigned short res = 0;
    while (c >>= 1) { // using the bits for the calc
        res++;
    }
    return res;
}

/*
 * converts a compressed image into a pgm file
 * *fname - the name of the compressed file
 */
void convertCompressedImageToPGM(char *fname) {
    FILE *binFile, *fp;
    unsigned short maxGrayLevel, numberBitsRequired, remainingBits, valueForMask;
    unsigned short width, height;
    char *newName;
    BYTE c;

    binFile = fopen(fname, "rb");
    checkFileOpen(fname, binFile);

    newName = getNewName(fname, ".pgm", 4);
    fp = fopen(newName, "w");
    checkFileOpen(fname, fp);

    readHeaderOfCompressedFile(binFile, &maxGrayLevel, &width, &height);

    writeHeaderForPPMFile(fp, maxGrayLevel, width, height, P2);

    numberBitsRequired = log2(maxGrayLevel) + 1;
    remainingBits = 8;
    valueForMask = getValueForMask(maxGrayLevel);
    fread(&c, sizeof(BYTE), 1, binFile);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            readBinaryFileAndWritePgm(numberBitsRequired, &remainingBits, valueForMask, &c, binFile, fp);
        }
    }
}

/*
 * the function reads the binary data of the compressed file and writes to P2 PGM file
 */
void readBinaryFileAndWritePgm(unsigned short numberBitsRequired, unsigned short *remainingBits,
                               unsigned short valueForMask, BYTE *c, FILE *binFile, FILE *fp) {
    BYTE mask, prevC;
    unsigned short newValue;

    if (*remainingBits - numberBitsRequired >= 0) {
        mask = valueForMask << (*remainingBits - numberBitsRequired);
        newValue = ((*c) & mask) >> (*remainingBits - numberBitsRequired); // using the mask we can take the orig value
        fprintf(fp, "%u ", newValue);
        *remainingBits -= numberBitsRequired;
    } else { // the value is "divided" to 2 bytes
        prevC = (*c);
        fread(c, sizeof(BYTE), 1, binFile);
        // store the start of the orig value
        prevC <<= numberBitsRequired - *remainingBits;
        prevC = prevC | ((*c) >> (8 - (numberBitsRequired - *remainingBits)));

        mask = valueForMask;
        newValue = prevC & mask;
        fprintf(fp, "%u ", newValue);
        *remainingBits = 8 - (numberBitsRequired - *remainingBits);
    }
}

/*
 * the function reads the header of a compressed file
 * *binFile - pointer to the compressed file
 * *maxGrayLevel - pointer to max gray level
 * *width - pointer to width of image
 * *height - pointer to height of image
 */
void
readHeaderOfCompressedFile(FILE *binFile, unsigned short *maxGrayLevel, unsigned short *width, unsigned short *height) {
    fread(height, sizeof(unsigned short), 1, binFile);
    fread(width, sizeof(unsigned short), 1, binFile);
    fread(maxGrayLevel, sizeof(unsigned char), 1, binFile);
}

/*
 * creates a dynamic mask for reading the value from compressed file by a given max value
 * maxValue - the max value that can be
 */
unsigned short getValueForMask(unsigned short maxValue) {
    unsigned short base2 = 2;
    unsigned short res = 1;
    while (maxValue >>= 1) {
        res += base2;
        base2 *= 2;
    }
    return res;
}