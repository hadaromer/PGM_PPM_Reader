//Omer Hadar 2073388331 and Dor Wachner 208277343

#ifndef TNODEUTILS_H

#include "Lists/TNodeUtils.h"

#endif

#ifndef IMGPOSUTILS_H

#include "Lists/ImgPosUtils.h"

#endif

#ifndef SEGMENTUTILS_H
#define SEGMENTUTILS_H

#include "utils.h"
#include "grayImage.h"

typedef struct vector {
    unsigned short x;
    unsigned short y;
} Vector;

#define VECTORS_SIZE 8
#define VECTORS (Vector []){{-1,0},{-1,1},{0,1},{1,1},{0,1},{1,-1},{0,-1},{1,-1}}

typedef struct segment {
    TNODE *root;
} SEGMENT;

SEGMENT findSingleSegment(GRAY_IMAGE *img, IMG_POS start, unsigned char threshold);

SEGMENT *createSegment(IMG_POS pos);

void findMinMaxValues(unsigned char startChar, unsigned char threshold, unsigned char *min, unsigned char *max);

void findSingleSegmentHelper(GRAY_IMAGE *img, unsigned char min, unsigned char max, BOOL **isPosVisited, TNODE *root,
                             IMG_POS_LIST *imgPosList);

BOOL **createBoolArrayForVisitedPositions(GRAY_IMAGE *img);

void insertPossiblePositions(const GRAY_IMAGE *img, unsigned char min, unsigned char max, BOOL **isPosVisited,
                             const TNODE *root, IMG_POS_LIST *imgPosList);

void continueFromRootPositions(GRAY_IMAGE *img, unsigned char min, unsigned char max, BOOL **isPosVisited, TNODE *root,
                               IMG_POS_LIST *imgPosList);

IMG_POS_LIST *initImgPosLists();

int findAllSegments(GRAY_IMAGE *img, unsigned char threshold, IMG_POS_LIST **segments);

void resizeImgPosLists(IMG_POS_LIST **pArray, unsigned int *pPhysicalSize);

GRAY_IMAGE *colorSegments(IMG_POS_LIST *segments, unsigned int size);

BOOL isPositionBigger(IMG_POS a, IMG_POS b);

void getImageDimensionsBySegments(IMG_POS_LIST *segments, unsigned int size, unsigned short *width, unsigned short *height);

void colorGrayImageBySegments(IMG_POS_LIST *segments, unsigned int segmentsSize,GRAY_IMAGE *grayImage);

void freeBoolArray(BOOL **arr, int rows);

#endif //SEGMENTUTILS_H
