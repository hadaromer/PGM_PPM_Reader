//Omer Hadar 2073388331 and Dor Wachner 208277343

#include "converter.h"

/*
 * converts a ppm P3 to a pgm P2 file
 * *fname - name of the ppm file
 */
void convertPPMToPGM(char *fname) {
    convertPPMToPGMHelper(fname, P3, P2);
}

/*
 * converts a ppm P6 to a pgm P5 file
 * *fname - name of the ppm file
 */
void convertPPMToPGM_Bin(char *fname) {
    convertPPMToPGMHelper(fname, P6, P5);
}

/*
 * converts a ppm to a pgm file
 * *fname - name of the ppm file
 * ppmType - type of ppm file to read
 * pgmType - type of pgm file to convert
 */
void convertPPMToPGMHelper(char *fname, IMAGE_TYPE ppmType, IMAGE_TYPE pgmType) {
    COLOR_IMAGE *colorImage;
    GRAY_IMAGE *grayImage;
    unsigned char maxGrayValue;
    FILE *fp;

    colorImage = readPPMHelper(fname, ppmType);
    fp = createNewPGMFile(fname, pgmType);
    grayImage = createGrayImage();
    setImageDimmensions(colorImage->cols, &grayImage->cols, colorImage->rows, &grayImage->rows);
    createPixelsArrayForGrayImage(grayImage);
    insertPixelsDataToGrayArrayFromColorImage(grayImage, colorImage);
    maxGrayValue = findMaxValueInGrayImagePixels(grayImage);

    writeHeaderForPPMFile(fp, maxGrayValue, grayImage->cols, grayImage->rows, pgmType);
    switch (pgmType) {
        case P2:
            writeGrayImagePixelsToP2File(fp, grayImage);
            break;
        case P5:
            writeGrayImagePixelsToP5File(fp, grayImage);
            break;
    }

    fclose(fp);

    freeColorImage(colorImage);
    freeGrayImage(grayImage);
}

/*
 * create new pgm file using ppm name and pgm type
 * *fname - orig ppm name
 * pgmType - the new type of ppm
 */
FILE *createNewPGMFile(char *fname, IMAGE_TYPE pgmType) {
    FILE *res;
    char *newName;

    newName = getNewName(fname, ".pgm", 4);
    res = fopen(newName, pgmType == P2 ? "w" : "wb");
    checkFileOpen(newName, res);
    return res;
}

/*
 * inserts pixel data to gray image array
 * *g - pointer to gray image
 * *c - pointer to color image
 */
void insertPixelsDataToGrayArrayFromColorImage(GRAY_IMAGE *g, COLOR_IMAGE *c) {
    unsigned short i, j;

    for (i = 0; i < g->rows; i++) {
        for (j = 0; j < g->cols; j++) {
            g->pixels[i][j] = getGrayVariety(c->pixels[i][j]);
        }
    }
}

/*
 * returns the gray value based on the given RGB value
 * color - the given RGB value
 */
unsigned char getGrayVariety(RGB color) {
    return (color.r + color.g + color.b) / 3;
}

/*
 * returns the max value in a given gray image
 * *g - the given gray image
 */
unsigned char findMaxValueInGrayImagePixels(GRAY_IMAGE *g) {
    unsigned short i, j;
    unsigned char max = 0;

    for (i = 0; i < g->rows; i++) {
        for (j = 0; j < g->cols; j++) {
            if (max < g->pixels[i][j]) {
                max = g->pixels[i][j];
            }
        }
    }
    return max;
}

/*
 * writes the gray image data to P2 file
 * *fp - the P2 file to write to
 * *g - the color image
 */
void writeGrayImagePixelsToP2File(FILE *fp, GRAY_IMAGE *g) {
    unsigned short i, j;

    for (i = 0; i < g->rows; i++) {
        for (j = 0; j < g->cols; j++) {
            fprintf(fp, "%u ", g->pixels[i][j]);
        }
    }
}

/*
 * writes the gray image data to P5 file
 * *fp - the P5 file to write to
 * *g - the color image
 */
void writeGrayImagePixelsToP5File(FILE *fp, GRAY_IMAGE *g) {
    unsigned short i;

    for (i = 0; i < g->rows; i++) {
        fwrite(&g->pixels[i][0], sizeof(unsigned char), g->cols, fp);
    }
}

/*
 * converts a pgm to black white image (P2)
 * *fname - the pgm name
 */
void convertPGMToBW(char *fname) {
    convertPGMToBWHelper(fname, P2);
}

/*
 * converts a pgm to black white image (P2)
 * *fname - the pgm name
 * imageType - the type of the pgm to read and save
 */
void convertPGMToBWHelper(char *fname, IMAGE_TYPE imageType) {
    GRAY_IMAGE *grayImage;
    unsigned char maxGrayValue;

    grayImage = readPGMHelper(fname, imageType);
    BLACK_WHITE_FILES *files = createBlackWhiteFiles();
    initBlackWhiteFiles(fname, files, imageType);
    maxGrayValue = 1; // black white image so the max gray value is 1
    writeHeadersForBlackWhiteFiles(grayImage, maxGrayValue, files, imageType);
    writeBlackWhiteFilesData(grayImage, files, imageType);
    closeBlackWhiteFiles(files);

    freeGrayImage(grayImage);
    free(files);
}

/*
 * write the black or white data to the files
 * *grayImage - pointer to the gray image
 * *files - array of all the files to save
 * imageType - the type of the pgm to save
 */
void writeBlackWhiteFilesData(GRAY_IMAGE *grayImage, BLACK_WHITE_FILES *files, IMAGE_TYPE imageType) {
    unsigned char newValue;
    unsigned short Masks[3][4][4] = {
            {{0, 2},        {1,  3}},
            {{2, 6, 4},     {5,  0, 1},     {8, 3,  7}},
            {{0, 8, 2, 10}, {12, 4, 14, 6}, {3, 11, 1, 9}, {15, 7, 13, 5}}
    }; // the masks for the algorithm
    unsigned char divider[3] = {(255 / (2 * 2)), (255 / (3 * 3)), (255 / (4 * 4))}; // all the dividers for the algorithm
    for (unsigned short i = 0; i < grayImage->rows; i++) {
        for (unsigned short j = 0; j < grayImage->cols; j++) {
            for (int k = 0; k < 3; k++) {
                if (grayImage->pixels[i][j] / divider[k] > Masks[k][i % (k + 2)][j % (k + 2)]) { // check if white or black
                    newValue = 1;
                } else {
                    newValue = 0;
                }
                switch (imageType) {
                    case P2:
                        fprintf(files[k].fp, "%u ", newValue);
                        break;
                    case P5:
                        fwrite(&newValue, sizeof(unsigned char), 1, files[k].fp);
                }
            }
        }
    }
}

/*
 * closes all the files in the array
 * *files - the array of files
 */
void closeBlackWhiteFiles(BLACK_WHITE_FILES *files) {
    for (int k = 0; k < NUMBER_OF_BW_FILES; k++)
        fclose(files[k].fp);
}

/*
 * writes headers for the files in the array by the gray image and image type
 * *grayImage - the given gray image
 * *files - the array of files
 * imageType - the type of the pgm to save
 */
void
writeHeadersForBlackWhiteFiles(GRAY_IMAGE *grayImage, unsigned char maxGrayValue, BLACK_WHITE_FILES *files,
                               IMAGE_TYPE imageType) {
    for (int i = 0; i < NUMBER_OF_BW_FILES; i++) {
        writeHeaderForPPMFile(files[i].fp, maxGrayValue, grayImage->cols, grayImage->rows, imageType);
    }
}

/*
 * init the bw files array
 * *fname - orig file name
 * *files - pointer to the array
 * imageType - the type of the pgm to save
 */
void initBlackWhiteFiles(char *fname, BLACK_WHITE_FILES *files, IMAGE_TYPE imageType) {
    char extensions[3][7] = {
            {"bw2.pgm"},
            {"bw3.pgm"},
            {"bw4.pgm"}
    };
    for (int i = 0; i < 3; i++) {
        files[i].k = i + 2;
        files[i].fname = getNewName(fname, extensions[i], 7);
        files[i].fp = fopen(files[i].fname, imageType == P2 ? "w" : "wb");
        checkFileOpen(files[i].fname, files[i].fp);
    }
}

/*
 * allocate memory for bw files arrray
 */
BLACK_WHITE_FILES *createBlackWhiteFiles() {
    BLACK_WHITE_FILES *res = (BLACK_WHITE_FILES *) malloc(sizeof(BLACK_WHITE_FILES) * NUMBER_OF_BW_FILES);
    checkMemoryAllocation(res);
    return res;
}

/*
 * converts a pgm to black white image (P5)
 * *fname - the pgm name
 */
void convertPGMToBW_Bin(char *fname) {
    convertPGMToBWHelper(fname, P5);
}