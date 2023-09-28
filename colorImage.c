//Omer Hadar 2073388331 and Dor Wachner 208277343

#include "colorImage.h"

/*
 * creates a COLOR_IMAGE from a given file and returns a pointer to it using helper
 * fname - the given text file name
 * the text's format is PPM with Magic Number: P3
 */
COLOR_IMAGE *readPPM(char *fname) {
    return readPPMHelper(fname,P3);
}

/*
 * creates a COLOR_IMAGE from a given file and returns a pointer to it using helper
 * fname - the given text file name
 * the text's format is PPM with Magic Number: P6
 */
COLOR_IMAGE *readPPM_P6(char *fname) {
    return readPPMHelper(fname,P6);
}

/*
 * creates a COLOR_IMAGE from a given file and returns a pointer to
 * fname - the given text file name
 * imageType - the image type P3/P6
 */
COLOR_IMAGE *readPPMHelper(char *fname,IMAGE_TYPE imageType) {
    FILE *fp;
    COLOR_IMAGE *res;
    char c;
    unsigned short width, height, maxDepth;

    fp = fopen(fname, imageType == P3 ? "r" : "rb");
    checkFileOpen(fname, fp);
    validateFile(fp, &c, imageType);
    readHeader(fp, &c, &width, &height, &maxDepth);
    res = createColorImage();
    setImageDimmensions(width, &res->cols, height, &res->rows);
    createPixelsArrayForColorImage(res);
    switch (imageType) {
        case P3:
            insertPixelsDataToRgbArray(fp,res);
            break;
        case P6:
            insertPixelsDataToRgbArrayP6(fp, res);
            break;
    }
    fclose(fp);
    return res;
}

/*
 * allocate memory for a COLOR_IMAGE and returns a pointer to it
 */
COLOR_IMAGE *createColorImage() {
    COLOR_IMAGE *pTemp;
    pTemp = (COLOR_IMAGE *) malloc(sizeof(COLOR_IMAGE));
    checkMemoryAllocation(pTemp);
    return pTemp;
}

/*
 * allocate memory for the pixels array of a color image
 * *res - pointer to the color image
 */
void createPixelsArrayForColorImage(COLOR_IMAGE *res) {
    unsigned short i;
    RGB *tempRgbArr;

    res->pixels = (RGB **) malloc(sizeof(RGB *) * res->rows);
    checkMemoryAllocation(res->pixels);
    for (i = 0; i < res->rows; i++) {
        tempRgbArr = (RGB *) malloc(sizeof(RGB) * res->cols);
        checkMemoryAllocation(tempRgbArr);
        res->pixels[i] = tempRgbArr;
    }
}

/*
 * inserts pixels data into the rgb pixels array of a color image
 * *fp - pointer to the file to read from (P3 file)
 * *res - pointer to the color image
 */
void insertPixelsDataToRgbArray(FILE *fp, COLOR_IMAGE *res) {
    unsigned short i, j;
    RGB *pCurrPos;

    for (i = 0; i < res->rows; i++) {
        for (j = 0; j < res->cols; j++) {
            pCurrPos = &res->pixels[i][j];
            fscanf(fp, "%d %d %d", &pCurrPos->r, &pCurrPos->g, &pCurrPos->b);
        }
    }
}

/*
 * inserts pixels data into the rgb pixels array of a color image
 * *fp - pointer to the file to read from (P6 file)
 * *res - pointer to the color image
 */
void insertPixelsDataToRgbArrayP6(FILE *fp, COLOR_IMAGE *res) {
    unsigned short i;
    for(i=0;i<res->rows;i++) {
        fread(&res->pixels[i][0],sizeof(RGB),res->cols,fp);
    }
}

/*
 * frees a given color image
 * *c - pointer to the color image
 */
void freeColorImage(COLOR_IMAGE *c){
    unsigned short i;

    for (i = 0; i < c->rows; i++) {
            free(c->pixels[i]);
    }
    free(c->pixels);
    free(c);
}
