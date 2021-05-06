#include "logic.h"
#include <stdio.h>
#include <ctype.h>
#include "column.h"

#include "util/stringview.h"

#define get_integers(n0_, n1_) {                                \
	if (column_get_int(&n0_, logic->col[0], recs)) {        \
		return FQL_FAIL;                                \
	}                                                       \
	if (column_get_int(&n1_, logic->col[1], recs)) {        \
		return FQL_FAIL;                                \
	}                                                       \
}

#define get_floats(n0_, n1_) {                                  \
	if (column_get_float(&n0_, logic->col[0], recs)) {      \
		return FQL_FAIL;                                \
	}                                                       \
	if (column_get_float(&n1_, logic->col[1], recs)) {      \
		return FQL_FAIL;                                \
	}                                                       \
}

#define get_stringviews(s0_, s1_) {                             \
	if (column_get_stringview(&s0_, logic->col[0], recs)) { \
		return FQL_FAIL;                                \
	}                                                       \
	if (column_get_stringview(&s1_, logic->col[1], recs)) { \
		return FQL_FAIL;                                \
	}                                                       \
}


/* Essentially strcasecmp but relying on length instead of '\0'
 *
 * This version does not consider trailing white space.
 * This is a true statement in tsql:
 *
 * 'hello' = 'Hello   '
 */
int _fql_stringview_compare(stringview* sv0, stringview* sv1)
{
	stringview* short_sv = sv0;
	stringview* long_sv = sv1;
	if (sv0->len > sv1->len) {
		short_sv = sv1;
		long_sv = sv0;
	}

	const unsigned char *s0 = (unsigned char*) sv0->data;
	const unsigned char *s1 = (unsigned char*) sv1->data;

	int ret = 0;

	/* I'm going to neglect '\0'. What could go wrong? */
	int i = 0;
	for (; ret == 0 && i < short_sv->len; ++i) {
		ret = tolower (s0[i]) - tolower (s1[i]);
	}

	/* All remaining bytes must be white space to stay truthy */
	const unsigned char *long_str = (unsigned char*) long_sv->data;

	for (; ret == 0 && i < long_sv->len; ++i) {
		ret = (isspace(long_str[i])) ? 0 : sv0->len - sv1->len;
	}
	return ret;
}


int fql_logic_eq_i(logic* logic, vec* recs)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 == n1);
}

int fql_logic_eq_f(logic* logic, vec* recs)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 == n1);
}

int fql_logic_eq_s(logic* logic, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (_fql_stringview_compare(&sv0, &sv1) == 0);
}

int fql_logic_ne_i(logic* logic, vec* recs)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 != n1);
}

int fql_logic_ne_f(logic* logic, vec* recs)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 != n1);
}

int fql_logic_ne_s(logic* logic, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (_fql_stringview_compare(&sv0, &sv1) != 0);
}

int fql_logic_gt_i(logic* logic, vec* recs)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 > n1);
}

int fql_logic_gt_f(logic* logic, vec* recs)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 > n1);
}

int fql_logic_gt_s(logic* logic, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (_fql_stringview_compare(&sv0, &sv1) > 0);
}

int fql_logic_ge_i(logic* logic, vec* recs)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 >= n1);
}

int fql_logic_ge_f(logic* logic, vec* recs)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 >= n1);
}

int fql_logic_ge_s(logic* logic, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (_fql_stringview_compare(&sv0, &sv1) >= 0);
}

int fql_logic_lt_i(logic* logic, vec* recs)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 < n1);
}

int fql_logic_lt_f(logic* logic, vec* recs)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 < n1);
}

int fql_logic_lt_s(logic* logic, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (_fql_stringview_compare(&sv0, &sv1) < 0);
}

int fql_logic_le_i(logic* logic, vec* recs)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 <= n1);
}

int fql_logic_le_f(logic* logic, vec* recs)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 <= n1);
}

int fql_logic_le_s(logic* logic, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (_fql_stringview_compare(&sv0, &sv1) <= 0);
}

int fql_logic_like(logic* logic, vec* recs)
{
	fputs("logical like not yet implemented\n", stderr);
	return FQL_FAIL;
}

int fql_logic_not_like(logic* logic, vec* recs)
{
	fputs("logical like not yet implemented\n", stderr);
	return FQL_FAIL;
}

int fql_logic_is_null(logic* logic, vec* recs)
{
	fputs("logical NULL check not yet implemented\n", stderr);
	return FQL_FAIL;
}

int fql_logic_not_null(logic* logic, vec* recs)
{
	fputs("logical NULL check not yet implemented\n", stderr);
	return FQL_FAIL;
}


