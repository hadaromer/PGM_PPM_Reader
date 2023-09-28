//Omer Hadar 2073388331 and Dor Wachner 208277343

#include "utils.h"
#include "colorImage.h"
#include "grayImage.h"
#include "segmentUtils.h"
#include "compresser.h"
#include "converter.h"

void main() {
    int choice;
    char fname[100];
    COLOR_IMAGE *colorImage = NULL;
    GRAY_IMAGE *grayImage = NULL,*grayImage2 = NULL,*grayImage3 = NULL;
    unsigned char threshold;
    IMG_POS_LIST *segments = NULL;
    int numberOfSegments;
    unsigned char maxGrayLevel; 
    printf("Hello and welcome to our project\n please select what you want to do:\n");
    printf("1. Read an image file in PPM (p3) format\n"
           "2. Read an image file in PGM (p2) format\n"
           "3. Find all segments\n"
           "4. Color the segments\n"
           "5. Save the colored image to PGM\n"
           "6. Compress and save the original image in a compressed format\n"
           "7. Convert a compressed image to PGM format\n"
           "8. Convert PPM to PGM (p3 -> p2)\n"
           "9. Convert PGM to BW (p2 -> p2)\n"
           "10.Convert PPM to PGM (p6 -> p5)\n"
           "11.Convert PGM to BW (p5 -> p5)\n");
    scanf("%d", &choice);
    while(choice>=1 && choice <=11){
        switch (choice) {
            case 1:
                printf("Please enter name of PPM file (P3):\n");
                scanf("%s",fname);
                colorImage = readPPM(fname);
                break;
            case 2:
                printf("Please enter name of PGM file (P2):\n");
                scanf("%s",fname);
                grayImage = readPGM(fname);
                break;
            case 3:
                if(grayImage != NULL){
                    printf("Please enter the threshold:\n");
                    scanf("%c",threshold);
                    numberOfSegments = findAllSegments(grayImage,threshold,&segments);
                }
                else{
                    printf("Please insert gray image first (option 2)");
                }
                break;
            case 4:
                if(segments == NULL){
                    if(grayImage != NULL){
                        printf("Please enter the threshold:\n");
                        scanf("%c",threshold);
                        numberOfSegments = findAllSegments(grayImage,threshold,&segments);
                    }
                    else{
                        printf("Please insert gray image first (option 2)");
                        break;
                    }
                }
                else{
                    grayImage2 = colorSegments(segments,numberOfSegments);
                }
                break;
            case 5:
                if(grayImage2 != NULL) {
                    writePgmFile("colorSegmnets.pgm", grayImage2, P2);
                }
                else {
                    printf("Please color segments first (option 4)");
                }
                break;
            case 6:
                printf("Please enter name of PGM file (P2):\n");
                scanf("%s",fname);
                grayImage3 = readPGM(fname);
                printf("Please enter name of compressed file:\n");
                scanf("%s",fname);
                printf("Please enter value of max gray level:\n");
                scanf("%c",&maxGrayLevel);
                saveCompressed(fname,grayImage3,maxGrayLevel);
                break;
            case 7:
                printf("Please enter name of compressed file:\n");
                scanf("%s",fname);
                convertCompressedImageToPGM(fname);
                break;
            case 8:
                printf("Please enter name of PPM file (P3):\n");
                scanf("%s",fname);
                convertPPMToPGM(fname);
                break;
            case 9:
                printf("Please enter name of PGM file (P2):\n");
                scanf("%s",fname);
                convertPGMToBW(fname);
                break;
            case 10:
                printf("Please enter name of PPM file (P6):\n");
                scanf("%s",fname);
                convertPGMToBW_Bin(fname);
                break;
            case 11:
                printf("Please enter name of PGM file (P2):\n");
                scanf("%s",fname);
                convertPGMToBW_Bin(fname);
                break;
        }
        printf("Please enter your new choice or -1 to finish\n");
        scanf("%d", &choice);
    }
}
