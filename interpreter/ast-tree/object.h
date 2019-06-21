/* object.h */

#ifndef _AST_TREE_OBJECT_H_
#define _AST_TREE_OBJECT_H_

#include "hashMap.h"
#include "primitive.h"

typedef enum { MEMBER_PRIMITIVE, MEMBER_OBJECT } MemberType;

typedef struct Object {
    /* Type of object */
    char *objectType;
    /* identifier of an entity of a objectType */
    char *identifier;
    /* hashMap<String, Member> */
    hashMap *members;
} Object;

typedef struct Member {
    MemberType type;
    union {
        Primitive *p;
        Object *o;
    };
} Member;

/* Member function implement as macros */
#define

#endif /* _AST_TREE_OBJECT_H_ */
