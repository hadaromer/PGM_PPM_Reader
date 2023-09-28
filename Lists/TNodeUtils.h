//Omer Hadar 2073388331 and Dor Wachner 208277343

#ifndef TNODEUTILS_H
#define TNODEUTILS_H

#include "../utils.h"

typedef struct tnode TNODE;

typedef struct tnode_lnode {
    TNODE *node;
    struct tnode_lnode *next;
} TNODE_LNODE;
typedef struct tnode_list {
    TNODE_LNODE *head;
    TNODE_LNODE *tail;
} TNODE_LIST;

typedef struct tnode {
    IMG_POS position;
    TNODE_LIST nextPossiblePositions;//הילדים רשימת
} TNODE;

TNODE * createTNODE(IMG_POS pos);

TNODE_LNODE *createTNODE_LNODE(IMG_POS pos);

BOOL isEmptyTNODE_LIST(TNODE_LIST *list);

void insertNodeToEndTNODE_LIST(TNODE_LIST *list, TNODE_LNODE *tail);

void insertDataToEndTNODE_LIST(TNODE_LIST *list, IMG_POS pos);

void makeEmptyTNODE_LIST(TNODE_LIST *list);


#endif //TNODEUTILS_H
