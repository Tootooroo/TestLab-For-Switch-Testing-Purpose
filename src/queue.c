/* queue.c */

#include "type.h"
#include "queue.h"
#include <malloc.h>

/* Private prototype */
private _Index_t __prev_pos(_Index_t idx, _Index_t upper);
private _Index_t __next_pos(_Index_t idx, _Index_t upper);
private _Bool __isFull(queue *q);
private _Bool __isEmpty(queue *q);
private _Bool __isOutOfRange(_Index_t idx, _Index_t head, _Index_t tail);
private _Index_t __prevHelper(_Index_t idx, size_t upper, QI_DIRECTION dir);
private _Index_t __nextHelper(_Index_t idx, size_t upper, QI_DIRECTION dir);

/* Public functions */
queue * queueCreate(size_t sizeOfQueue, queueMethods *method) {
    queue *q = (queue *)calloc(sizeof(queue), 1);

    queueSetHead(q, 0);
    queueSetTail(q, 0);
    queueSetNumOfItems(q, 0);
    queueSetMaxSize(q, sizeOfQueue);
    queueSetItems(q, (void **)calloc(sizeof(void *), sizeOfQueue));

    if (method) queueSetMethod(q, *method);

    return q;
}

_Status_t queueRelease(queue *q) {
    if (isNull(q)) return ERROR;

    void *item;
    while ((item = queueRetrive(q)) != null) {
        if (q->methods.release(item) == ERROR)
            return ERROR;
    }

    free(queueItems(q));
    free(q);

    return OK;
}

_Status_t queueAppend(queue *q, void *item) {
    if (isNull(q) || isNull(item))
        return ERROR;

    _Index_t upper = queueMaxSize(q),
             head = queueHead(q),
             tail = queueTail(q);

    // Is full ?
    if (__isFull(q)) return ERROR;

    tail = __next_pos(tail, upper);
    queueSetItemWithIdx(q, tail, item);
    queueSetTail(q, tail);

    return OK;
}

void * queueRetrive(queue *q) {
    if (isNull(q))
        return null;

    _Index_t upper = queueMaxSize(q),
             head = queueHead(q),
             tail = queueTail(q);

    // Is empty ?
    if (__isEmpty(q)) return null;

    void *item;
    head = __next_pos(head, upper);
    item =  queueItemWithIdx(q, head);
    queueSetHead(q, head);

    return item;
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

qIter * queueIter(queue *q, QI_DIRECTION dir) {
    if (isNull(q)) return null;

    qIter *qI = (qIter *)calloc(sizeof(qIter), 1);

    qI->head = &queueHead(q);
    qI->tail = &queueTail(q);
    qI->maxNumOfItems = queueMaxSize(q);
    qI->items = queueItems(q);
    if (dir == QI_FORWARD)
        qI->current = *qI->head;
    else
        qI->current = *qI->tail + 1;
    qI->dir = dir;

    return qI;
}

void * queueIterPrev(qIter *qi) {
    if (isNull(qi)) return null;

    _Index_t current = qi->current;

    if (__isOutOfRange(current, qIterHead(qi), qIterTail(qi)))
        return NULL;

    void *item = qIterItemWithIdx(qi, current);
    current = __prevHelper(current, qIterMaxSize(qi), qIterDir(qi));
    qi->current = current;

    return item;
}

void * queueIterNext(qIter *qi) {
    if (isNull(qi)) return null;


    _Index_t current = qi->current;

    current = __nextHelper(current, qIterMaxSize(qi), qIterDir(qi));

    if (__isOutOfRange(current, qIterHead(qi), qIterTail(qi)))
        return NULL;

    void *item = qIterItemWithIdx(qi, current);
    qi->current = current;

    return item;
}

_Status_t queueIterRewind(qIter *qi) {
    if (isNull(qi)) return ERROR;

    if (qIterDir(qi) == QI_FORWARD)
        qi->current = *qi->head;
    else
        qi->current = *qi->tail + 1;

    return OK;
}

/* Private functions */
private _Index_t __prev_pos(_Index_t idx, _Index_t upper) {
    if (--idx < 0) return upper;
    return idx;
}

private _Index_t __next_pos(_Index_t idx, _Index_t upper) {
    if (++idx >= upper) return 0;
    return idx;
}

private _Bool __isFull(queue *q) {
    if (isNull(q)) return false;
    return __next_pos(queueTail(q), queueMaxSize(q)) == queueHead(q);
}

private _Bool __isEmpty(queue *q) {
    if (isNull(q)) return false;
    return queueHead(q) == queueTail(q);
}

private _Bool __isOutOfRange(_Index_t idx, _Index_t head, _Index_t tail) {
    _Bool isInRange;
    if (head <= tail) {
        isInRange = idx >= head && idx <= tail;
    } else {
        isInRange = !(idx > tail && idx < head);
    }
    return !isInRange;
}

#define FORWARD_BACKWARD_FUNC_PAIR(MOVE1, MOVE2, IDX, UPPER, DIR) ({\
    if (DIR == QI_FORWARD)\
        IDX = MOVE1(IDX, UPPER);\
    else\
        IDX =  MOVE2(IDX, UPPER);\
    IDX;\
})

private _Index_t __prevHelper(_Index_t idx, size_t upper, QI_DIRECTION dir) {
    return FORWARD_BACKWARD_FUNC_PAIR(__prev_pos, __next_pos, idx, upper, dir);
}

private _Index_t __nextHelper(_Index_t idx, size_t upper, QI_DIRECTION dir) {
    return FORWARD_BACKWARD_FUNC_PAIR(__next_pos, __prev_pos, idx, upper, dir);
}

#ifdef _TEST_LAB_UNIT_TESTING_

#include "test.h"

void queue_Basic(void **state) {
    const unsigned long length = 1000;

    int *nums = (int *)calloc(sizeof(int), length);
    assert_non_null(nums);

    queue *q = queueCreate(length, NULL);

    int idx = 0;
    while (idx < length) {
        nums[idx] = idx;
        ++idx;
    }
    // Append
    for (idx = 0; idx < length; ++idx)
        queueAppend(q, nums+idx);

    int *item;

    // Iterator testing
    idx = 0;
    qIter *iter = queueIter(q, QI_FORWARD);
    while (item = queueIterNext(iter)) {
        assert_int_equal(*item, idx);
        ++idx;
    }

    idx = length - 2;
    iter = queueIter(q, QI_BACKWARD);
    while (item = queueIterNext(iter)) {
        assert_int_equal(*item, idx);
       --idx;
    }

    // Retrive
    idx = 0;
    while (item = queueRetrive(q)) {
        assert_int_equal(*item, idx);
        ++idx;
    }
}

#endif /* _TEST_LAB_UNIT_TESTING */
