#include "function.h"
#include "fql.h"
#include "column.h"

#include "util/stringview.h"

/* ret is assumed to be of the correct type */

int fql_left(Function* fn, union field* ret, Vec* rec)
{
	Column** args = fn->args->data;
	StringView s;
	column_get_stringview(&s, args[0], rec);
	long n = 0;
	if (column_get_int(&n, args[1], rec)) {
		return FQL_FAIL;
	}

	int i = 0;
	for (; i < n && i < s.len; ++i) {
		string_push_back(ret->s, s.data[i]);
	}

	return FQL_GOOD;
}

int fql_right(Function* fn, union field* ret, Vec* rec)
{
	Column** args = fn->args->data;
	StringView s;
	column_get_stringview(&s, args[0], rec);
	long n = 0;
	if (column_get_int(&n, args[1], rec)) {
		return FQL_FAIL;
	}

	int i = s.len - n;
	for (; i < s.len; ++i) {
		string_push_back(ret->s, s.data[i]);
	}

	return FQL_GOOD;
}

/* Opertor functions */
int fql_op_plus_i(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	ret->i = n0 + n1;

	return FQL_GOOD;
}

int fql_op_plus_f(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	double n0 = 0;
	double n1 = 0;
	if (column_get_float(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_float(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	ret->f = n0 + n1;

	return FQL_GOOD;
}

int fql_op_plus_s(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	StringView s0;
	StringView s1;
	column_get_stringview(&s0, args[0], rec);
	column_get_stringview(&s1, args[1], rec);

	string_copy_from_stringview(ret->s, &s0);
	string_append_stringview(ret->s, &s1);

	return FQL_GOOD;
}

int fql_op_minus_i(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	ret->i = n0 - n1;

	return FQL_GOOD;
}

int fql_op_minus_f(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	double n0 = 0;
	double n1 = 0;
	if (column_get_float(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_float(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	ret->f = n0 - n1;

	return FQL_GOOD;
}

int fql_op_mult_i(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	ret->i = n0 * n1;

	return FQL_GOOD;
}

int fql_op_mult_f(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	double n0 = 0;
	double n1 = 0;
	if (column_get_float(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_float(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	ret->f = n0 * n1;

	return FQL_GOOD;
}

int fql_op_divi_i(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	ret->i = n0 / n1;

	return FQL_GOOD;
}

int fql_op_divi_f(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	double n0 = 0;
	double n1 = 0;
	if (column_get_float(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_float(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	ret->f = n0 / n1;

	return FQL_GOOD;
}

int fql_op_mod_i(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	ret->i = n0 % n1;

	return FQL_GOOD;
}

int fql_op_mod_f(struct function* fn, union field* ret, struct vec* rec)
{
	//Column** args = fn->args->data;
	//double n0 = 0;
	//double n1 = 0;
	//if (column_get_float(&n0, args[0], rec)) {
	//        return FQL_FAIL;
	//}
	//if (column_get_float(&n1, args[1], rec)) {
	//        return FQL_FAIL;
	//}

	//ret->f = n0 % n1;

	return FQL_GOOD;
}

int fql_op_bit_or(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	ret->i = n0 | n1;

	return FQL_GOOD;
}

int fql_op_bit_and(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	ret->i = n0 & n1;

	return FQL_GOOD;
}

int fql_op_bit_xor(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	ret->i = n0 ^ n1;

	return FQL_GOOD;
}

int fql_op_bit_not(struct function* fn, union field* ret, struct vec* rec)
{
	Column** args = fn->args->data;
	long n0 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}

	ret->i = ~n0;

	return FQL_GOOD;
}


