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
    listNode *tail;
    _Status_t (*release)(void *);
    _Status_t (*match)(void *, void *);
    void * (*dup)(void *);
} list;

typedef enum {
    LITER_FORWARD,
    LITER_BACKWARD
} LITER_DIR;

typedef struct listIter {
    listNode *node;
    LITER_DIR dir;
} listIter;

/* Member function implemented as macros */
#define listGetNode(l) ((l)->node)
#define listSetNode(l, n) ((l)->node = n)
#define listGetTail(l) ((l)->tail)
#define listSetTail(l, t) ((l)->tail = t)
#define listSetReleaseMethod(l, m) ((l)->release = m)
#define listSetMatchMethod(l, m) ((l)->match = m)
#define listSetDupMethod(l, m) ((l)->dup = m)

/* list Prototypes */
list * listCreate(void);
_Status_t listRelease(list *l);
_Status_t listAddNode(list *l, listNode *node);
_Status_t listDelNode(list *l, void *key);
list * listDup(list *l);
listNode * listSearch(list *, void *key);
listNode * listNext(listIter *);
_Status_t listRewind(list *, listIter *);

#ifdef _TEST_LAB_UNIT_TESTING_

void list_Basic(void **state);

#endif /* _TEST_LAB_UNIT_TESTING_ */

#endif /* ifndef _LIST_H_ */

