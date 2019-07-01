/* variable.h */

#ifndef _AST_TREE_VARIABLE_H_
#define _AST_TREE_VARIABLE_H_

#include "object.h"
#include "primitive.h"
#include "scope.h"

typedef enum {
    VAR_EMPTY,
    VAR_PRIMITIVE_INT,
    VAR_PRIMITIVE_STR,
    VAR_OBJECT
} VarType;

typedef enum {
    VAR_OP_PLUS,
    VAR_OP_MINUS,
    VAR_OP_MUL,
    VAR_OP_LESS_THAN,
    VAR_OP_GREATER_THAN,
    VAR_OP_LESS_OR_EQUAL,
    VAR_OP_GREATER_OR_EQUAL,
    VAR_OP_NOT_EQUAL
} VarOp;

struct VarOps;
typedef struct Variable {
    /* ident of variable, this field
     * may will be null if its a te-
     * mporary variable. */
    char *identifier;

    // Type of variable
    VarType type;

    union {
        Primitive *p;
        Object *o;
    };

    // Operators
    struct VarOps *ops;

    void (*release)(struct Variable *, Scope *);
} Variable;

typedef struct VarOps {
    Variable * (*plus)(Variable *, Variable *);
    Variable * (*minus)(Variable *, Variable *);
    Variable * (*mul)(Variable *, Variable *);
    Variable * (*div)(Variable *, Variable *);
    Variable * (*equal)(Variable *, Variable *);
    Variable * (*lessThan)(Variable *, Variable *);
    Variable * (*greaterThan)(Variable *, Variable *);
    Variable * (*lessOrEqual)(Variable *, Variable *);
    Variable * (*greaterOrEqual)(Variable *, Variable *);
    Variable * (*notEqual)(Variable *, Variable *);
} VarOps;

/* Member function implement as macros */
#define VAR_SET_PRIMITIVE(V, P) ({ (V)->type = VAR_PRIMITIVE; (V)->p = (P) })
#define VAR_SET_OBJECT(V, O) ({ (V)->type = VAR_OBJECT; (V)->o = (O); })
#define VAR_IS_PRIMITIVE(V) ((V)->type == VAR_PRIMITIVE)
#define VAR_IS_OBJECT(V)    ((V)->type == VAR_OBJECT)

#define VAR_SET_RELEASE_METHOD(V, M) ((V)->release = (M))
#define VAR_SET_OP(V, OP_SET) ((V)->ops = OP_SET)
#define VAR_BIN_OP_CALL(V, OP, AR1, AR2) ((V)->ops->OP(AR1, AR2))

/* Prototypes */
Variable varDefault_Empty();
Variable * varDefault();
Variable * varGen(char *ident, VarType type, void *value);

#endif /* _AST_TREE_VARIABLE_H_ */
