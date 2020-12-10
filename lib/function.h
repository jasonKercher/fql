#ifndef FUNCTION_H
#define FUNCTION_H

#include "column.h"
#include "util/vec.h"

#define FUNC_NAME_MAX 100

enum expr_operator {
        OPERATOR_PLUS,
        OPERATOR_MINUS,
        OPERATOR_MULTIPY,
        OPERATOR_DIVIDE,
        OPERATOR_MODULE,
        OPERATOR_BIT_OR,
        OPERATOR_BIT_AND,
        OPERATOR_BIT_XOR,
        OPERATOR_BIT_NOT,
};

typedef int(*scalar_f)(void* data, void* ret, struct vec* args);

struct function {
        scalar_f caller;
        struct vec* args;
        char name[FUNC_NAME_MAX];
        unsigned arg_min;
        unsigned arg_max;
};
typedef struct function Function;

struct function* function_new(const char*);
struct function* function_new_op(enum expr_operator);
struct function* function_init(struct function*, const char*);
void function_free(Function* func);

int function_validate(struct function*);
void function_add_column(struct function* func, struct column* col);

#endif  /* FUNCTION_H */
