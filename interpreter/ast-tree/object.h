/* object.h */

#ifndef _AST_TREE_OBJECT_H_
#define _AST_TREE_OBJECT_H_

#include "hashMap.h"
#include "primitive.h"

typedef struct Variable Variable;

typedef enum { MEMBER_PRIMITIVE, MEMBER_OBJECT } MemberType;

typedef struct Object {
    /* Type of object */
    char *objectType;
    /* identifier of an entity of a objectType */
    char *identifier;
    /* list<Variable> */
    list *members;
} Object;

typedef struct Member {
    MemberType type;
    union {
        Primitive *p;
        Object *o;
    };
} Member;

/* Member function implement as macros */
#define OBJ_IDENTIFIER(O) ((O)->identifier)
#define OBJ_SET_IDENTIFIER(O, N) ((O)->identifier = (N))

#define OBJ_TYPE(O) ((O)->identifier)
#define OBJ_SET_TYPE(O, T) ((O)->objectType = (T))

#define OBJ_MEMBERS(O) ((O)->members)
#define OBJ_SET_MEMBERS(O, M) ((O)->members = (M))

/* Prototypes */
Object * objDefualt();
Object * objGen(char *identifier, char *type);
Object * objDup(Object *);
Variable * objGetMember(Object *, char *);
void objectRelease(Object *);

#endif /* _AST_TREE_OBJECT_H_ */
