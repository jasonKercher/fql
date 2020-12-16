#ifndef FUNCTION_H
#define FUNCTION_H

#include "field.h"
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

struct function;

typedef int(*scalar_f)(struct function*, union field* ret, struct vec* rec);

struct function {
        scalar_f caller;
        //struct vec* retbuf;
        struct vec* args;
        char name[FUNC_NAME_MAX];
        unsigned arg_min;
        unsigned arg_max;
};
typedef struct function Function;

struct function* function_new(const char*, enum field_type*);
struct function* function_new_op(enum expr_operator, enum field_type*);
struct function* function_init(struct function*, const char*, enum field_type*);
void function_free(Function* func);

int function_validate(struct function*);
void function_add_column(struct function* func, void* col);

/* Function list */
int fql_left(struct function*, union field* ret, struct vec* rec);

#endif  /* FUNCTION_H */
