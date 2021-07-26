#include "logic.h"
#include <stdio.h>
#include <ctype.h>
#include "expression.h"
#include "misc.h"
#include "util/stringview.h"

#define get_integers(n0_, n1_)                                     \
	{                                                          \
		if (expression_get_int(&n0_, self->expr[0], rg)) { \
			return FQL_FAIL;                           \
		}                                                  \
		if (expression_get_int(&n1_, self->expr[1], rg)) { \
			return FQL_FAIL;                           \
		}                                                  \
	}

#define get_floats(n0_, n1_)                                         \
	{                                                            \
		if (expression_get_float(&n0_, self->expr[0], rg)) { \
			return FQL_FAIL;                             \
		}                                                    \
		if (expression_get_float(&n1_, self->expr[1], rg)) { \
			return FQL_FAIL;                             \
		}                                                    \
	}

#define get_stringviews(s0_, s1_)                                         \
	{                                                                 \
		if (expression_get_stringview(&s0_, self->expr[0], rg)) { \
			return FQL_FAIL;                                  \
		}                                                         \
		if (expression_get_stringview(&s1_, self->expr[1], rg)) { \
			return FQL_FAIL;                                  \
		}                                                         \
	}

int fql_logic_eq_i(logic* self, node* rg)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 == n1);
}

int fql_logic_eq_f(logic* self, node* rg)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 == n1);
}

int fql_logic_eq_s(logic* self, node* rg)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (stringview_compare_nocase_rtrim(&sv0, &sv1) == 0);
}

int fql_logic_ne_i(logic* self, node* rg)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 != n1);
}

int fql_logic_ne_f(logic* self, node* rg)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 != n1);
}

int fql_logic_ne_s(logic* self, node* rg)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (stringview_compare_nocase_rtrim(&sv0, &sv1) != 0);
}

int fql_logic_gt_i(logic* self, node* rg)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 > n1);
}

int fql_logic_gt_f(logic* self, node* rg)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 > n1);
}

int fql_logic_gt_s(logic* self, node* rg)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (stringview_compare_nocase_rtrim(&sv0, &sv1) > 0);
}

int fql_logic_ge_i(logic* self, node* rg)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 >= n1);
}

int fql_logic_ge_f(logic* self, node* rg)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 >= n1);
}

int fql_logic_ge_s(logic* self, node* rg)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (stringview_compare_nocase_rtrim(&sv0, &sv1) >= 0);
}

int fql_logic_lt_i(logic* self, node* rg)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 < n1);
}

int fql_logic_lt_f(logic* self, node* rg)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 < n1);
}

int fql_logic_lt_s(logic* self, node* rg)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (stringview_compare_nocase_rtrim(&sv0, &sv1) < 0);
}

int fql_logic_le_i(logic* self, node* rg)
{
	long n0 = 0;
	long n1 = 0;
	get_integers(n0, n1);
	return (n0 <= n1);
}

int fql_logic_le_f(logic* self, node* rg)
{
	double n0 = 0;
	double n1 = 0;
	get_floats(n0, n1);
	return (n0 <= n1);
}

int fql_logic_le_s(logic* self, node* rg)
{
	stringview sv0, sv1;
	get_stringviews(sv0, sv1);
	return (stringview_compare_nocase_rtrim(&sv0, &sv1) <= 0);
}

int fql_logic_in_i(logic* self, node* rg)
{
	long n0 = 0;
	try_(expression_get_int(&n0, self->expr[0], rg));

	vec* list_vec = self->in_data->expressions;
	expression** it = vec_begin(list_vec);
	for (; it != vec_end(list_vec); ++it) {
		long n1 = 0;
		try_(expression_get_int(&n1, *it, rg));
		if (n1 == n0) {
			return true;
		}
	}

	return false;
}

int fql_logic_in_f(logic* self, node* rg)
{
	double n0 = 0;
	try_(expression_get_float(&n0, self->expr[0], rg));

	vec* list_vec = self->in_data->expressions;
	expression** it = vec_begin(list_vec);
	for (; it != vec_end(list_vec); ++it) {
		double n1 = 0;
		try_(expression_get_float(&n1, *it, rg));
		if (n1 == n0) {
			return true;
		}
	}

	return false;
}

int fql_logic_in_s(logic* self, node* rg)
{
	stringview sv0;
	try_(expression_get_stringview(&sv0, self->expr[0], rg));

	vec* list_vec = self->in_data->expressions;
	expression** it = vec_begin(list_vec);
	for (; it != vec_end(list_vec); ++it) {
		stringview sv1;
		try_(expression_get_stringview(&sv1, *it, rg));
		if (!stringview_compare_nocase_rtrim(&sv0, &sv1)) {
			return true;
		}
	}

	return false;
}

int fql_logic_subin_i(logic* self, node* rg)
{
	long n0 = 0;
	try_(expression_get_int(&n0, self->expr[0], rg));

	return set_nhas(self->in_data->list_data, (char*)&n0, sizeof(n0));
}

int fql_logic_subin_f(logic* self, node* rg)
{
	double n0 = 0;
	try_(expression_get_float(&n0, self->expr[0], rg));

	return set_nhas(self->in_data->list_data, (char*)&n0, sizeof(n0));
}

int fql_logic_subin_s(logic* self, node* rg)
{
	stringview sv0;
	try_(expression_get_stringview(&sv0, self->expr[0], rg));

	return set_nhas(self->in_data->list_data, sv0.data, sv0.len);
}

int fql_logic_like(logic* self, node* rg)
{
	stringview sv0;
	try_(expression_get_stringview(&sv0, self->expr[0], rg));

	if (self->expr[1]->expr != EXPR_CONST) {
		stringview sv1;
		try_(expression_get_stringview(&sv1, self->expr[1], rg));
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

int fql_logic_is_null(logic* self, node* rg)
{
	fputs("logical NULL check not yet implemented\n", stderr);
	return FQL_FAIL;
}
