/* variable.h */

#ifndef _AST_TREE_VARIABLE_H_
#define _AST_TREE_VARIABLE_H_

#include "object.h"
#include "primitive.h"
#include "scope.h"

typedef struct Array Array;

typedef enum {
    VAR_EMPTY = -1,
    VAR_PRIMITIVE_INT = 0,
    VAR_PRIMITIVE_STR,
    VAR_PRIMITIVE_OPS,
    VAR_OBJECT,
    VAR_ARRAY,
    VAR_OCCUPY,
    VAR_TYPE_NUM
} VarType;

typedef enum {
    VAR_OP_PLUS,
    VAR_OP_MINUS,
    VAR_OP_MUL,
    VAR_OP_DIV,
    VAR_OP_EQUAL,
    VAR_OP_LESS_THAN,
    VAR_OP_GREATER_THAN,
    VAR_OP_LESS_OR_EQUAL,
    VAR_OP_GREATER_OR_EQUAL,
    VAR_OP_NOT_EQUAL,
    VAR_OP_ASSIGN,
    VAR_OP_DOT,
    VAR_OP_NUM
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
        Array *array;
    };

    // Operators
    struct VarOps *ops;
    struct VarInnerOps *iOps;
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
    Variable * (*assign)(Variable *, Variable *);
    Variable * (*dot)(Variable *, Variable *);
} VarOps;

typedef struct VarInnerOps {
    /* Copy variable itself into a scope
     * this action is depend on
     * variable type, primitive type will
     * copy completly into a scope which
     * means its call by value, and object
     * is copy the Variable part only, wh-
     * ich means its call by reference. */
    Variable * (*pass)(Variable *, Scope *, char *ident);
    Variable * (*dup)(Variable *);
    void     * (*valueDup)(Variable *);
} VarInnerOps;

typedef void (*VarInitRtn)(Variable *v, char *ident, void *value);

// This variable is used to occupy space in scope.
extern Variable emptyVar;

/* Member function implement as macros */
#define VAR_INNER_OP(V, OP, args...) ((V)->iOps->OP(args))

#define VAR_IS_OCCUPY(V) ((V)->type == VAR_OCCUPY)
#define VAR_IS_LVAL(V) ((V)->identifier != null)

#define VAR_SET_IDENT(V, I) ((V)->identifier = (I))
#define VAR_IDENT(V) ((V)->identifier)

#define VAR_SET_TYPE(V, T) ((V)->type = (T))
#define VAR_TYPE(V) ((V)->type)

#define VAR_SET_PRIMITIVE(V, P) ({\
    if (isPrimitive_int( (P) )) (V)->type = VAR_PRIMITIVE_INT;  \
    else if (isPrimitive_str( (P) )) (V)->type = VAR_PRIMITIVE_STR;\
    (V)->p = (P);\
})

#define VAR_SET_OBJECT(V, O) ({ (V)->type = VAR_OBJECT; (V)->o = (O); })
#define VAR_IS_PRIMITIVE(V) ((V)->type == VAR_PRIMITIVE_INT || \
                             (V)->type == VAR_PRIMITIVE_STR)
#define VAR_IS_OBJECT(V)    ((V)->type == VAR_OBJECT)

#define VAR_SET_RELEASE_METHOD(V, M) ((V)->release = (M))
#define VAR_SET_OP(V, OP_SET) ((V)->ops = OP_SET)
#define VAR_BIN_OP_CALL(V, OP, AR1, AR2) ((V)->ops->OP(AR1, AR2))

#define VAR_GET_PRIMITIVE_INT(V) ((V)->p->val_i)
#define VAR_GET_PRIMITIVE_STR(V) ((V)->p->val_str)

#define VAR_GET_ARRAY(V) ((V)->array)

/* Prototypes */
Variable varDefault_Empty();
Variable * varDefault();
Variable * varGen(char *ident, VarType type, void *value);
void varRelease(Variable *);
Variable * varDup(Variable *);
_Status_t varAssign_(Variable *l, Variable *r);
_Bool varOpsPreCheck(Variable *l, Variable *r, VarOp ops, VarType type);
void setVarInitRtn(VarType t, VarInitRtn r);

// Misc
_Bool varIdentCmp(Variable *, char *);
_Bool varTypeCmp(Variable *, Variable *);
_Bool varIsType(Variable *, char *);
_Bool varIsTrue(Variable *);
VarType varTypeStr2Int(char *);
char * varType2Str(Variable *);
VarType varTypeIs(Variable *v);

#ifdef _AST_TREE_TESTING_

void varTest(void **state);

#endif /* _AST_TREE_TESTING_ */

#endif /* _AST_TREE_VARIABLE_H_ */
