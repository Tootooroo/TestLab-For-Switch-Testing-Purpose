/* list.c */

#include <malloc.h>
#include "type.h"
#include "list.h"

/* Private prototype */
private _Status_t __listShallowRelease(list *l);
private _Status_t __listDeepRelease(list *l);


/* Public functions */
list * listCreate(void) {
    list *l = (list *)calloc(sizeof(list), 1);

    return l;
}

_Status_t listRelease(list *l) {
    _Status_t status;

    if (isNull(l)) 
        return ERROR;
    
    if (l->release) {
        status = __listDeepRelease(l);
    } else {
        status =  __listShallowRelease(l);
    }
    return status;
}

_Status_t listAddNode(list *l, listNode *node) {
    if (isNull(l) || isNull(node))
        return ERROR;

    if (listGetNode(l) == null) {
        listSetNode(l, node);
    } else {
         
    }
}

list * listDup(list *l) {
    if (isNull(l) || isNull(l->dup)) 
        return null;

    list *l_copy = (list *)calloc(sizeof(list), 1);
    

}

listNode * listSearch(list *l, void *key) {

}

_Status_t listNodeAppend(listNode *nl, listNode *nr) {
    if (isNull(nl) || isNull(nr))
        return ERROR;
    
    listNode *tail = listNodeTail(nl);
    return listNodeCancate(tail, nr);
}

listNode * listNodePrev(listNode *node) {
    if (isNull(node) || listNodeIsFirst(node)) 
        return null;
    return node->prev; 
}

listNode * listNodeNext(listNode *node) {
    if (isNull(node) || listNodeIsLast(node))
        return null;
    return node->next;
}

listNode * listNodeHead(listNode *node) {
    if (isNull(node))
       return null;

    while (listNodeIsFirst(node)) {
        node = listNodePrev(node);     
    }
    return node;
}

listNode * listNodeTail(listNode *node) {
    if (isNull(node))
        return null;

    while (listNodeIsLast(node)) {
        node = listNodeNext(node); 
    }
    return node;
}

_Status_t listNodeCancate(listNode *nl, listNode *nr) {
    if (isNull(nl) || isNull(nr))
        return ERROR;

    nl->next = nr;
    nr->prev = nl;

    return OK;
}

/* Private functions */
private _Status_t __listShallowRelease(list *l) {
    listNode *current, *next;

    current = listGetNode(l);
    next = listNodeNext(current);

    while (isNonNull(current)) {
        // ShallowRelease 
        free(current); 

        // listNode update
        current = next;
        next = listNodeNext(next);     
    }
    return OK;
}

private _Status_t __listDeepRelease(list *l) {
    listNode *current, *next;

    current = listGetNode(l);
    next = listNodeNext(current);

    while (isNonNull(current)) {
        // DeepRelease
        if (l->release(current->value) == ERROR)
            return ERROR;
        
        // listNode update
        current = next;
        next = listNodeNext(next);
    }
    return OK;
}


