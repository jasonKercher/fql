#ifndef FUNCTION_H
#define FUNCTION_H

#include "field.h"
#include "query.h"
#include "util/vec.h"

#define FUNC_NAME_MAX 100

#define OPERATOR_COUNT 10
struct function;

typedef int(*scalar_fn)(struct function*, union field* ret, struct vec* rec);

struct function {
	scalar_fn call__;
	struct vec* args;
	enum scalar_function type;
	unsigned arg_min;
	unsigned arg_max;
	_Bool char_as_byte;
};
typedef struct function function;

struct function* function_construct(struct function*, enum scalar_function, enum field_type*, int);
void function_destroy(struct function*);

const char* function_get_name(struct function*);
int function_validate(struct function*);
void function_add_column(struct function* func, void* col);
int function_op_resolve(function* func, enum field_type*);

/* scalar functions list */
int fql_left(struct function*, union field* ret, struct vec* rec);
int fql_right(struct function*, union field* ret, struct vec* rec);

/* scalar operators */
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
static scalar_fn scalar_ops[OPERATOR_COUNT][FIELD_TYPE_COUNT] = {
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
