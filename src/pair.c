/* pair.c */

#include "pair.h"
#include "wrapper.h"

/* Public procedures */
Pair * pairDefault(void *dupMethod, void *releaseMethod, void *cmpMethod) {
    Pair *p = (Pair *)zMalloc(sizeof(Pair));

    PAIR_SET_DUP_METHOD(p, dupMethod);
    PAIR_SET_RELEASE_METHOD(p, releaseMethod);
    PAIR_SET_CMP_METHOD(p, cmpMethod);

    return p;
}

Pair * pairGen(void *left, void *right, void *dupMethod, void *releaseMethod, void *cmpMethod) {
    Pair *p = pairDefault(dupMethod, releaseMethod, cmpMethod);

    PAIR_SET_LEFT(p, left);
    PAIR_SET_RIGHT(p, right);

    return p;
}

Pair * pairGen_Integer(int left, int right, void *dupMethod, void *releaseMethod, void *cmpMethod) {
    Pair *p = pairDefault(dupMethod, releaseMethod, cmpMethod);

    PAIR_SET_LEFT_I(p, left);
    PAIR_SET_RIGHT_I(p, right);

    return p;
}

#ifdef _TEST_LAB_UNIT_TESTING_

#include "test.h"

Pair * tPairDup(Pair *p) {
    Pair *dup = (Pair *)zMalloc(szieof(Pair));

    PAIR_SET_DUP_METHOD(dup, p->pairDup);
    PAIR_SET_RELEASE_METHOD(dup, p->pairRelease);
    PAIR_SET_CMP_METHOD(dup, p->pairCmp);

    PAIR_SET_LEFT(dup, p->left);
    PAIR_SET_RIGHT(dup, p->right);

    return dup;
}

void tPairRelease(Pair *p) {
    free(p->left);
    free(p->right);
}

_Bool tPairCmp(Pair *left, Pair *right) {
    char *left_str1, *left_str2,
        *right_str1, *right_str2;

    _Bool isLeftEqual = strCompare((char *)left->left, (char *)right->left);
    _Bool isRightEqual = strCompare((char *)left->right, (char *)right->right);

    return isLeftEqual && isRightEqual;
}

void pairTest(void **state) {
    Pair *pair = pairDefault(tPairDup, tPairRelease, tPairCmp);

    PAIR_SET_LEFT_I(pair, 1);
    PAIR_SET_LEFT_I(pair, 2);

    assert_int_equal(PAIR_GET_LEFT_I(pair), 1);
    assert_int_equal(PAIR_GET_RIGHT_I(pair), 2);

    char *str1 = "Hello", *str2 = "World";

    Pair *pair_ = pairGen(strdup(str1), strdup(str2), tPairDup, tPairRelease, tPairCmp);

    PAIR_SET_LEFT(pair, strdup(PAIR_GET_LEFT(pair_)));
    PAIR_SET_RIGHT(pair, strdup(PAIR_GET_RIGHT(pair_)));

    assert_int_equal(pair->pairCmp(pair, pair_), true);

    pair->pairRelease(pair);
    pair_->pairRelease(pair_);
}

#endif /* _TEST_LAB_UNIT_TESTING_ */
