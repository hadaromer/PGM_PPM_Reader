//Omer Hadar 2073388331 and Dor Wachner 208277343

#include "utils.h"


void setImageDimmensions(unsigned short width, unsigned short *imageWidth, unsigned short height,
                         unsigned short *imageHeight) {
    *imageHeight = height;
    *imageWidth = width;
}


void readHeader(FILE *fp, char *c, unsigned short *width, unsigned short *height, unsigned short *maxDepth) {
    (*width) = getNextNumber(fp, c);
    (*height) = getNextNumber(fp, c);
    (*maxDepth) = getNextNumber(fp, c);
    *c = getc(fp);
}

void validateFile(FILE *fp, char *c, char expected) {
    fscanf(fp, "P%c", c);

    if ((*c) != expected) {
        printf("invalid file!!!");
        exit(1);
    }
}


/*
 * skips a comment in a file
 * *fp - pointer to the file
 * *c - pointer to char
 */
void skipComments(FILE *fp, char *c) {
    if (*c == '#') {
        *c = getc(fp);
        while (*c != '\n') { // comments extends to the end of line
            *c = getc(fp);
        }
    }
    *c = ungetc(*c, fp); // return to last char
}


unsigned short getNextNumber(FILE *fp, char *c) {
    unsigned short temp;
    *c = getc(fp);
    while (!isDigit(*c)) {
        if (*c == '#') skipComments(fp, c);
        *c = getc(fp);
    }
    *c = ungetc(*c, fp);
    fscanf(fp, "%hu", &temp);
    return temp;
}

/*
 * generic function that check that the file operation succeed
 */
void checkFileOpen(const char *fname, const FILE *fp) {
    if (fp == NULL) {
        printf("Could not open %s\n", fname);
        exit(1);
    }
}

/*
 * generic function that check that the memory allocation succeed
 */
void checkMemoryAllocation(void *ptr) {
    if (ptr == NULL) {
        printf("Memory allocation failure!!!\n");
        exit(1);
    }
}

void copyPosition(IMG_POS src, IMG_POS dest) {
    dest[0] = src[0];
    dest[1] = src[1];
}

char *getNewName(char *old, char *extension, unsigned short extensionSize) {
    unsigned short oldSize;
    unsigned short i;
    char *res;

    oldSize = getStringLen(old);
    res = (char *) malloc(sizeof(char) * (oldSize + extensionSize + 1)); // 1 for  '\0'
    checkMemoryAllocation(res);
    for (i = 0; i < oldSize; i++) {
        res[i] = old[i];
    }
    for (; i < (oldSize + extensionSize); i++) {
        res[i] = *extension;
        extension++;
    }
    res[i] = '\0';
    return res;
}

unsigned short getStringLen(char *old) {
    unsigned short counter = 0;
    while (old[counter] != '\0' && old[counter] != '.') {
        counter++;
    }
    return counter;
}
