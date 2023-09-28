//Omer Hadar 2073388331 and Dor Wachner 208277343

#include "grayImage.h"


/*
 * creates a GRAY_IMAGE from a given file and returns a pointer to it
 * fname - the given text file name
 * the text's format is PPM with Magic Number: P2
 */
GRAY_IMAGE *readPGM(char *fname) {
    return readPGMHelper(fname,P2);
}

/*
 * creates a GRAY_IMAGE from a given file and returns a pointer to it
 * fname - the given text file name
 * the text's format is PPM with Magic Number: P5
 */
GRAY_IMAGE *readPGMP5(char *fname) {
    return readPGMHelper(fname,P5);
}

/*
 * creates a GRAY_IMAGE from a given file and returns a pointer to it
 * fname - the given text file name
 * imageType - the image type P2/P5
 */
GRAY_IMAGE *readPGMHelper(char *fname, IMAGE_TYPE imageType) {
    FILE *fp;
    GRAY_IMAGE *res;
    char c;
    unsigned short width, height, maxDepth;

    fp = fopen(fname, imageType == P2 ? "r" : "rb");
    checkFileOpen(fname, fp);
    validateFile(fp, &c, imageType);
    readHeader(fp, &c, &width, &height, &maxDepth);
    res = createGrayImage();
    setImageDimmensions(width, &res->cols, height, &res->rows);
    createPixelsArrayForGrayImage(res);
    switch (imageType) {
        case P2:
            insertPixelsDataToGrayArray(fp, res);
            break;
        case P5:
            insertPixelsDataToGrayArrayP5(fp, res);
            break;
    }
    fclose(fp);
    return res;
}

/*
 * allocate memory for a GRAY_IMAGE and returns a pointer to it
 */
GRAY_IMAGE *createGrayImage() {
    GRAY_IMAGE *pTemp;
    pTemp = (GRAY_IMAGE *) malloc(sizeof(GRAY_IMAGE));
    checkMemoryAllocation(pTemp);
    return pTemp;
}

/*
 * allocate memory for the pixels array of a gray image
 * *res - pointer to the gray image
 */
void createPixelsArrayForGrayImage(GRAY_IMAGE *res) {
    int i;
    char *tempCharArr;

    res->pixels = (unsigned char **) malloc(sizeof(unsigned char *) * res->rows);
    checkMemoryAllocation(res->pixels);
    for (i = 0; i < res->rows; i++) {
        tempCharArr = (unsigned char *) malloc(sizeof(unsigned char) * res->cols);
        checkMemoryAllocation(tempCharArr);
        res->pixels[i] = tempCharArr;
    }
}

/*
 * inserts pixels data into the char pixels array of a gray image
 * *fp - pointer to the file to read from (P2 file)
 * *res - pointer to the gray image
 */
void insertPixelsDataToGrayArray(FILE *fp, GRAY_IMAGE *res) {
    unsigned short i, j;
    char *pCurrPos;

    for (i = 0; i < res->rows; i++) {
        for (j = 0; j < res->cols; j++) {
            pCurrPos = &res->pixels[i][j];
            fscanf(fp, "%d", pCurrPos);
        }
    }
}

/*
 * inserts pixels data into the char pixels array of a gray image
 * *fp - pointer to the file to read from (P5 file)
 * *res - pointer to the gray image
 */
void insertPixelsDataToGrayArrayP5(FILE *fp, GRAY_IMAGE *res) {
    unsigned short i;
    for(i=0;i<res->rows;i++) {
        fread(&res->pixels[i][0],sizeof(unsigned char),res->cols,fp);
    }
}

void writeHeaderForPPMFile(FILE *fp, unsigned short maxGrayLevel, unsigned short width, unsigned short height,
                           IMAGE_TYPE imageType) {
    switch (imageType) {
        case P2:
            fprintf(fp, "P2 ");
            break;
        case P5:
            fprintf(fp, "P5 ");
            break;
    }

    fprintf(fp, "%u %u ", width, height);
    fprintf(fp, "%u\n", maxGrayLevel);
}

void writePgmFile(char *fname,GRAY_IMAGE *g,IMAGE_TYPE imageType){
    FILE *fp;
    fp = fopen(fname, imageType == P2 ? "w" : "wb");
    checkFileOpen(fname, fp);
    writeHeaderForPPMFile(fp,MAX_VALUE,g->cols,g->rows,imageType);
    switch (imageType) {
        case P2:
            insertPixelsDataToGrayArray(fp, g);
            break;
        case P5:
            insertPixelsDataToGrayArrayP5(fp, g);
            break;
    }
    fclose(fp);
}

/*
 * frees a given gray image
 * *c - pointer to the color image
 */
void freeGrayImage(GRAY_IMAGE *c){
    unsigned short i;

    for (i = 0; i < c->rows; i++) {
        free(c->pixels[i]);
    }
    free(c->pixels);
    free(c);
}