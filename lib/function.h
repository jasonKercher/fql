#ifndef FUNCTION_H
#define FUNCTION_H

#include "field.h"
#include "query.h"
#include "record.h"

struct vec;

#define FUNC_NAME_MAX 100

#define OPERATOR_COUNT 10
struct function;

typedef int (*scalar_fn)(struct function*, union field* ret, struct node*);

struct function {
	scalar_fn call__;
	struct vec* args;
	enum scalar_function type;
	unsigned arg_min;
	unsigned arg_max;
	bool char_as_byte;
};
typedef struct function function;

struct function* function_construct(struct function*,
                                    enum scalar_function,
                                    enum field_type*,
                                    int);
void function_destroy(struct function*);

const char* function_get_name(struct function*);
int function_validate(struct function*, struct expression* parent);
void function_add_expression(struct function* func, void* col);
int function_op_resolve(function* func, enum field_type*);

/* cast function list */
int fql_cast_int(struct function*, union field* ret, struct node*);
int fql_cast_bit(struct function*, union field* ret, struct node*);
int fql_cast_float(struct function*, union field* ret, struct node*);
int fql_cast_string(struct function*, union field* ret, struct node*);
int fql_cast_char(struct function*, union field* ret, struct node*);

/* scalar functions list */
int fql_len(struct function*, union field* ret, struct node*);
int fql_datalength(struct function*, union field* ret, struct node*);
int fql_left(struct function*, union field* ret, struct node*);
int fql_right(struct function*, union field* ret, struct node*);
int fql_substring(struct function*, union field* ret, struct node*);

int fql_isnull_i(struct function*, union field* ret, struct node*);
int fql_isnull_f(struct function*, union field* ret, struct node*);
int fql_isnull_s(struct function*, union field* ret, struct node*);

/* scalar operators */
int fql_op_plus_i(struct function*, union field* ret, struct node*);
int fql_op_plus_f(struct function*, union field* ret, struct node*);
int fql_op_plus_s(struct function*, union field* ret, struct node*);
int fql_op_minus_i(struct function*, union field* ret, struct node*);
int fql_op_minus_f(struct function*, union field* ret, struct node*);
int fql_op_mult_i(struct function*, union field* ret, struct node*);
int fql_op_mult_f(struct function*, union field* ret, struct node*);
int fql_op_divi_i(struct function*, union field* ret, struct node*);
int fql_op_divi_f(struct function*, union field* ret, struct node*);
int fql_op_mod_i(struct function*, union field* ret, struct node*);
//int fql_op_mod_f(struct function*, union field* ret, struct node*);
int fql_op_bit_or(struct function*, union field* ret, struct node*);
int fql_op_bit_and(struct function*, union field* ret, struct node*);
int fql_op_bit_xor(struct function*, union field* ret, struct node*);
int fql_op_bit_not(struct function*, union field* ret, struct node*);
int fql_op_unary_minus_i(struct function*, union field* ret, struct node*);
int fql_op_unary_minus_f(struct function*, union field* ret, struct node*);

#endif /* FUNCTION_H */
