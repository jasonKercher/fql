#include "logic.h"
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


int fql_logic_eq_i(Logic* logic, Vec* recs)
{
        long n0 = 0;
        long n1 = 0;
        get_integers(n0, n1);
        return (n0 == n1);
}

int fql_logic_eq_f(Logic* logic, Vec* recs)
{
        double n0 = 0;
        double n1 = 0;
        get_floats(n0, n1);
        return (n0 == n1);
}

int fql_logic_eq_s(Logic* logic, Vec* recs)
{
        StringView s0, s1;
        get_stringviews(s0, s1);
        return FQL_FAIL;
}

int fql_logic_ne_i(Logic* logic, Vec* recs)
{
        long n0 = 0;
        long n1 = 0;
        get_integers(n0, n1);
        return (n0 != n1);
}

int fql_logic_ne_f(Logic* logic, Vec* recs)
{
        double n0 = 0;
        double n1 = 0;
        get_floats(n0, n1);
        return (n0 != n1);
}

int fql_logic_ne_s(Logic* logic, Vec* recs)
{
        StringView s0, s1;
        get_stringviews(s0, s1);
        return FQL_FAIL;
}

int fql_logic_gt_i(Logic* logic, Vec* recs)
{
        long n0 = 0;
        long n1 = 0;
        get_integers(n0, n1);
        return (n0 > n1);
}

int fql_logic_gt_f(Logic* logic, Vec* recs)
{
        double n0 = 0;
        double n1 = 0;
        get_floats(n0, n1);
        return (n0 > n1);
        return 1;
}

int fql_logic_gt_s(Logic* logic, Vec* recs)
{
        StringView s0, s1;
        get_stringviews(s0, s1);
        return FQL_FAIL;
}

int fql_logic_ge_i(Logic* logic, Vec* recs)
{
        long n0 = 0;
        long n1 = 0;
        get_integers(n0, n1);
        return (n0 >= n1);
}

int fql_logic_ge_f(Logic* logic, Vec* recs)
{
        double n0 = 0;
        double n1 = 0;
        get_floats(n0, n1);
        return (n0 >= n1);
}

int fql_logic_ge_s(Logic* logic, Vec* recs)
{
        StringView s0, s1;
        get_stringviews(s0, s1);
        return FQL_FAIL;
}

int fql_logic_lt_i(Logic* logic, Vec* recs)
{
        long n0 = 0;
        long n1 = 0;
        get_integers(n0, n1);
        return (n0 < n1);
}

int fql_logic_lt_f(Logic* logic, Vec* recs)
{
        double n0 = 0;
        double n1 = 0;
        get_floats(n0, n1);
        return (n0 < n1);
}

int fql_logic_lt_s(Logic* logic, Vec* recs)
{
        StringView s0, s1;
        get_stringviews(s0, s1);
        return FQL_FAIL;
}

int fql_logic_le_i(Logic* logic, Vec* recs)
{
        long n0 = 0;
        long n1 = 0;
        get_integers(n0, n1);
        return (n0 <= n1);
}

int fql_logic_le_f(Logic* logic, Vec* recs)
{
        double n0 = 0;
        double n1 = 0;
        get_floats(n0, n1);
        return (n0 <= n1);
}

int fql_logic_le_s(Logic* logic, Vec* recs)
{
        StringView s0, s1;
        get_stringviews(s0, s1);
        return FQL_FAIL;
}

int fql_logic_like(Logic* logic, Vec* recs)
{
        fputs("logical like not yet implemented\n", stderr);
        return FQL_FAIL;
}

int fql_logic_not_like(Logic* logic, Vec* recs)
{
        fputs("logical like not yet implemented\n", stderr);
        return FQL_FAIL;
}

int fql_logic_is_null(Logic* logic, Vec* recs)
{
        fputs("logical NULL check not yet implemented\n", stderr);
        return FQL_FAIL;
}

int fql_logic_not_null(Logic* logic, Vec* recs)
{
        fputs("logical NULL check not yet implemented\n", stderr);
        return FQL_FAIL;
}


