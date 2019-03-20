/* queue.h */

#ifndef _TEST_LAB_QUEUE_
#define _TEST_LAB_QUEUE_

typedef _Status_t (*queueElemRelease)(void *);
typedef void * (*queueElemDup)(void *);
typedef _Status_t (*Append)(void *, void * /* item */);
typedef void * (*Retrive)(void *);

typedef struct queueMethods {
    queueElemRelease release;
    queueElemDup dup;
    Append append;
    Retrive retrive;
} queueMethods;

typedef struct queue {
    size_t numOfItems; 
    size_t maxNumOfItems;
    _Index_t head;
    _Index_t tail;
    void **items;
    queueMethods methods;
} queue;

typedef struct qIter {
    void **items; 
    _Index_t *head;
    _Index_t *tail;
    _Index_t current;
    size_t maxNumOfItems;
} qIter;

/* Member functions of queue implemented as macros */
#define queueSetNumOfItems(q, n) ((q)->numOfItems = n)
#define queueNumOfItems(q) ((q)->numOfItems)

#define queueSetMaxSize(q, s) ((q)->maxNumOfItems = s)
#define queueMaxSize(q) ((q)->maxNumOfItems)

#define queueSetHead(q, h) ((q)->head = h)
#define queueHead(q) ((q)->head)

#define queueSetTail(q, t) ((q)->tail = t)
#define queueTail(q) ((q)->tail)

#define queueSetItems(q, i) ((q)->items = i)
#define queueSetItemWithIdx(q, idx, i) ((q)->items[idx] = i)
#define queueItems(q) ((q)->items)
#define queueItemWithIdx(q, idx) ((q)->items[idx])

#define queueSetMethod(q, m) ((q)->methods = m)

/* Member functions of quIter implemented as macros */
#define qIterItems(qi) ((qi))->items)
#define qIterItemWithIdx(qi, i) ((qi)->items[i])
#define qIterHead(qi) (*((qi)->head))
#define qIterTail(qi) (*((qi)->tail))
#define qIterCurrent(qi) ((qi)->current)
#define qIterMaxSize(qi) ((qi)->maxNumOfItems)

/* Prototypes */
queue *   queueCreate(size_t sizeOfQueue, queueMethods *method);
_Status_t queueRelease(queue *q);
_Status_t queueAppend(queue *q, void *item);
void *    queueRetrive(queue *q);
queue *   queueDup(queue *orig);
qIter *   queueIter(queue *q);

#endif /* _TEST_LAB_QUEUE_ */

