#include "function.h"
#include "fql.h"
#include "column.h"

#include "util/stringview.h"

/* ret is assumed to be of the correct type */

int fql_left(Function* fn, union field* ret, Vec* rec)
{
        Column** argv = fn->args->data;
        StringView s;
        column_get_stringview(&s, argv[0], rec);
        long n = 0;
        if (column_get_int(&n, argv[1], rec)) {
                return FQL_FAIL;
        }

        int i = 1;
        for (; i < n && i <= s.len; ++i) {
                string_push_back(ret->s, s.data[i-1]);
        }

        return FQL_GOOD;
}
