//Omer Hadar 2073388331 and Dor Wachner 208277343

#ifndef IMGPOSUTILS_H
#define IMGPOSUTILS_H

#include "../utils.h"

typedef struct img_pos_node {
    IMG_POS position;
    struct img_pos_node *next, *prev;
} IMG_POS_NODE;
typedef struct img_pos_list {
    IMG_POS_NODE *head;
    IMG_POS_NODE *tail;
} IMG_POS_LIST;

IMG_POS_NODE *createIMG_POS_NODE(IMG_POS pos);

BOOL isEmptyIMG_POS_LIST(IMG_POS_LIST *list);

void insertNodeToEndIMG_POS_LIST(IMG_POS_LIST *list, IMG_POS_NODE *tail);

void insertDataToEndIMG_POS_LIST(IMG_POS_LIST *list, IMG_POS pos);

void makeEmptyIMG_POS_LIST(IMG_POS_LIST *list);


#endif //IMGPOSUTILS_H
