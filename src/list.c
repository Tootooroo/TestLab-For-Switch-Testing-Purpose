/* list.c */

#include <malloc.h>

#include "type.h"
#include "wrapper.h"
#include "list.h"

/* Private prototype */
typedef void (*valueOp)(void *);
typedef _Status_t (*listNodeOp)(listNode *);
typedef _Status_t (*listNodeOpWithArgs)(listNode *, void *args);
typedef _Status_t (*OpSelector)(listNode *, listNodeOp, void *);

#define listNodeGetValue(ln) ((ln)->value)
#define listNodeGetPrev(ln) ((ln)->prev)
#define listNodeSetPrev(ln, p) ((ln)->prev = p)
#define listNodeGetNext(ln) ((ln)->next)
#define listNodeSetNext(ln, n) ((ln)->next = n)
#define listNodeIsFirst(ln) ((ln)->prev == null)
#define listNodeIsLast(ln) ((ln)->next == null)

private void valueRelease(listNode *, valueOp);
private _Status_t __listOperateWithArgs(listNode *, listNodeOp, void *);
private _Status_t __listOperateWithOutArgs(listNode *, listNodeOp, void *);
private _Status_t __listShallowRelease(list *);
private _Status_t __listDeepRelease(list *);
private listNode * __listOperate(list *, listNodeOp, void *, _Status_t);

private _Status_t listNodeAppend(listNode *, listNode *);
private _Status_t listNodeDel(listNode *);
private listNode * listNodePrev(listNode *);
private listNode * listNodeNext(listNode *);
private listNode * listNodeHead(listNode *);
private listNode * listNodeTail(listNode *);
private _Status_t listNodeCancate(listNode *, listNode *);


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

_Status_t listAddNode(list *l, void *value) {
    _Status_t status = OK;
    
    if (isNull(l) || isNull(value))
        return ERROR;
    
    listNode *node = (listNode *)zMalloc(sizeof(listNode));
    node->value = value;

    if (listGetNode(l) == null) {
        listSetNode(l, node);
        listSetTail(l, node); 
    } else {
        status = listNodeCancate(listGetTail(l), node);
        listSetTail(l, node);
    }

    return status;
}

_Status_t listDelNode(list *l, void *key) {
    if (isNull(l) || isNull(key))
        return ERROR;
    
    listNode *node = listSearch(l, key);
    if (listNodeIsFirst(node))
        l->node = listNodeNext(node);     
    
    return listNodeDel(node);
}

list * listDup(list *l) {
    if (isNull(l) || isNull(l->dup)) 
        return null;

    list *l_copy = (list *)calloc(sizeof(list), 1);
    
    listSetReleaseMethod(l_copy, l->release);
    listSetMatchMethod(l_copy, l->match);
    listSetDupMethod(l_copy, l->dup);
    
    listNode *current = listGetNode(l);

    while (isNonNull(current)) { 
        if (listAddNode(l_copy, l->dup(current->value)) == ERROR) {
            listRelease(l_copy);
            return NULL; 
        } 
        current = listNodeNext(current);
    }

    return l_copy;
}

listNode * listSearch(list *l, void *key) {
    if (isNull(l) || isNull(key))
        return null;
    
    return __listOperate(l, (listNodeOp)l->match, key, true);
}

listNode * listNext(listIter *iter) {
    if (isNull(iter))
       return null;

    listNode *node = iter->node;

    if (iter->dir == LITER_FORWARD)
        node = listNodeNext(node);
    else
        node = listNodePrev(node);

    iter->node = node;

    return iter->node;
}

_Status_t listRewind(list *l, listIter *iter) {
    if (isNull(iter))
        return ERROR;
    
    if (iter->dir == LITER_FORWARD)
        iter->node = listGetNode(l);
    else 
        iter->node = listGetTail(l);

    return OK;
}

/* Private functions */

private _Status_t listNodeAppend(listNode *nl, listNode *nr) {
    if (isNull(nl) || isNull(nr))
        return ERROR;
    
    listNode *tail = listNodeTail(nl);
    return listNodeCancate(tail, nr);
}

private _Status_t listNodeDel(listNode *n) {
    if (isNull(n))
        return ERROR; 

    /* Situations: (1) Single seperated node.
     *             (2) First node.
     *             (3) Last node.
     *             (4) Between first and last. */
    if (n->prev == null && null == n->next) {
        return OK;      
    } else if (n->prev == null) {
        n->next->prev = null;
        n->next = null;
    } else if (n->next == null) {
        n->prev->next = null;
        n->prev = null; 
    } else {
        n->prev->next = n->next;
        n->next->prev = n->prev;     
        n->prev = n->next = null;
    } 

    return OK;
}

private listNode * listNodePrev(listNode *node) {
    if (isNull(node) || listNodeIsFirst(node)) 
        return null;
    return node->prev; 
}

private listNode * listNodeNext(listNode *node) {
    if (isNull(node) || listNodeIsLast(node))
        return null;
    return node->next;
}

private listNode * listNodeHead(listNode *node) {
    if (isNull(node))
       return null;

    while (listNodeIsFirst(node)) {
        node = listNodePrev(node);     
    }
    return node;
}

private listNode * listNodeTail(listNode *node) {
    if (isNull(node))
        return null;

    while (listNodeIsLast(node)) {
        node = listNodeNext(node); 
    }
    return node;
}

private _Status_t listNodeCancate(listNode *nl, listNode *nr) {
    if (isNull(nl) || isNull(nr))
        return ERROR;

    nl->next = nr;
    nr->prev = nl;

    return OK;
}

private void valueRelease(listNode *n, valueOp op) { op(n->value); free(n); }

private _Status_t __listOperateWithOutArgs(listNode *n, listNodeOp op, void *args) { 
    return op(n->value); 
}

private _Status_t __listOperateWithArgs(listNode *n, listNodeOp op, void *args) { 
    return ((listNodeOpWithArgs)op)(n->next, args); 
}

private listNode * __listOperate(list *l, listNodeOp op, void *args, _Status_t exitCond) {
    if (isNull(l) || isNull(op))
        return NULL;
    
    OpSelector selectedOp; 
    if (args) selectedOp = __listOperateWithArgs;
    else      selectedOp = __listOperateWithOutArgs;

    listNode *current = listGetNode(l), 
             *next = listNodeNext(current); 

    while (isNonNull(current)) {
        if (selectedOp(current, op, args) == exitCond)
            break;

        current = next;
        next = listNodeNext(next); 
    } 
    return current;;
}

private _Status_t __listShallowRelease(list *l) {
    if (__listOperate(l, (listNodeOp)free, NULL, ERROR) == NULL)
        return OK;
    return ERROR;
}

private _Status_t __listDeepRelease(list *l) {
    if (__listOperate(l, (listNodeOp)valueRelease, l->release, ERROR) == NULL)
        return OK;
    return ERROR;
}

#ifdef _TEST_LAB_UNIT_TESTING_

#include "test.h"

/* Predicate(match) */
_Bool match_int(int *left, int *right) {
    return *left == *right;  
}

void list_Basic(void **state) {
    /* Test Subjects: 
     * (1) listCreate
     * (2) Insert 1000 Elements
     * (3) Delete all of them
     * (4) Iterate over all elements 
     * (5) Duplication of list */     
    
    /* (1) listCreate */
    list *l = listCreate();    
    
    /* Insert 1000 elements */
    int i = 0, *value_int, bound = 1000;
    
    int valueArray[bound];

    while (i < bound) { valueArray[i] = i; ++i; printf("%d\n", i);}

    while (i < bound) {
        listAddNode(l, (void *)(value_int + i));
        ++i;
    }
    
    listSetMatchMethod(l, match_int);

    listNode *found;
    for (i = 0; i < bound; i++) {
        found = listSearch(l, (void *)(value_int + i));
        assert_non_null(found);
        printf("%d\n", found->value);
    }
}

#endif

