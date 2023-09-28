//Omer Hadar 2073388331 and Dor Wachner 208277343

#include "segmentUtils.h"

/*
 * finds a single segment from a start point in a gray image by a given threshold
 * *img - the gray image
 * start - the start position
 * threshold - threshold for the segment
 */
SEGMENT findSingleSegment(GRAY_IMAGE *img, IMG_POS start, unsigned char threshold) {
    BOOL **isPosVisited;
    SEGMENT *res;
    unsigned char min, max;

    res = createSegment(start);
    isPosVisited = createBoolArrayForVisitedPositions(img);
    isPosVisited[start[0]][start[1]] = TRUE; // start will be the root and obviously will be visited
    findMinMaxValues(img->pixels[start[0]][start[1]], threshold, &min, &max);
    findSingleSegmentHelper(img, min, max, isPosVisited, res->root, NULL);
    freeBoolArray(isPosVisited, img->rows);
    return *res;
}

/*
 * creates a segment and return it
 * pos - the start position of the segment
 */
SEGMENT *createSegment(IMG_POS pos) {
    SEGMENT *res;
    res = (SEGMENT *) malloc(sizeof(SEGMENT));
    checkMemoryAllocation(res);
    res->root = createTNODE(pos);
    return res;
}

/*
 * find the min max values by threshold and the start char
 * startChar - the value to check min max from
 * threshold - threshold for the segment
 * *min - pointer to the min value
 * *max - pointer to the max value
 */
void findMinMaxValues(unsigned char startChar, unsigned char threshold, unsigned char *min, unsigned char *max) {
    *min = startChar - threshold > MIN_VALUE ? startChar - threshold : MIN_VALUE;
    *max = startChar + threshold < MAX_VALUE ? startChar + threshold : MAX_VALUE;
}

/*
 * helper recursive function to find a single segment
 * *img - the gray image
 * mix - minimum allowed value
 * max - maximum allowed value
 * **isPosVistied - bool array that holds which positions visited
 * *root - pointer to the root of tree
 * *imgPosList - the list of the segment
 */
void findSingleSegmentHelper(GRAY_IMAGE *img, unsigned char min, unsigned char max, BOOL **isPosVisited, TNODE *root,
                             IMG_POS_LIST *imgPosList) {

    insertPossiblePositions(img, min, max, isPosVisited, root, imgPosList);

    continueFromRootPositions(img, min, max, isPosVisited, root, imgPosList);

}

/*
 * continue the recursive function from the root childs
 **img - the gray image
 * mix - minimum allowed value
 * max - maximum allowed value
 * **isPosVistied - bool array that holds which positions visited
 * *root - pointer to the root of tree
 * *imgPosList - the list of the segment
 */
void
continueFromRootPositions(GRAY_IMAGE *img, unsigned char min, unsigned char max, BOOL **isPosVisited, TNODE *root,
                          IMG_POS_LIST *imgPosList) {
    TNODE_LNODE *curr;

    curr = root->nextPossiblePositions.head;

    while (curr != NULL) {
        findSingleSegmentHelper(img, min, max, isPosVisited, curr->node, imgPosList);
        curr = curr->next;
    }
}

/*
 * insert possible positions to the tree and list(if thre is a list)
 **img - the gray image
 * mix - minimum allowed value
 * max - maximum allowed value
 * **isPosVistied - bool array that holds which positions visited
 * *root - pointer to the root of tree
 * *imgPosList - the list of the segment
 */
void insertPossiblePositions(const GRAY_IMAGE *img, unsigned char min, unsigned char max, BOOL **isPosVisited,
                             const TNODE *root, IMG_POS_LIST *imgPosList) {
    char c;
    unsigned short posX, posY;
    IMG_POS tempPos;

    makeEmptyTNODE_LIST(&root->nextPossiblePositions);
    for (int i = 0; i < VECTORS_SIZE; i++) {
        posX = root->position[0] + VECTORS[i].x;
        posY = root->position[1] + VECTORS[i].y;
        if (posX < img->rows && posY < img->cols) {
            if (isPosVisited[posX][posY] == FALSE) {
                c = img->pixels[posX][posY];
                if (c >= min && c <= max) {
                    isPosVisited[posX][posY] = TRUE;
                    tempPos[0] = posX;
                    tempPos[1] = posY;
                    insertDataToEndTNODE_LIST(&root->nextPossiblePositions, tempPos);
                    if (imgPosList != NULL) { insertDataToEndIMG_POS_LIST(imgPosList, tempPos); }
                }
            }
        }
    }
}

/*
 * creates a bool array for visited positions
 * *img - the gray image to make the array for
 */
BOOL **createBoolArrayForVisitedPositions(GRAY_IMAGE *img) {
    int i;
    BOOL **res;
    BOOL *tempBoolArr;

    res = (BOOL **) malloc(sizeof(BOOL *) * img->rows);
    checkMemoryAllocation(res);
    for (i = 0; i < img->rows; i++) {
        tempBoolArr = (BOOL *) malloc(sizeof(BOOL) * img->cols);
        checkMemoryAllocation(tempBoolArr);
        res[i] = tempBoolArr;
    }

    return res;
}

/*
 * frees a bool array using image dimensions
 * **arr - the array to free
 * rows - number of rows in the array
 */
void freeBoolArray(BOOL **arr, int rows){
    int i;

    for (i = 0; i < rows; i++) {
        free(arr[i]);
    }
    free(arr);
}

/*
 * find all segments in an image
 * returns the number of segments and list of them
 * *img - the gray image
 * threshold - the threshold for a segment
 * **segments - the segments that will be returned
 */
int findAllSegments(GRAY_IMAGE *img, unsigned char threshold, IMG_POS_LIST **segments) {
    SEGMENT *tempSegment;
    BOOL **isPosVisited;
    unsigned char min, max;
    IMG_POS currPos;
    IMG_POS_LIST *imgPosLists;
    int logicalSize = 0, physicalSize = 1; // unknown segements size
    BOOL loop = TRUE;
    int i, j;

    imgPosLists = initImgPosLists();
    isPosVisited = createBoolArrayForVisitedPositions(img);
    // start from (0,0)
    currPos[0] = 0;
    currPos[1] = 0;
    for (i = 0; i < img->rows; i++) {
        for (j = 0; j < img->cols; j++) {
            if (isPosVisited[i][j] == FALSE) {
                isPosVisited[i][j] = TRUE;
                findMinMaxValues(img->pixels[i][j], threshold, &min, &max);
                currPos[0] = i;
                currPos[1] = j;
                tempSegment = createSegment(currPos);
                insertDataToEndIMG_POS_LIST(&imgPosLists[logicalSize], currPos);
                findSingleSegmentHelper(img, min, max, isPosVisited, tempSegment->root, &imgPosLists[logicalSize]);
                logicalSize++;
                if (logicalSize == physicalSize) {
                    resizeImgPosLists(&imgPosLists, &physicalSize); // resize if needed
                }
            }
        }
    }
    free(tempSegment);
    freeBoolArray(isPosVisited,img->rows);
    imgPosLists = (IMG_POS_LIST *) realloc(imgPosLists, sizeof(IMG_POS_LIST) * logicalSize-1);
    *segments = imgPosLists;
    return logicalSize;
}


/*
 * init an image positions lists array
 */
IMG_POS_LIST *initImgPosLists() {
    IMG_POS_LIST *arr;
    arr = (IMG_POS_LIST *) malloc(sizeof(IMG_POS_LIST)); // creates an images positions list with size 1
    checkMemoryAllocation(arr);
    return arr;
}

/*
 * double the size of an image lists araay by double the size and copy the data to new memory allocation
 * returns as an out parameter the new size of the array
 * **pArray - address of the array
 * *pPhysicalSize - address of physical size of the array
 */
void resizeImgPosLists(IMG_POS_LIST **pArray, unsigned int *pPhysicalSize) {
    (*pPhysicalSize) *= 2; // double the size
    (*pArray) = (IMG_POS_LIST *) realloc((*pArray), sizeof(IMG_POS_LIST) * (*pPhysicalSize));
    checkMemoryAllocation(*(pArray));
}

/*
 * create a gray image by segments
 * *segments - the segments
 * size - size of segments
 */
GRAY_IMAGE *colorSegments(IMG_POS_LIST *segments, unsigned int size) {
    GRAY_IMAGE *res;
    unsigned short width, height;

    res = createGrayImage();
    getImageDimensionsBySegments(segments, size, &width, &height);
    setImageDimmensions(width, &res->cols, height, &res->rows);
    createPixelsArrayForGrayImage(res);
    colorGrayImageBySegments(segments,size,res);

    return res;
}

/*
 * checks if a position b is bigger than a
 * *a - first position
 * *b - second position
 */
BOOL isPositionBigger(IMG_POS a, IMG_POS b){ //check
    if(a[0] < b[0] && a[1] < b[1]){
        return TRUE;
    }
    return FALSE;
}

/*
 * gets the image dimensions by segmnets
 * *segments - the given segments
 * size - size of the segments
 * *width - pointer for width
 * *height - pointer for height
 */
void getImageDimensionsBySegments(IMG_POS_LIST *segments, unsigned int size, unsigned short *width, unsigned short *height){
    int i;
    IMG_POS tempPos;
    IMG_POS_NODE *curr;

    i=0;
    tempPos[0] = 0;
    tempPos[1] = 0;
    for(i=0;i<size;i++){
    curr = segments[i].head;
        while (curr != NULL){
            if(isPositionBigger(tempPos,curr->position) == TRUE){
                copyPosition(curr->position,tempPos);
            }
            curr = curr->next;
        }
    }
    *width = tempPos[0];
    *height = tempPos[1];
}

/*
 * color the gray image by the segments
 * *segments - the given segments
 * segmentsSize - the size of segments
 * *grayImage - the gray image to color
 */
void colorGrayImageBySegments(IMG_POS_LIST *segments, unsigned int segmentsSize,GRAY_IMAGE *grayImage){
    int i;
    IMG_POS_NODE *curr;


    for(i=0; i< segmentsSize; i++){
    curr = segments[i].head;
        while (curr != NULL){
            grayImage->pixels[curr->position[0]][curr->position[1]] = i*(MAX_VALUE/segmentsSize);
            curr = curr->next;
        }
    }
}

