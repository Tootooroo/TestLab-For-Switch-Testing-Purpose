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
    /* Action(release, _Status_t, OK, ERROR) */
    _Status_t (*release)(void *);
    /* Predicate(match) */
    _Bool (*match)(const void *, const void *);
    /* Action(dup, void *, NON_NULL, NULL) */
    void * (*dup)(void *);
} list;

typedef enum {
    LITER_FORWARD,
    LITER_BACKWARD
} LITER_DIR;

typedef struct listIter {
    listNode *node;
    list *l;
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
list *      listCreate(void);
_Status_t   listRelease(list *l);
_Status_t   listPush(list *l, void *value);
_Status_t   listAppend(list *l, void *value);
_Status_t   listJoin(list *l, list *r);
_Status_t   listDelNode(list *l, void *key);
list *      listDup(const list *l);
listNode *  listSearch(const list *, const void *key);
listNode *  listNext(listIter *);
_Status_t   listRewind(listIter *);

listIter * listGetIter(list *l, LITER_DIR dir);
listIter * listIterInit(const list *l, listIter *iter, const LITER_DIR dir);

#ifdef _TEST_LAB_UNIT_TESTING_

void list_Basic(void **state);

#endif /* _TEST_LAB_UNIT_TESTING_ */

#endif /* ifndef _LIST_H_ */

