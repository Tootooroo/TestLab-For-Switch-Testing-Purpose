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

/* Ops is a base type to represent operations.
 * A Machine can involve a specific Ops to in-
 * dicate that it support operations that rep-
 * resent by the Ops. */
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

#define getPrimitive_int(p) ((p)->val_i)
#define getPrimitive_str(p) ((p)->val_str)
#define getPrimitive_ops(p) ((p)->ops)

/* Prototypes */
Primitive * primitiveGenerate_i(int i);
Primitive * primitiveGenerate_s(char *s);
Primitive * primitiveGenerate_op(Ops op);
_Status_t primitiveRelease(Primitive *);

// Operators
Primitive * primitivePlusOp(Primitive *l, Primitive *r);
Primitive * primitiveMinusOp(Primitive *l, Primitive *r);
Primitive * primitiveMulOp(Primitive *l, Primitive *r);
Primitive * primitiveDivOp(Primitive *l, Primitive *r);
Primitive * primitiveLessThanOp(Primitive *l, Primitive *r);
Primitive * primitiveGreaterThanOp(Primitive *l, Primitive *r);
Primitive * primitiveLessOrEqualOp(Primitive *l, Primitive *r);
Primitive * primitiveGreaterOrEqualOp(Primitive *l, Primitive *r);
Primitive * primitiveNotEqualOp(Primitive *l, Primitive *r);

#endif /* _AST_TREE_PRIMITIVE_H_ */
