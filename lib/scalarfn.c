#include "function.h"
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include "fql.h"
#include "column.h"

#include "util/stringview.h"


/* naive utf8 handling. if I cared, I'd use libicu. */
int _get_byte_count(char c, unsigned limit)
{
	if ((c & 0x80) == 0) {
		return 1;
	}

	int n = 0;
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
	for (; (s[i] & 0xC0) == 0x80 && -i < limit; --i);

	if (-i >= limit) {
		fputs("invalid UTF-8 sequence\n", stderr);
		return FQL_FAIL;
	}

	/* check leading byte */
	_Bool valid = false;
	switch (-i+1) {
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

	return -i+1;
}


/** ret is assumed to be of the correct type **/

int fql_len(function* fn, union field* ret, vec* recs)
{
	column** arg = vec_begin(fn->args);
	stringview sv;

	if (column_get_stringview(&sv, *arg, recs)) {
		return FQL_FAIL;
	}

	int i = 0;
	int bytes = 1;
	int len = 0;
	int len_nospace = 0;
	for (; i < sv.len; i += bytes) {
		bytes = 1;
		if (!fn->char_as_byte) {
			bytes = _get_byte_count(sv.data[i], sv.len - i);
			if (bytes == FQL_FAIL) {
				return FQL_FAIL;
			}
		}
		++len;
		if (!isspace(sv.data[i])) {
			len_nospace = len;
		}
	}

	ret->i = len_nospace;

	return FQL_GOOD;
}

int fql_datalength(function* fn, union field* ret, vec* recs)
{
	column** arg = vec_begin(fn->args);
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
	if (column_get_stringview(&sv, *arg, recs)) {
		return FQL_FAIL;
	}

	ret->i = sv.len;

	return FQL_GOOD;
}


int fql_left(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	stringview sv;
	if (column_get_stringview(&sv, args[0], recs)) {
		return FQL_FAIL;
	}
	long n = 0;
	if (column_get_int(&n, args[1], recs)) {
		return FQL_FAIL;
	}

	if (n > sv.len) {
		string_strncpy(ret->s, sv.data, sv.len);
		return FQL_GOOD;
	}

	if (fn->char_as_byte) {
		string_strncpy(ret->s, sv.data, n);
		return FQL_GOOD;
	}

	int i = 0;
	int bytes = 0;
	unsigned byte_count = 0;
	for (; i < n && i < sv.len; i += bytes) {
		bytes = _get_byte_count(sv.data[i], sv.len - i);
		if (bytes == FQL_FAIL) {
			return FQL_FAIL;
		}
		byte_count += bytes;
	}

	string_strncpy(ret->s, sv.data, byte_count);


	return FQL_GOOD;
}

int fql_right(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	stringview sv;
	column_get_stringview(&sv, args[0], recs);
	long n = 0;
	if (column_get_int(&n, args[1], recs)) {
		return FQL_FAIL;
	}

	if (n > sv.len) {
		string_strncpy(ret->s, sv.data, n);
		return FQL_GOOD;
	}

	if (fn->char_as_byte) {
		int i = sv.len - n;
		string_strncpy(ret->s, &sv.data[i], n);
		return FQL_GOOD;
	}

	int i = sv.len - 1;
	unsigned char_count = 0;
	unsigned byte_count = 0;
	for (; i >= 0 && char_count < n; ++char_count) {
		int bytes = _get_rev_byte_count(&sv.data[i], i+1);
		if (bytes == FQL_FAIL) {
			return FQL_FAIL;
		}
		byte_count += bytes;
		i -= bytes;
	}

	string_strncpy(ret->s, &sv.data[i+1], byte_count);

	return FQL_GOOD;
}

/* opertor functions */
int fql_op_plus_i(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], recs)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], recs)) {
		return FQL_FAIL;
	}

	/* detect overflow */
	if (n0 > 0 && n1 > 0
	 && (unsigned long) n0 + (unsigned long) n1 > (unsigned long) LONG_MAX) {
		fputs("arithmetic overflow detected\n", stderr);
		return FQL_FAIL;
	} else if (n0 < 0 && n1 < 0
	 && (unsigned long) -n0 + (unsigned long) -n1 > (unsigned long) LONG_MAX + 1) {
		fputs("arithmetic overflow detected\n", stderr);
		return FQL_FAIL;
	}

	ret->i = n0 + n1;

	return FQL_GOOD;
}

int fql_op_plus_f(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	double n0 = 0;
	double n1 = 0;
	if (column_get_float(&n0, args[0], recs)) {
		return FQL_FAIL;
	}
	if (column_get_float(&n1, args[1], recs)) {
		return FQL_FAIL;
	}

	ret->f = n0 + n1;

	return FQL_GOOD;
}

int fql_op_plus_s(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	stringview s0;
	stringview s1;
	column_get_stringview(&s0, args[0], recs);
	column_get_stringview(&s1, args[1], recs);

	string_copy_from_stringview(ret->s, &s0);
	string_append_stringview(ret->s, &s1);

	return FQL_GOOD;
}

int fql_op_minus_i(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], recs)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], recs)) {
		return FQL_FAIL;
	}

	/* detect overflow */
	if (n0 > 0 && n1 < 0
	 && (unsigned long) n0 + (unsigned long) -n1 > (unsigned long) LONG_MAX) {
		fputs("arithmetic overflow detected\n", stderr);
		return FQL_FAIL;
	} else if (n0 < 0 && n1 > 0
	 && (unsigned long) -n0 + (unsigned long) n1 > (unsigned long) LONG_MAX + 1) {
		fputs("arithmetic overflow detected\n", stderr);
		return FQL_FAIL;
	}

	ret->i = n0 - n1;

	return FQL_GOOD;
}

int fql_op_minus_f(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	double n0 = 0;
	double n1 = 0;
	if (column_get_float(&n0, args[0], recs)) {
		return FQL_FAIL;
	}
	if (column_get_float(&n1, args[1], recs)) {
		return FQL_FAIL;
	}

	ret->f = n0 - n1;

	return FQL_GOOD;
}

int fql_op_mult_i(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], recs)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], recs)) {
		return FQL_FAIL;
	}

	unsigned long u0 = 0;
	unsigned long u1 = 0;
	_Bool neg = false;
	/* use unsigned so not undefined behavior */
	if (n0 < 0) {
		u0 = n0 * -1;
		neg = true;
	} else {
		u0 = n0;
	}

	if (n1 < 0) {
		u1 = n1 * -1;
		neg = !neg;
	} else {
		u1 = n1;
	}
	unsigned long result = u0 * u1;

	if (u0 != 0
	 && (result / u0 != u1
	     || result > LONG_MAX + neg)) {
		fputs("arithmetic overflow detected\n", stderr);
		return FQL_FAIL;
	}

	ret->i = (neg) ? -1 * result : result;

	return FQL_GOOD;
}

int fql_op_mult_f(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	double n0 = 0;
	double n1 = 0;
	if (column_get_float(&n0, args[0], recs)) {
		return FQL_FAIL;
	}
	if (column_get_float(&n1, args[1], recs)) {
		return FQL_FAIL;
	}

	ret->f = n0 * n1;

	return FQL_GOOD;
}

int fql_op_divi_i(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], recs)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], recs)) {
		return FQL_FAIL;
	}

	if (n1 == 0) {
		fputs("division by zero\n", stderr);
		return FQL_FAIL;
	}

	ret->i = n0 / n1;

	return FQL_GOOD;
}

int fql_op_divi_f(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	double n0 = 0;
	double n1 = 0;
	if (column_get_float(&n0, args[0], recs)) {
		return FQL_FAIL;
	}
	if (column_get_float(&n1, args[1], recs)) {
		return FQL_FAIL;
	}

	if (n1 == 0) {
		fputs("division by zero\n", stderr);
		return FQL_FAIL;
	}

	ret->f = n0 / n1;

	return FQL_GOOD;
}

int fql_op_mod_i(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], recs)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], recs)) {
		return FQL_FAIL;
	}

	ret->i = n0 % n1;

	return FQL_GOOD;
}

int fql_op_mod_f(function* fn, union field* ret, vec* recs)
{
	//column** args = vec_begin(fn->args);
	//double n0 = 0;
	//double n1 = 0;
	//if (column_get_float(&n0, args[0], recs)) {
	//        return FQL_FAIL;
	//}
	//if (column_get_float(&n1, args[1], recs)) {
	//        return FQL_FAIL;
	//}

	//ret->f = n0 % n1;

	return FQL_GOOD;
}

int fql_op_bit_or(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], recs)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], recs)) {
		return FQL_FAIL;
	}

	ret->i = n0 | n1;

	return FQL_GOOD;
}

int fql_op_bit_and(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], recs)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], recs)) {
		return FQL_FAIL;
	}

	ret->i = n0 & n1;

	return FQL_GOOD;
}

int fql_op_bit_xor(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], recs)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], recs)) {
		return FQL_FAIL;
	}

	ret->i = n0 ^ n1;

	return FQL_GOOD;
}

int fql_op_bit_not(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	long n0 = 0;
	if (column_get_int(&n0, args[0], recs)) {
		return FQL_FAIL;
	}

	ret->i = ~n0;

	return FQL_GOOD;
}


int fql_op_unary_minus_i(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	long n0 = 0;
	if (column_get_int(&n0, args[0], recs)) {
		return FQL_FAIL;
	}

	if (n0 == LONG_MIN) {
		fputs("arithmetic overflow detected\n", stderr);
		return FQL_FAIL;
	}

	ret->i = -n0;

	return FQL_GOOD;
}

int fql_op_unary_minus_f(function* fn, union field* ret, vec* recs)
{
	column** args = vec_begin(fn->args);
	double n0 = 0;
	if (column_get_float(&n0, args[0], recs)) {
		return FQL_FAIL;
	}

	ret->f = -n0;

	return FQL_GOOD;
}
