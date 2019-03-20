/* queue.c */

#include "type.h"
#include "queue.h"
#include <malloc.h>

/* Private prototype */
_Index_t __prev_pos(_Index_t idx, _Index_t upper);
_Index_t __next_pos(_Index_t idx, _Index_t upper);
_Bool __isFull(queue *q);
_Bool __isEmpty(queue *q);
_Bool __isOutOfRange(_Index_t idx, _Index_t head, _Index_t tail);


/* Public functions */
queue * queueCreate(size_t sizeOfQueue, queueMethods *method) {
    queue *q = (queue *)calloc(sizeof(queue), 1);
    
    queueSetHead(q, 0);
    queueSetTail(q, 0);
    queueSetNumOfItems(q, 0); 
    queueSetMaxSize(q, sizeOfQueue);
    queueSetItems(q, (void **)calloc(sizeof(void *), sizeOfQueue));
    queueSetMethod(q, *method);
    
    return q;
}

_Status_t queueRelease(queue *q) {
    if (isNull(q)) return error;
    
    void *item;
    while ((item = queueRetrive(q)) != null) {
        if (q->methods.release(item) == error)
            return error;
    } 
   
    free(queueItems(q));
    free(q);

    return ok;
}

_Status_t queueAppend(queue *q, void *item) {
    if (isNull(q) || isNull(item))
        return error; 
    
    _Index_t upper = queueMaxSize(q),
             head = queueHead(q),
             tail = queueTail(q);

    // Is full ?
    if (__isFull(q)) return error;
    
    queueSetItemWithIdx(q, tail, item); 
}

void * queueRetrive(queue *q) {
    if (isNull(q))
        return null;
    
    _Index_t upper = queueMaxSize(q),
             tail = queueHead(q),
             head = queueTail(q);

    // Is empty ?
    if (__isEmpty(q)) return null;
    
    return queueItemWithIdx(q, head);
}

queue * queueDup(queue *orig) {
    if (isNull(orig)) return null;
    
    queue *copy = queueCreate(orig->maxNumOfItems, &orig->methods);
    
    _Index_t upper = queueMaxSize(orig),
             head = queueHead(orig),
             tail = queueTail(orig);
    
    queueSetMaxSize(copy, upper);
    queueSetHead(copy, head);
    queueSetTail(copy, tail);
    queueSetNumOfItems(copy, queueNumOfItems(orig));
    
    void *beCopyed, *current_copy;
    _Index_t current_idx = head;
    while (current_idx = __next_pos(current_idx, upper)) {
        beCopyed = queueItems(orig)[current_idx];
        current_copy = orig->methods.dup(beCopyed); 
        queueSetItemWithIdx(copy, current_idx, current_copy);

        if (current_idx == tail) 
            break; 
    }

    return copy;
}

qIter * queueIter(queue *q) {
    if (isNull(q)) return null;

    qIter *qI = (qIter *)calloc(sizeof(qIter), 1);
    
    qI->head = &queueHead(q);
    qI->tail = &queueTail(q);
    qI->maxNumOfItems = queueMaxSize(q);
    qI->items = queueItems(q);
    qI->current = *qI->head;

    return qI;
}

void * queueIterPrev(qIter *qi) {
    if (isNull(qi)) return null;
    
    _Index_t current = __prev_pos(qi->current, qi->maxNumOfItems);
    if (__isOutOfRange(current, *qi->head, *qi->tail)) 
        current = qIterTail(qi);

    return qIterItemWithIdx(qi, current);
}

void * queueIterNext(qIter *qi) {
    if (isNull(qi)) return null;
    
    _Index_t current = __next_pos(qIterCurrent(qi), qIterMaxSize(qi));
    if (__isOutOfRange(current, qIterHead(qi), qIterTail(qi)))
        current = qIterHead(qi);

    return qIterItemWithIdx(qi, current);
}

/* Private functions */
_Index_t __prev_pos(_Index_t idx, _Index_t upper) {
    if (--idx < 0) return upper; 
    return idx;
}

_Index_t __next_pos(_Index_t idx, _Index_t upper) {
    if (++idx > upper) return 0;      
    return idx;
}

_Bool __isFull(queue *q) {
    if (isNull(q)) return false;
    return __next_pos(queueTail(q), queueMaxSize(q)) == queueHead(q);  
}

_Bool __isEmpty(queue *q) {
    if (isNull(q)) return false;
    return queueHead(q) == queueTail(q);
}

_Bool __isOutOfRange(_Index_t idx, _Index_t head, _Index_t tail) {
    _Bool isInRange;    
    if (head <= tail) {
        isInRange = idx >= head && idx <= tail; 
    } else {
        isInRange = !(idx > tail && idx < head); 
    }  
    return isInRange;
}

#ifdef _TEST_LAB_UNIT_TESTING

#endif /* _TEST_LAB_UNIT_TESTING */

