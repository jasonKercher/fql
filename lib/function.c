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
        "PLUS",      "MINUS",   "MULTIPY",       "DIVIDE",      "MODULE",     "BIT_OR",
        "BIT_AND",   "BIT_XOR", "UNARY_BIT_NOT", "UNARY_MINUS", "ABS",        "ASCII",
        "CEILING",   "CHAR",    "CHARINDEX",     "CHECKSUM",    "DATALENGTH", "DAY",
        "FLOOR",     "ISDATE",  "ISNUMERIC",     "LEFT",        "LEN",        "LOWER",
        "LTRIM",     "MONTH",   "NCHAR",         "PATINDEX",    "RAND",       "REPLACE",
        "RIGHT",     "ROUND",   "RTRIM",         "SIGN",        "SPACE",      "STR",
        "SUBSTRING", "UPPER",   "USER_NAME",     "YEAR",
};

int _not_implemented(function* self, union field* f, recgroup* unused)
{
	fprintf(stderr, "function not implemented: %s\n", _scalar_str[self->type]);
	return 0;
}

function* function_construct(function* self,
                             enum scalar_function scalar_type,
                             enum field_type* type,
                             int char_as_byte)
{
	*self = (function) {
	        &_not_implemented,        /* call__ */
	        new_t_(vec, expression*), /* args */
	        scalar_type,              /* type */
	        0,                        /* arg_min */
	        0,                        /* arg_max */
	        char_as_byte              /* char_as_byte */
	};

	/* if this is an operator, we don't have enough info */
	if (scalar_type <= SCALAR_OP_UNARY_MINUS) {
		return self;
	}

	//self->arg_min = 0;
	//self->arg_max = 0;
	//CURRENT_TIMESTAMP
	//GETDATE
	//GETUTCDATE
	//CURRENT_USER
	//SYSTEM_USER
	//SESSION_USER

	self->arg_min = 0;
	self->arg_max = 1;
	switch (scalar_type) {
	case SCALAR_RAND:
	case SCALAR_USER_NAME:
		return self;
	default:;
	}

	self->arg_min = 1;
	self->arg_max = 1;
	switch (scalar_type) {
	case SCALAR_DATALENGTH:
		self->call__ = &fql_datalength;
		*type = FIELD_INT;
		return self;
	case SCALAR_LEN:
		self->call__ = &fql_len;
		*type = FIELD_INT;
		return self;
	case SCALAR_ABS:
	case SCALAR_ASCII:
	case SCALAR_CEILING:
	case SCALAR_CHAR:
	case SCALAR_DAY:
	case SCALAR_FLOOR:
	case SCALAR_ISDATE:
	case SCALAR_ISNUMERIC:
	case SCALAR_LOWER:
	case SCALAR_LTRIM:
	case SCALAR_MONTH:
	case SCALAR_NCHAR:
	case SCALAR_RTRIM:
	//case SCALAR_SESSIONPROPERTY:
	case SCALAR_SPACE:
	//case SCALAR_TRY_CAST:
	case SCALAR_UPPER:
	case SCALAR_SIGN:
	case SCALAR_YEAR:
		return self;
	default:;
	}

	self->arg_min = 1;
	self->arg_max = 3;
	switch (scalar_type) {
	case SCALAR_STR:
		return self;
	default:;
	}

	self->arg_min = 2;
	self->arg_max = 2;
	switch (scalar_type) {
	case SCALAR_LEFT:
		self->call__ = &fql_left;
		*type = FIELD_STRING;
		return self;
	case SCALAR_RIGHT:
		self->call__ = &fql_right;
		*type = FIELD_STRING;
		return self;
	//case SCALAR_DATENAME:
	//case SCALAR_DATEPART:
	//case SCALAR_ISNULL:
	//case SCALAR_NULLIF:
	case SCALAR_PATINDEX:
		return self;
	default:;
	}

	self->arg_min = 2;
	self->arg_max = 3;
	switch (scalar_type) {
	//case SCALAR_CONVERT:
	case SCALAR_ROUND:
		return self;
	//case SCALAR_TRY_CONVERT:
	default:;
	}

	self->arg_min = 3;
	self->arg_max = 3;
	switch (scalar_type) {
	case SCALAR_SUBSTRING:
		self->call__ = &fql_substring;
		*type = FIELD_STRING;
		return self;
	case SCALAR_CHARINDEX:
	//case SCALAR_DATEADD:
	//case SCALAR_DATEDIFF:
	case SCALAR_REPLACE:
		return self;
	default:;
	}

	//self->arg_min = 4;
	//self->arg_max = 4;
	//STUFF

	//self->arg_min = 2;
	//self->arg_max = 10000;
	//COALESCE
	//CONCAT

	return self;
}

void function_destroy(function* self)
{
	expression** it = vec_begin(self->args);
	for (; it != vec_end(self->args); ++it) {
		delete_(expression, *it);
	}
	delete_(vec, self->args);
}

int function_op_resolve(function* self, enum field_type* type)
{
	if (self->type > SCALAR_OP_UNARY_MINUS) {
		return FQL_GOOD;
	}

	self->arg_min = 2;
	self->arg_max = 2;

	expression** args = self->args->data;
	expression* expr0 = args[0];
	expression* expr1 = expr0;
	if (self->args->size == 2) {
		expr1 = args[1];
	}

	if (expr0->expr == EXPR_AGGREGATE) {
		aggregate_resolve(expr0->field.agg, expr0);
	}
	if (expr1->expr == EXPR_AGGREGATE) {
		aggregate_resolve(expr1->field.agg, expr1);
	}

	*type = field_determine_type(expr0->field_type, expr1->field_type);

	switch (self->type) {
	case SCALAR_OP_UNARY_BIT_NOT:
		self->arg_min = 1;
		self->arg_max = 1;
		break;
	case SCALAR_OP_UNARY_MINUS:
		self->arg_min = 1;
		self->arg_max = 1;
		break;
	default:;
	}

	self->call__ = _scalar_ops[self->type][*type];
	if (self->call__ == NULL) {
		fprintf(stderr,
		        "invalid type for %s operation\n",
		        _scalar_str[self->type]);
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

const char* function_get_name(function* self)
{
	return _scalar_str[self->type];
}

int function_validate(function* self)
{
	unsigned argc = self->args->size;
	if (argc >= self->arg_min && argc <= self->arg_max) {
		return FQL_GOOD;
	}
	if (self->arg_min == self->arg_max) {
		fprintf(stderr,
		        "function `%s' expected %d argument(s)... found %d\n",
		        _scalar_str[self->type],
		        self->arg_min,
		        argc);
	} else {
		fprintf(stderr,
		        "function `%s' expected between %d and %d arguments... "
		        "found %d\n",
		        _scalar_str[self->type],
		        self->arg_min,
		        self->arg_max,
		        argc);
	}
	return FQL_FAIL;
}

void function_add_expression(function* self, void* expr)
{
	vec_push_back(self->args, &expr);
}
