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
    if (isNull(l)) return ERROR;
    
    listIter iter;
    if (listIterInit(l, &iter, LITER_FORWARD) == NULL)
        return ERROR;

    listNode *node, *next;
    
    node = listNext(&iter);
    next = listNext(&iter);

    while (node != null) {
        if (l->release) l->release(node->value);
        free(node);

        node = next;
        next = listNext(&iter);
    }
    return OK;
}

_Status_t listAppend(list *l, void *value) {
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

_Status_t listPush(list *l, void *value) {
    if (isNull(l) || isNull(value))
        return ERROR;

    listNode *node = (listNode *)zMalloc(sizeof(listNode));
    node->value = value;
    
    listNodeCancate(node, l->node);
    l->node = node; 

    return OK;
}

_Status_t listJoin(list *l, list *r) {
    if (isNull(l) || isNull(r)) 
        return ERROR;
    
    _Status_t status;
    status = listNodeCancate(listNodeTail(l->node), r->node);
    if (status == ERROR)
        return ERROR;
    status = listRelease(r);
    if (status == ERROR)
        return ERROR;

    return OK;
}

_Status_t listDelNode(list *l, void *key) {
    if (isNull(l) || isNull(key))
        return ERROR;
    
    listNode *node = listSearch(l, key);
    if (listNodeIsFirst(node))
        l->node = listNodeNext(node);     
    
    return listNodeDel(node);
}

list * listDup(const list *l) {
    if (isNull(l) || isNull(l->dup)) 
        return null;

    list *l_copy = (list *)calloc(sizeof(list), 1);
    
    listSetReleaseMethod(l_copy, l->release);
    listSetMatchMethod(l_copy, l->match);
    listSetDupMethod(l_copy, l->dup);
    
    listNode *current = listGetNode(l);

    while (isNonNull(current)) { 
        if (listAppend(l_copy, l->dup(current->value)) == ERROR) {
            listRelease(l_copy);
            return NULL; 
        } 
        current = listNodeNext(current);
    }

    return l_copy;
}

listNode * listSearch(const list *l, const void *key) {
    if (isNull(l) || isNull(key))
        return null;
    
    listIter iter;
    listNode *node;

    if (listIterInit(l, &iter, LITER_FORWARD) == null)
        return null;
   
    while (node = listNext(&iter)) {
        if (l->match(node->value, key))
            break;
    }
    return node;
}

listIter *listGetIter(list *l, LITER_DIR dir) {
    listIter *iter = (listIter *)zMalloc(sizeof(listIter));
    if (listIterInit(l, iter, dir) == null)
        free(iter);

    return iter;
}

listIter *listIterInit(const list *l, listIter *iter, const LITER_DIR dir) {
    if (isNull(l) || isNull(iter)) return NULL;
    if (dir != LITER_FORWARD && dir != LITER_BACKWARD) 
        return NULL;

    iter->dir = dir;
    iter->node = l->node;
    iter->l = (list *)l;

    return iter;
}

_Status_t listIterRelease(listIter *iter) {
    if (isNull(iter)) return ERROR;
    free(iter);
}

listNode * listNext(listIter *iter) {
    if (isNull(iter))
       return null;
    
    listNode *current = iter->node;
    listNode *node = current;

    if (iter->dir == LITER_FORWARD)
        node = listNodeNext(node);
    else
        node = listNodePrev(node);

    iter->node = node;

    return current;
}

_Status_t listRewind(listIter *iter) {
    if (isNull(iter))
        return ERROR;
    
    if (iter->dir == LITER_FORWARD)
        iter->node = listGetNode(iter->l);
    else 
        iter->node = listGetTail(iter->l);

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

#ifdef _TEST_LAB_UNIT_TESTING_

#include "test.h"

/* Predicate(match) */
_Bool match_int(int *left, int *right) {
    return *left == *right;
}

int * int_dup(int *number) {
    if (isNull(number)) return NULL;

    int *copy = (int *)zMalloc(sizeof(int));

    *copy = *number;

    return copy;
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

    while (i < bound) { valueArray[i] = i; ++i; }
    
    i = 0;
    while (i < bound) {
        listAppend(l, (void *)(valueArray + i));
        ++i;
    }
    
    listSetMatchMethod(l, match_int);
    
    assert_non_null(listSearch(l, (void *)(valueArray + 256)));

    listNode *found;
    for (i = 0; i < bound; i++) {
        found = listSearch(l, (void *)(valueArray + i));
        assert_non_null(found);
    }

    /* Iterate overAll elements */
    listNode *node;
    listIter *iter = listGetIter(l, LITER_FORWARD); 
    
    i = 0;
    while (node = listNext(iter)) {
        value_int = node->value;
        assert_int_equal(i++, *value_int);
    }
    
    listIterRelease(iter);

    /* Duplication */
    list *y;
     
    listSetDupMethod(l, int_dup); 

    y = listDup(l);
    
    iter = listGetIter(y, LITER_FORWARD);
    i = 0;
    while (node = listNext(iter)) {
        value_int = node->value;
        assert_int_equal(i++, *value_int);
    }
    
    /* Release list */
    listRelease(l);
    
    /* list joing testing */ 
    int numbers[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    list *r = listCreate();
    l = listCreate(); 

    i = 0;
    while (i < 5) {
        listAppend(l, numbers + i); 
        ++i;
    }
    
    i = 5;
    while (i < 10) {
        listAppend(l, numbers + i);
        ++i;
    }
    
    listSetMatchMethod(l, match_int);

    listJoin(l, r);
    i = 0;
    while (i < 10) {
        node = listSearch(l, numbers+i); 
        value_int = node->value;
        assert_int_equal(i, *value_int);
        ++i;
    }

}

#endif

