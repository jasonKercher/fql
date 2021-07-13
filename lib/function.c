#include "function.h"

#include "fql.h"
#include "misc.h"
#include "query.h"
#include "expression.h"
#include "aggregate.h"
#include "fqlselect.h"
#include "util/util.h"

/* order is important here */
static scalar_fn _scalar_ops[OPERATOR_COUNT][FIELD_TYPE_COUNT] = {
        {&fql_op_plus_i, &fql_op_plus_f, &fql_op_plus_s},
        {&fql_op_minus_i, &fql_op_minus_f, NULL},
        {&fql_op_mult_i, &fql_op_mult_f, NULL},
        {&fql_op_divi_i, &fql_op_divi_f, NULL},
        {&fql_op_mod_i, NULL, NULL},
        {&fql_op_bit_or, NULL, NULL},
        {&fql_op_bit_and, NULL, NULL},
        {&fql_op_bit_xor, NULL, NULL},
        {&fql_op_bit_not, NULL, NULL},
        {&fql_op_unary_minus_i, &fql_op_unary_minus_f, NULL}};

static const char* _scalar_str[] = {
        "PLUS",      "MINUS",      "MULTIPY",  "DIVIDE",        "MODULE",
        "BIT_OR",    "BIT_AND",    "BIT_XOR",  "UNARY_BIT_NOT", "UNARY_MINUS",
        "ABS",       "ASCII",      "CEILING",  "CHAR",          "CHARINDEX",
        "CHECKSUM",  "DATALENGTH", "DAY",      "FLOOR",         "ISDATE",
        "ISNUMERIC", "LEFT",       "LEN",      "LOWER",         "LTRIM",
        "MONTH",     "NCHAR",      "PATINDEX", "RAND",          "REPLACE",
        "RIGHT",     "ROUND",      "RTRIM",    "SIGN",          "SPACE",
        "STR",       "SUBSTRING",  "UPPER",    "USER_NAME",     "YEAR",
};

int _not_implemented(function* fn, union field* f, recgroup* unused)
{
	fprintf(stderr,
	        "function not implemented: %s\n",
	        _scalar_str[fn->type]);
	return 0;
}

function* function_construct(function* func,
                             enum scalar_function scalar_type,
                             enum field_type* type,
                             int char_as_byte)
{
	*func = (function) {
	        &_not_implemented,        /* call__ */
	        new_t_(vec, expression*), /* args */
	        scalar_type,              /* type */
	        0,                        /* arg_min */
	        0,                        /* arg_max */
	        char_as_byte              /* char_as_byte */
	};

	/* if this is an operator, we don't have enough info */
	if (scalar_type <= SCALAR_OP_UNARY_MINUS) {
		return func;
	}

	//func->arg_min = 0;
	//func->arg_max = 0;
	//CURRENT_TIMESTAMP
	//GETDATE
	//GETUTCDATE
	//CURRENT_USER
	//SYSTEM_USER
	//SESSION_USER

	func->arg_min = 0;
	func->arg_max = 1;
	switch (scalar_type) {
	case SCALAR_RAND:
		return func;
	case SCALAR_USER_NAME:
		return func;
	default:;
	}

	func->arg_min = 1;
	func->arg_max = 1;
	switch (scalar_type) {
	case SCALAR_ABS:
		return func;
	case SCALAR_ASCII:
		return func;
	case SCALAR_CEILING:
		return func;
	case SCALAR_CHAR:
		return func;
	case SCALAR_DATALENGTH:
		func->call__ = &fql_datalength;
		*type = FIELD_INT;
		return func;
	case SCALAR_DAY:
		return func;
	case SCALAR_FLOOR:
		return func;
	case SCALAR_ISDATE:
		return func;
	case SCALAR_ISNUMERIC:
		return func;
	case SCALAR_LEN:
		func->call__ = &fql_len;
		*type = FIELD_INT;
		return func;
	case SCALAR_LOWER:
		return func;
	case SCALAR_LTRIM:
		return func;
	case SCALAR_MONTH:
		return func;
	case SCALAR_NCHAR:
		return func;
	case SCALAR_RTRIM:
		return func;
	//case SCALAR_SESSIONPROPERTY: return func;
	case SCALAR_SPACE:
		return func;
	//case SCALAR_TRY_CAST: return func;
	case SCALAR_UPPER:
		return func;
	case SCALAR_SIGN:
		return func;
	case SCALAR_YEAR:
		return func;
	default:;
	}

	func->arg_min = 1;
	func->arg_max = 3;
	switch (scalar_type) {
	case SCALAR_STR:
		return func;
	default:;
	}

	func->arg_min = 2;
	func->arg_max = 2;
	switch (scalar_type) {
	//case SCALAR_DATENAME: return func;
	//case SCALAR_DATEPART: return func;
	//case SCALAR_ISNULL: return func;
	case SCALAR_LEFT:
		func->call__ = &fql_left;
		*type = FIELD_STRING;
		return func;
	//case SCALAR_NULLIF: return func;
	case SCALAR_PATINDEX:
		return func;
	case SCALAR_RIGHT:
		func->call__ = &fql_right;
		*type = FIELD_STRING;
		return func;
	default:;
	}

	func->arg_min = 2;
	func->arg_max = 3;
	switch (scalar_type) {
	//case SCALAR_CONVERT: return func;
	case SCALAR_ROUND:
		return func;
	//case SCALAR_TRY_CONVERT: return func;
	default:;
	}

	func->arg_min = 3;
	func->arg_max = 3;
	switch (scalar_type) {
	case SCALAR_CHARINDEX:
		return func;
	//case SCALAR_DATEADD: return func;
	//case SCALAR_DATEDIFF: return func;
	case SCALAR_REPLACE:
		return func;
	case SCALAR_SUBSTRING:
		func->call__ = &fql_substring;
		*type = FIELD_STRING;
		return func;
	default:;
	}

	//func->arg_min = 4;
	//func->arg_max = 4;
	//STUFF

	//func->arg_min = 2;
	//func->arg_max = 10000;
	//COALESCE
	//CONCAT

	return func;
}

void function_destroy(function* func)
{
	expression** it = vec_begin(func->args);
	for (; it != vec_end(func->args); ++it) {
		delete_(expression, *it);
	}
	delete_(vec, func->args);
}

int function_op_resolve(function* func, enum field_type* type)
{
	if (func->type > SCALAR_OP_UNARY_MINUS) {
		return FQL_GOOD;
	}

	func->arg_min = 2;
	func->arg_max = 2;

	expression** args = func->args->data;
	expression* expr0 = args[0];
	expression* expr1 = expr0;
	if (func->args->size == 2) {
		expr1 = args[1];
	}

	if (expr0->expr == EXPR_AGGREGATE) {
		aggregate_resolve(expr0->field.agg, expr0);
	}
	if (expr1->expr == EXPR_AGGREGATE) {
		aggregate_resolve(expr1->field.agg, expr1);
	}

	*type = field_determine_type(expr0->field_type, expr1->field_type);

	switch (func->type) {
	case SCALAR_OP_UNARY_BIT_NOT:
		func->arg_min = 1;
		func->arg_max = 1;
		break;
	case SCALAR_OP_UNARY_MINUS:
		func->arg_min = 1;
		func->arg_max = 1;
		break;
	default:;
	}

	func->call__ = _scalar_ops[func->type][*type];
	if (func->call__ == NULL) {
		fprintf(stderr,
		        "invalid type for %s operation\n",
		        _scalar_str[func->type]);
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

const char* function_get_name(function* func)
{
	return _scalar_str[func->type];
}

int function_validate(function* func)
{
	unsigned argc = func->args->size;
	if (argc >= func->arg_min && argc <= func->arg_max) {
		return FQL_GOOD;
	}
	if (func->arg_min == func->arg_max) {
		fprintf(stderr,
		        "function `%s' expected %d argument(s)... found %d\n",
		        _scalar_str[func->type],
		        func->arg_min,
		        argc);
	} else {
		fprintf(stderr,
		        "function `%s' expected between %d and %d arguments... "
		        "found %d\n",
		        _scalar_str[func->type],
		        func->arg_min,
		        func->arg_max,
		        argc);
	}
	return FQL_FAIL;
}

void function_add_expression(function* func, void* expr)
{
	vec_push_back(func->args, &expr);
}
