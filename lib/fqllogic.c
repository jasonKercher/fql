#include "logic.h"
#include <stdio.h>
#include <ctype.h>
#include "column.h"

#include "util/stringview.h"

#define get_integers(n0_, n1_)                                                 \
	{                                                                      \
		if (column_get_int(&n0_, self->col[0], recs)) {                \
			return FQL_FAIL;                                       \
		}                                                              \
		if (column_get_int(&n1_, self->col[1], recs)) {                \
			return FQL_FAIL;                                       \
		}                                                              \
	}

#define get_floats(n0_, n1_)                                                   \
	{                                                                      \
		if (column_get_float(&n0_, self->col[0], recs)) {              \
			return FQL_FAIL;                                       \
		}                                                              \
		if (column_get_float(&n1_, self->col[1], recs)) {              \
			return FQL_FAIL;                                       \
		}                                                              \
	}

#define get_stringviews(s0_, s1_)                                              \
	{                                                                      \
		if (column_get_stringview(&s0_, self->col[0], recs)) {         \
			return FQL_FAIL;                                       \
		}                                                              \
		if (column_get_stringview(&s1_, self->col[1], recs)) {         \
			return FQL_FAIL;                                       \
		}                                                              \
	}

int fql_logic_eq_i(logic* self, vec* recs)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 == n1);
}

int fql_logic_eq_f(logic* self, vec* recs)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 == n1);
}

int fql_logic_eq_s(logic* self, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (stringview_compare_nocase_rtrim(&sv0, &sv1) == 0);
}

int fql_logic_ne_i(logic* self, vec* recs)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 != n1);
}

int fql_logic_ne_f(logic* self, vec* recs)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 != n1);
}

int fql_logic_ne_s(logic* self, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (stringview_compare_nocase_rtrim(&sv0, &sv1) != 0);
}

int fql_logic_gt_i(logic* self, vec* recs)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 > n1);
}

int fql_logic_gt_f(logic* self, vec* recs)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 > n1);
}

int fql_logic_gt_s(logic* self, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (stringview_compare_nocase_rtrim(&sv0, &sv1) > 0);
}

int fql_logic_ge_i(logic* self, vec* recs)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 >= n1);
}

int fql_logic_ge_f(logic* self, vec* recs)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 >= n1);
}

int fql_logic_ge_s(logic* self, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (stringview_compare_nocase_rtrim(&sv0, &sv1) >= 0);
}

int fql_logic_lt_i(logic* self, vec* recs)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 < n1);
}

int fql_logic_lt_f(logic* self, vec* recs)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 < n1);
}

int fql_logic_lt_s(logic* self, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (stringview_compare_nocase_rtrim(&sv0, &sv1) < 0);
}

int fql_logic_le_i(logic* self, vec* recs)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 <= n1);
}

int fql_logic_le_f(logic* self, vec* recs)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 <= n1);
}

int fql_logic_le_s(logic* self, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (stringview_compare_nocase_rtrim(&sv0, &sv1) <= 0);
}

int fql_logic_like(logic* self, vec* recs)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);

	if (self->col[1]->expr != EXPR_CONST) {
		like_to_regex(self->like_data, sv1);
	}
	int match_ret = pcre2_match(self->like_data->regex,
	                            (PCRE2_SPTR)sv0.data,
	                            sv0.len,
	                            0,
	                            0,
	                            self->like_data->match,
	                            NULL);
	if (match_ret < 0) {
		return false;
	}
	return true;
}

int fql_logic_is_null(logic* self, vec* recs)
{
	fputs("logical NULL check not yet implemented\n", stderr);
	return FQL_FAIL;
}
