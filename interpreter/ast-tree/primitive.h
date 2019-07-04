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
        Ops *ops;
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
Primitive * primitiveDefault(void);
Primitive * primitiveGen(void *, primitiveType);
Primitive * primitiveDup(Primitive *);
_Status_t primitiveRelease(Primitive *);
_Bool primitiveIsEqual(Primitive *, Primitive *);
_Status_t primitiveAssign(Primitive *, Primitive *);

/* Ops procedures
 *
 * Ops should be regular so Primitive can also be regular */
Ops * opsDefault(void);
_Status_t opsNewMember(Ops *op, char *member);
_Status_t opsRmMember(Ops *op, char *member);
void opsRelease(Ops *);
Ops * opsDup(Ops *);
_Bool opsIsEqual(Ops *, Ops *);
_Status_t opsAssign(Ops *, Ops *);
_Bool opsIsSupportOp(Ops *ops, char *opName);
_Bool isPrimitiveType(char *);

#define OPS_SET_OP_SET(OP, SET) ((OP)->ops = (SET))

#ifdef _AST_TREE_TESTING_

void primitiveTest(void **state);

#endif /* AST_TREE_TESTING_ */

#endif /* _AST_TREE_PRIMITIVE_H_ */
