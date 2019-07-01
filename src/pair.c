/* pair.c */

#include "pair.h"
#include "wrapper.h"

/* Public procedures */
pair * pairDefault(void *dupMethod, void *releaseMethod, void *cmpMethod) {
    pair *p = (pair *)zMalloc(sizeof(pair));

    PAIR_SET_DUP_METHOD(p, dupMethod);
    PAIR_SET_RELEASE_METHOD(p, releaseMethod);
    PAIR_SET_CMP_METHOD(p, cmpMethod);

    return p;
}

pair * pairGen(void *left, void *right, void *dupMethod, void *releaseMethod, void *cmpMethod) {
    pair *p = pairDefault(dupMethod, releaseMethod, cmpMethod);

    PAIR_SET_LEFT(p, left);
    PAIR_SET_RIGHT(p, right);

    return p;
}

pair * pairGen_Integer(int left, int right, void *dupMethod, void *releaseMethod, void *cmpMethod) {
    pair *p = pairDefault(dupMethod, releaseMethod, cmpMethod);

    PAIR_SET_LEFT_I(p, left);
    PAIR_SET_RIGHT_I(p, right);

    return p;
}

#ifdef _TEST_LAB_UNIT_TESTING_

#include "test.h"

pair * tpairDup(pair *p) {
    pair *dup = (pair *)zMalloc(sizeof(pair));

    PAIR_SET_DUP_METHOD(dup, p->pairDup);
    PAIR_SET_RELEASE_METHOD(dup, p->pairRelease);
    PAIR_SET_CMP_METHOD(dup, p->pairCmp);

    PAIR_SET_LEFT(dup, p->left);
    PAIR_SET_RIGHT(dup, p->right);

    return dup;
}

void tpairRelease(pair *p) {
    free(p->left);
    free(p->right);
}

_Bool tpairCmp(pair *left, pair *right) {
    _Bool isLeftEqual = strCompare((char *)left->left, (char *)right->left);
    _Bool isRightEqual = strCompare((char *)left->right, (char *)right->right);

    return isLeftEqual && isRightEqual;
}

void pairTest(void **state) {
    pair *pair1 = pairDefault(tpairDup, tpairRelease, tpairCmp);

    PAIR_SET_LEFT_I(pair1, 1);
    PAIR_SET_RIGHT_I(pair1, 2);

    assert_int_equal(PAIR_GET_LEFT_I(pair1), 1);
    assert_int_equal(PAIR_GET_RIGHT_I(pair1), 2);

    char *str1 = "Hello", *str2 = "World";

    pair *pair2 = pairGen(strdup(str1), strdup(str2),
                          tpairDup, tpairRelease, tpairCmp);

    PAIR_SET_LEFT(pair1, strdup(PAIR_GET_LEFT(pair2)));
    PAIR_SET_RIGHT(pair1, strdup(PAIR_GET_RIGHT(pair2)));

    assert_int_equal(pair1->pairCmp(pair1, pair2), true);

    pair1->pairRelease(pair1);
    pair2->pairRelease(pair2);
}

#endif /* _TEST_LAB_UNIT_TESTING_ */
