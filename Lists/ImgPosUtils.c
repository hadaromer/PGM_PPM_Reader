//Omer Hadar 2073388331 and Dor Wachner 208277343

#include "ImgPosUtils.h"

/*
 * creates an IMG_POS_NODE by IMG_POS
 * pos - the given IMG_POS
 */
IMG_POS_NODE * createIMG_POS_NODE(IMG_POS pos) {
    IMG_POS_NODE *res;
    res = (IMG_POS_NODE *) malloc(sizeof(IMG_POS_NODE));
    checkMemoryAllocation(res);
    copyPosition(pos, res->position);
    res->next = NULL;
    res->prev = NULL;
    return res;
}

/*
 * returns boolean value as if the list is empty
 * *lIst - address for the list
 */
BOOL isEmptyIMG_POS_LIST(IMG_POS_LIST *list) {
    if (list->head == NULL)
        return TRUE;
    else
        return FALSE;
}

/*
*
* inserts TNODE_LNODE to the end of a TNODE_LIST
* *list - address for the list
* *tail - address to the node to insert (now it's the new tail obviously)
*/
void insertNodeToEndIMG_POS_LIST(IMG_POS_LIST *list, IMG_POS_NODE *tail) {
    // if the list is empty the head and the tail point to same node
    if (isEmptyIMG_POS_LIST(list) == TRUE) {
        list->head = list->tail = tail;
    } else {
        tail->prev = list->tail;
        list->tail->next = tail;
        list->tail = tail;
    }
}

/*
 * inserts data to end of the list
 * *list - address for the list
 * pos - the position of the new TNODE_LNODE
 */
void insertDataToEndIMG_POS_LIST(IMG_POS_LIST *list, IMG_POS pos) {
    IMG_POS_NODE *newTail;
    newTail = createIMG_POS_NODE(pos);
    insertNodeToEndIMG_POS_LIST(list, newTail);
}

/*
 * makes an empty IMG_POS_LIST
 * *list - address of a list
 */
void makeEmptyIMG_POS_LIST(IMG_POS_LIST *list) {
    list->head = NULL;
    list->tail = NULL;
}