/* pair.h */

#ifndef _PAIR_H_
#define _PAIR_H_

typedef struct Pair {
    union {
        void *left;
        int left_i;
    };

    union {
        void *right;
        int right_i;
    };

    struct Pair * (*pairDup)(struct Pair);
    void (*pairRelease)(struct Pair *);
} Pair;

/* Member functions implement as macros */
#define PAIR_SET_LEFT(P, L) ((P)->left = (L))
#define PAIR_SET_RIGHT(P, R) ((P)->right = (R))

#define PAIR_SET_LEFT_I(P, L) ((P)->left_i = (L))
#define PAIR_SET_RIGHT_I(P, R) ((P)->right_i = (R))

#define PAIR_SET_DUP_METHOD(P, M) ((P)->pairDup = (M))
#define PAIR_SET_RELEASE_METHOD(P, M) ((P)->pairRelease = (M))

/* Prototypes */
Pair * pairDefault();
Pair * pairGen(void *left, void *right, void *, void *);
Pair * pairGen_Integer(int left, int right);


#ifdef _TEST_LAB_UNIT_TESTING_
void pairTest(void **state);
#endif /* _TEST_LAB_UNIT_TESTING_ */


#endif /* _PAIR_H_ */
