/* list.h */

#ifndef _LIST_H_
#define _LIST_H_

#include "type.h"

typedef struct listNode {
    void *value;
    struct listNode *prev;
    struct listNode *next;
} listNode;

typedef struct list {
    listNode *node;
    _Status_t (*release)(void *);
    _Status_t (*match)(void *, void *);
    void * (*dup)(void *);
} list;

/* Member function implemented as macros */
#define listGetNode(l) ((l)->node)
#define listSetNode(l, n) ((l)->node = n)
#define listSetReleaseMethod(l, m) ((l)->release = m)
#define listSetMatchMethod(l, m) ((l)->match = m)
#define listSetDupMethod(l, m) ((l)->dup = m)

#define listNodeGetValue(ln) ((ln)->value)
#define listNodeGetPrev(ln) ((ln)->prev)
#define listNodeSetPrev(ln, p) ((ln)->prev = p)
#define listNodeGetNext(ln) ((ln)->next)
#define listNodeSetNext(ln, n) ((ln)->next = n)
#define listNodeIsFirst(ln) ((ln)->prev == null)
#define listNodeIsLast(ln) ((ln)->next == null)

/* list Prototypes */
list * listCreate(void);
_Status_t listRelease(list *l);
_Status_t listAddNode(list *l, listNode *node);
list * listDup(list *l);
listNode * listSearch(list *, void *key);

/* listNode Prototypes */
_Status_t listNodeAppend(listNode *, listNode *);
listNode * listNodePrev(listNode *);
listNode * listNodeNext(listNode *);
listNode * listNodeHead(listNode *);
listNode * listNodeTail(listNode *);
_Status_t listNodeCancate(listNode *, listNode *);

#ifdef _TEST_LAB_UNIT_TESTING_

#endif /* _TEST_LAB_UNIT_TESTING_ */

#endif /* ifndef _LIST_H_ */

