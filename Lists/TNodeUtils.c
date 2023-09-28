//Omer Hadar 2073388331 and Dor Wachner 208277343

#include "TNodeUtils.h"

/*
 * creates a TNODE by IMG_POS
 * pos - the given IMG_POS
 */
TNODE * createTNODE(IMG_POS pos) {
    TNODE *res;
    res = (TNODE *) malloc(sizeof(TNODE));
    checkMemoryAllocation(res);
    copyPosition(pos, res->position);
    return res;
}

/*
 * creates a TNODE_LNODE by IMG_POS
 * pos - the given IMG_POS
 */
TNODE_LNODE *createTNODE_LNODE(IMG_POS pos) {
    TNODE_LNODE *res;
    res = (TNODE_LNODE *) malloc(sizeof(TNODE_LNODE));
    checkMemoryAllocation(res);
    res->node = createTNODE(pos);
    res->next = NULL;
    return res;
}

/*
 * returns boolean value as if the list is empty
 * *lIst - address for the list
 */
BOOL isEmptyTNODE_LIST(TNODE_LIST *list) {
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
void insertNodeToEndTNODE_LIST(TNODE_LIST *list, TNODE_LNODE *tail) {
    // if the list is empty the head and the tail point to same node
    if (isEmptyTNODE_LIST(list) == TRUE) {
        list->head = list->tail = tail;
    } else {
        list->tail->next = tail;
        list->tail = tail;
    }
}

/*
 * inserts data to end of the list
 * *list - address for the list
 * pos - the position of the new TNODE_LNODE
 */
void insertDataToEndTNODE_LIST(TNODE_LIST *list, IMG_POS pos) {
    TNODE_LNODE *newTail;
    newTail = createTNODE_LNODE(pos);
    insertNodeToEndTNODE_LIST(list, newTail);
}

/*
 * makes an empty TNODE_LIST
 * *list - address of a list
 */
void makeEmptyTNODE_LIST(TNODE_LIST *list) {
    list->head = NULL;
    list->tail = NULL;
}
