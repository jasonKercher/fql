#ifndef FUNCTION_H
#define FUNCTION_H

#include "field.h"
#include "util/vec.h"

#define FUNC_NAME_MAX 100

#define OPERATOR_COUNT 10
enum expr_operator {
	OPERATOR_NONE = -1,
	OPERATOR_PLUS,
	OPERATOR_MINUS,
	OPERATOR_MULTIPY,
	OPERATOR_DIVIDE,
	OPERATOR_MODULE,
	OPERATOR_BIT_OR,
	OPERATOR_BIT_AND,
	OPERATOR_BIT_XOR,
	OPERATOR_UNARY_BIT_NOT,
	OPERATOR_UNARY_MINUS,
};

struct function;

typedef int(*scalar_f)(struct function*, union field* ret, struct vec* rec);

struct function {
	scalar_f call__;
	struct vec* args;
	char name[FUNC_NAME_MAX];
	enum expr_operator op;
	unsigned arg_min;
	unsigned arg_max;
};
typedef struct function Function;

struct function* function_new(const char*, enum field_type*);
int function_op_resolve(Function* func, enum field_type*);
struct function* function_new_op(enum expr_operator);
struct function* function_construct(struct function*, const char*, enum field_type*);
void function_free(Function* func);

int function_validate(struct function*);
void function_add_column(struct function* func, void* col);

/* Scalar function list */
int fql_left(struct function*, union field* ret, struct vec* rec);
int fql_right(struct function*, union field* ret, struct vec* rec);

/* operator functions */
int fql_op_plus_i(struct function*, union field* ret, struct vec* rec);
int fql_op_plus_f(struct function*, union field* ret, struct vec* rec);
int fql_op_plus_s(struct function*, union field* ret, struct vec* rec);
int fql_op_minus_i(struct function*, union field* ret, struct vec* rec);
int fql_op_minus_f(struct function*, union field* ret, struct vec* rec);
int fql_op_mult_i(struct function*, union field* ret, struct vec* rec);
int fql_op_mult_f(struct function*, union field* ret, struct vec* rec);
int fql_op_divi_i(struct function*, union field* ret, struct vec* rec);
int fql_op_divi_f(struct function*, union field* ret, struct vec* rec);
int fql_op_mod_i(struct function*, union field* ret, struct vec* rec);
//int fql_op_mod_f(struct function*, union field* ret, struct vec* rec);
int fql_op_bit_or(struct function*, union field* ret, struct vec* rec);
int fql_op_bit_and(struct function*, union field* ret, struct vec* rec);
int fql_op_bit_xor(struct function*, union field* ret, struct vec* rec);
int fql_op_bit_not(struct function*, union field* ret, struct vec* rec);
int fql_op_unary_minus_i(struct function*, union field* ret, struct vec* rec);
int fql_op_unary_minus_f(struct function*, union field* ret, struct vec* rec);

/* order is important here */
static scalar_f scalar_matrix[OPERATOR_COUNT][FIELD_TYPE_COUNT] = {
	{&fql_op_plus_i,        &fql_op_plus_f,        &fql_op_plus_s},
	{&fql_op_minus_i,       &fql_op_minus_f,       NULL},
	{&fql_op_mult_i,        &fql_op_mult_f,        NULL},
	{&fql_op_divi_i,        &fql_op_divi_f,        NULL},
	{&fql_op_mod_i,         NULL,                  NULL},
	{&fql_op_bit_or,        NULL,                  NULL},
	{&fql_op_bit_and,       NULL,                  NULL},
	{&fql_op_bit_xor,       NULL,                  NULL},
	{&fql_op_bit_not,       NULL,                  NULL},
	{&fql_op_unary_minus_i, &fql_op_unary_minus_f, NULL}
};


#endif  /* FUNCTION_H */
