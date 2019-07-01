/* pair.h */

#ifndef _PAIR_H_
#define _PAIR_H_

typedef struct pair {
    union {
        void *left;
        int left_i;
    };

    union {
        void *right;
        int right_i;
    };

    struct pair * (*pairDup)(struct pair);
    void (*pairRelease)(struct pair *);
    _Bool (*pairCmp)(struct pair *, struct pair *);
} pair;

/* Member functions implement as macros */
#define PAIR_SET_LEFT(P, L) ((P)->left = (L))
#define PAIR_GET_LEFT(P) ((P)->left)

#define PAIR_SET_RIGHT(P, R) ((P)->right = (R))
#define PAIR_GET_RIGHT(P) ((P)->right)

#define PAIR_SET_LEFT_I(P, L) ((P)->left_i = (L))
#define PAIR_GET_LEFT_I(P) ((P)->left_i)

#define PAIR_SET_RIGHT_I(P, R) ((P)->right_i = (R))
#define PAIR_GET_RIGHT_I(P) ((P)->right_i)

#define PAIR_SET_DUP_METHOD(P, M) ((P)->pairDup = (M))
#define PAIR_SET_RELEASE_METHOD(P, M) ((P)->pairRelease = (M))
#define PAIR_SET_CMP_METHOD(P, M) ((P)->pairCmp = M)

/* Prototypes */
pair * pairDefault(void *dupMethod, void *releaseMethod, void *cmpMethod);
pair * pairGen(void *left, void *right, void *, void *, void *);
pair * pairGen_Integer(int left, int right, void *dupMethod, void *releasemethod, void *cmpMethod);

#ifdef _TEST_LAB_UNIT_TESTING_

void pairTest(void **state);

#endif /* _TEST_LAB_UNIT_TESTING_ */


#endif /* _PAIR_H_ */
