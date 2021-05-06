#include "function.h"
#include <stdio.h>
#include <limits.h>
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


/* ret is assumed to be of the correct type */

int fql_left(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
	stringview s;
	column_get_stringview(&s, args[0], rec);
	long n = 0;
	if (column_get_int(&n, args[1], rec)) {
		return FQL_FAIL;
	}

	if (n > s.len) {
		string_strncpy(ret->s, s.data, s.len);
		return FQL_GOOD;
	}

	if (fn->char_as_byte) {
		string_strncpy(ret->s, s.data, n);
		return FQL_GOOD;
	}

	int i = 0;
	unsigned offset = 0;
	unsigned bytes = 0;
	for (; i < n && i+offset < s.len; ++i) {
		int b = _get_byte_count(s.data[i+offset], s.len - i);
		if (b == FQL_FAIL) {
			return FQL_FAIL;
		}
		bytes += b;
		offset += b - 1;
	}

	string_strncpy(ret->s, s.data, bytes);


	return FQL_GOOD;
}

int fql_right(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
	stringview s;
	column_get_stringview(&s, args[0], rec);
	long n = 0;
	if (column_get_int(&n, args[1], rec)) {
		return FQL_FAIL;
	}

	if (n > s.len) {
		string_strncpy(ret->s, s.data, n);
		return FQL_GOOD;
	}

	if (fn->char_as_byte) {
		int i = s.len - n;
		string_strncpy(ret->s, &s.data[i], n);
		return FQL_GOOD;
	}

	int i = s.len - 1;
	unsigned char_count = 0;
	unsigned bytes = 0;
	for (; i >= 0 && char_count < n; ++char_count) {
		int b = _get_rev_byte_count(&s.data[i], i+1);
		if (b == FQL_FAIL) {
			return FQL_FAIL;
		}
		bytes += b;
		i -= b;
	}

	string_strncpy(ret->s, &s.data[i+1], bytes);

	return FQL_GOOD;
}

/* opertor functions */
int fql_op_plus_i(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], rec)) {
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

int fql_op_plus_f(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
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

int fql_op_plus_s(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
	stringview s0;
	stringview s1;
	column_get_stringview(&s0, args[0], rec);
	column_get_stringview(&s1, args[1], rec);

	string_copy_from_stringview(ret->s, &s0);
	string_append_stringview(ret->s, &s1);

	return FQL_GOOD;
}

int fql_op_minus_i(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], rec)) {
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

int fql_op_minus_f(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
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

int fql_op_mult_i(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], rec)) {
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

int fql_op_mult_f(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
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

int fql_op_divi_i(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
	long n0 = 0;
	long n1 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_int(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	if (n1 == 0) {
		fputs("division by zero\n", stderr);
		return FQL_FAIL;
	}

	ret->i = n0 / n1;

	return FQL_GOOD;
}

int fql_op_divi_f(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
	double n0 = 0;
	double n1 = 0;
	if (column_get_float(&n0, args[0], rec)) {
		return FQL_FAIL;
	}
	if (column_get_float(&n1, args[1], rec)) {
		return FQL_FAIL;
	}

	if (n1 == 0) {
		fputs("division by zero\n", stderr);
		return FQL_FAIL;
	}

	ret->f = n0 / n1;

	return FQL_GOOD;
}

int fql_op_mod_i(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
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

int fql_op_mod_f(function* fn, union field* ret, vec* rec)
{
	//column** args = fn->args->data;
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

int fql_op_bit_or(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
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

int fql_op_bit_and(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
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

int fql_op_bit_xor(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
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

int fql_op_bit_not(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
	long n0 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}

	ret->i = ~n0;

	return FQL_GOOD;
}


int fql_op_unary_minus_i(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
	long n0 = 0;
	if (column_get_int(&n0, args[0], rec)) {
		return FQL_FAIL;
	}

	if (n0 == LONG_MIN) {
		fputs("arithmetic overflow detected\n", stderr);
		return FQL_FAIL;
	}

	ret->i = -n0;

	return FQL_GOOD;
}

int fql_op_unary_minus_f(function* fn, union field* ret, vec* rec)
{
	column** args = fn->args->data;
	double n0 = 0;
	if (column_get_float(&n0, args[0], rec)) {
		return FQL_FAIL;
	}

	ret->f = -n0;

	return FQL_GOOD;
}
