#include "function.h"
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include "fql.h"
#include "expression.h"
#include "misc.h"
#include "util/stringview.h"
#include "util/util.h"

/* naive utf8 handling. if I cared, I'd use libicu. */
int _get_byte_count(char c, unsigned limit)
{
	if ((c & 0x80) == 0) {
		return 1;
	}

	unsigned n = 0;
	if ((c & 0xE0) == 0xC0) {
		n = 2;
	} else if ((c & 0xF0) == 0xE0) {
		n = 3;
	} else if ((c & 0xF8) == 0xF0) {
		n = 4;
	} else {
		fputs("invalid UTF-8 sequence\n", stderr);
		return FQL_FAIL;
	}

	if (n > limit) {
		fputs("invalid UTF-8 sequence\n", stderr);
		return FQL_FAIL;
	}
	return n;
}

int _get_rev_byte_count(const char* s, unsigned limit)
{
	if ((*s & 0x80) == 0) {
		return 1;
	}

	int i = 0;
	for (; (s[i] & 0xC0) == 0x80 && -i < (int)limit; --i)
		;

	if (-i >= (int)limit) {
		fputs("invalid UTF-8 sequence\n", stderr);
		return FQL_FAIL;
	}

	/* check leading byte */
	bool valid = false;
	switch (-i + 1) {
	case 2:
		valid = ((s[i] & 0xE0) == 0xC0);
		break;
	case 3:
		valid = ((s[i] & 0xF0) == 0xE0);
		break;
	case 4:
		valid = ((s[i] & 0xF8) == 0xF0);
		break;
	}

	if (!valid) {
		fputs("invalid UTF-8 sequence\n", stderr);
		return FQL_FAIL;
	}

	return -i + 1;
}

/** ret is assumed to be of the correct type **/

int fql_len(function* fn, union field* ret, recgroup* rg)
{
	expression** arg = vec_begin(fn->args);
	stringview sv;

	try_(expression_get_stringview(&sv, *arg, rg));

	unsigned idx = 0;
	int bytes = 1;
	int len = 0;
	int len_nospace = 0;
	for (; idx < sv.len; idx += bytes) {
		bytes = 1;
		if (!fn->char_as_byte) {
			bytes = try_(_get_byte_count(sv.data[idx], sv.len - idx));
		}
		++len;
		if (!isspace(sv.data[idx])) {
			len_nospace = len;
		}
	}

	ret->i = len_nospace;

	return FQL_GOOD;
}

int fql_datalength(function* fn, union field* ret, recgroup* rg)
{
	expression** arg = vec_begin(fn->args);
	switch ((*arg)->field_type) {
	case FIELD_INT:
		ret->i = sizeof(long);
		return FQL_GOOD;
	case FIELD_FLOAT:
		ret->i = sizeof(double);
		return FQL_GOOD;
	case FIELD_STRING:
		break;
	default:
		return FQL_FAIL;
	}

	stringview sv;
	try_(expression_get_stringview(&sv, *arg, rg));

	ret->i = sv.len;

	return FQL_GOOD;
}

int fql_left(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	stringview sv;
	try_(expression_get_stringview(&sv, args[0], rg));
	long n = 0;
	try_(expression_get_int(&n, args[1], rg));

	if (n > sv.len) {
		string_strncpy(ret->s, sv.data, sv.len);
		return FQL_GOOD;
	}

	if (fn->char_as_byte) {
		string_strncpy(ret->s, sv.data, n);
		return FQL_GOOD;
	}

	unsigned idx = 0;
	int bytes = 0;
	unsigned char_count = 0;
	unsigned byte_count = 0;
	for (; char_count++ < n && idx < sv.len; idx += bytes) {
		bytes = try_(_get_byte_count(sv.data[idx], sv.len - idx));
		byte_count += bytes;
	}

	string_strncpy(ret->s, sv.data, byte_count);

	return FQL_GOOD;
}

int fql_right(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	stringview sv;
	expression_get_stringview(&sv, args[0], rg);
	long n = 0;
	try_(expression_get_int(&n, args[1], rg));

	if (n > sv.len) {
		string_strncpy(ret->s, sv.data, n);
		return FQL_GOOD;
	}

	if (fn->char_as_byte) {
		int idx = sv.len - n;
		string_strncpy(ret->s, &sv.data[idx], n);
		return FQL_GOOD;
	}

	int idx = sv.len - 1;
	unsigned char_count = 0;
	unsigned byte_count = 0;
	for (; idx >= 0 && char_count < n; ++char_count) {
		int bytes = try_(_get_rev_byte_count(&sv.data[idx], idx + 1));
		byte_count += bytes;
		idx -= bytes;
	}

	string_strncpy(ret->s, &sv.data[idx + 1], byte_count);

	return FQL_GOOD;
}

int fql_substring(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	stringview sv;
	try_(expression_get_stringview(&sv, args[0], rg));
	long start = 0;
	try_(expression_get_int(&start, args[1], rg));
	--start;
	long n = 0;
	try_(expression_get_int(&n, args[2], rg));

	unsigned idx = 0;
	int bytes = 0;
	unsigned byte_count = 0;
	unsigned char_count = 0;
	for (; char_count++ < start && idx < sv.len; idx += bytes) {
		if (fn->char_as_byte) {
			bytes = 1;
		} else {
			bytes = try_(_get_byte_count(sv.data[idx], sv.len - idx));
		}
	}

	if (start < 0) {
		n += start;
	}

	if (idx + n > sv.len) {
		string_strncpy(ret->s, &sv.data[idx], sv.len - idx);
		return FQL_GOOD;
	}

	if (fn->char_as_byte) {
		string_strncpy(ret->s, &sv.data[idx], n);
		return FQL_GOOD;
	}

	unsigned start_idx = idx;
	for (char_count = 0; char_count++ < n && idx < sv.len; idx += bytes) {
		bytes = try_(_get_byte_count(sv.data[idx], sv.len - idx));
		byte_count += bytes;
	}

	string_strncpy(ret->s, &sv.data[start_idx], byte_count);

	return FQL_GOOD;
}

/* opertor functions */
int fql_op_plus_i(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	try_(expression_get_int(&n0, args[0], rg));
	try_(expression_get_int(&n1, args[1], rg));

	ret->i = overflow_safe_add_i(n0, n1);

	return FQL_GOOD;
}

int fql_op_plus_f(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	double n0 = 0;
	double n1 = 0;
	try_(expression_get_float(&n0, args[0], rg));
	try_(expression_get_float(&n1, args[1], rg));

	ret->f = n0 + n1;

	return FQL_GOOD;
}

int fql_op_plus_s(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	stringview s0;
	stringview s1;
	expression_get_stringview(&s0, args[0], rg);
	expression_get_stringview(&s1, args[1], rg);

	string_copy_from_stringview(ret->s, &s0);
	string_append_stringview(ret->s, &s1);

	return FQL_GOOD;
}

int fql_op_minus_i(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	try_(expression_get_int(&n0, args[0], rg));
	try_(expression_get_int(&n1, args[1], rg));

	ret->i = overflow_safe_minus_i(n0, n1);

	return FQL_GOOD;
}

int fql_op_minus_f(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	double n0 = 0;
	double n1 = 0;
	try_(expression_get_float(&n0, args[0], rg));
	try_(expression_get_float(&n1, args[1], rg));

	ret->f = n0 - n1;

	return FQL_GOOD;
}

int fql_op_mult_i(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	try_(expression_get_int(&n0, args[0], rg));
	try_(expression_get_int(&n1, args[1], rg));

	ret->i = overflow_safe_multiply_i(n0, n1);

	return FQL_GOOD;
}

int fql_op_mult_f(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	double n0 = 0;
	double n1 = 0;
	try_(expression_get_float(&n0, args[0], rg));
	try_(expression_get_float(&n1, args[1], rg));

	ret->f = n0 * n1;

	return FQL_GOOD;
}

int fql_op_divi_i(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	try_(expression_get_int(&n0, args[0], rg));
	try_(expression_get_int(&n1, args[1], rg));

	if (n1 == 0) {
		fputs("division by zero\n", stderr);
		return FQL_FAIL;
	}

	ret->i = n0 / n1;

	return FQL_GOOD;
}

int fql_op_divi_f(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	double n0 = 0;
	double n1 = 0;
	try_(expression_get_float(&n0, args[0], rg));
	try_(expression_get_float(&n1, args[1], rg));

	if (n1 == 0) {
		fputs("division by zero\n", stderr);
		return FQL_FAIL;
	}

	ret->f = n0 / n1;

	return FQL_GOOD;
}

int fql_op_mod_i(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	try_(expression_get_int(&n0, args[0], rg));
	try_(expression_get_int(&n1, args[1], rg));

	ret->i = n0 % n1;

	return FQL_GOOD;
}

int fql_op_bit_or(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	try_(expression_get_int(&n0, args[0], rg));
	try_(expression_get_int(&n1, args[1], rg));

	ret->i = n0 | n1;

	return FQL_GOOD;
}

int fql_op_bit_and(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	try_(expression_get_int(&n0, args[0], rg));
	try_(expression_get_int(&n1, args[1], rg));

	ret->i = n0 & n1;

	return FQL_GOOD;
}

int fql_op_bit_xor(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	try_(expression_get_int(&n0, args[0], rg));
	try_(expression_get_int(&n1, args[1], rg));

	ret->i = n0 ^ n1;

	return FQL_GOOD;
}

int fql_op_bit_not(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	long n0 = 0;
	try_(expression_get_int(&n0, args[0], rg));

	ret->i = ~n0;

	return FQL_GOOD;
}

int fql_op_unary_minus_i(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	long n0 = 0;
	try_(expression_get_int(&n0, args[0], rg));

	if (n0 == LONG_MIN) {
		fputs("arithmetic overflow detected\n", stderr);
		return FQL_FAIL;
	}

	ret->i = -n0;

	return FQL_GOOD;
}

int fql_op_unary_minus_f(function* fn, union field* ret, recgroup* rg)
{
	expression** args = vec_begin(fn->args);
	double n0 = 0;
	try_(expression_get_float(&n0, args[0], rg));

	ret->f = -n0;

	return FQL_GOOD;
}
