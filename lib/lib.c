#include "fql.h"
#include "util/stack.h"
#include "antlr/antlr.h"

int execute_query(const char* query_str)
{
        analyze_query(query_str);

        return 0;
}
