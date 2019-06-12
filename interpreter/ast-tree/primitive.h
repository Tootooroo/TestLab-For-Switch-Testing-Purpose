/* primitive.h */

#ifndef _AST_TREE_PRIMITIVE_H_
#define _AST_TREE_PRIMITIVE_H_

#include "type.h"
#include "list.h"

typedef enum {
    PRIMITIVE_TYPE_INT,
    PRIMITIVE_TYPE_STR,
    PRIMITIVE_TYPE_OPS
} primitiveType;

typedef struct Ops {
    /* list of pair that represent
     * operation and their parameters
     * list<opName :: String, parameter :: String> */
    list *ops;
} Ops;

typedef struct Primitive {
    primitiveType type;

    union {
        int val_i;
        char *val_str;
        Ops ops;
    };
} Primitive;

/* Member function implement as macros */
#define isPrimitive_int(p) ((p)->type == PRIMITIVE_TYPE_INT)
#define isPrimitive_str(p) ((p)->type == PRIMITIVE_TYPE_STR)
#define isPrimitive_ops(p) ((p)->type == PRIMITIVE_TYPE_OPS)

/* Prototypes */
Primitive * primitiveGenerate_i(int i);
Primitive * primitiveGenerate_s(char *s);
Primitive * primitiveGenerate_op(Ops op);

#endif /* _AST_TREE_PRIMITIVE_H_ */
