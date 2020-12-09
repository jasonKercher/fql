#include "function.h"

#include "fql.h"
#include "column.h"
#include "util/util.h"

Function* function_new(const char* func_name)
{
        Function* new_func = NULL;
        malloc_(new_func, sizeof(*new_func));

        return function_init(new_func, func_name);
}

Function* function_init(Function* func, const char* func_name)
{
        *func = (Function) {
                 NULL                   /* caller */
                ,vec_new_(Column*)      /* args */
                ,""                     /* name */
                ,0                      /* arg_min */
                ,0                      /* arg_max */
        };

        strncpy_(func->name, func_name, FUNC_NAME_MAX);

        func->arg_min = 0;
        func->arg_max = 0;
        if      (istring_eq(func_name, "CURRENT_TIMESTAMP")){ return func; }
        else if (istring_eq(func_name, "GETDATE")){ return func; }
        else if (istring_eq(func_name, "GETUTCDATE")){ return func; }
        else if (istring_eq(func_name, "CURRENT_USER")){ return func; }
        else if (istring_eq(func_name, "SYSTEM_USER")){ return func; }
        else if (istring_eq(func_name, "SESSION_USER")){ return func; }

        if(func->caller)
            return func;

        func->arg_min = 0;
        func->arg_max = 1;
        if (istring_eq(func_name, "RAND")){ return func; }
        if (istring_eq(func_name, "USER_NAME")){ return func; }

        if(func->caller)
            return func;

        func->arg_min = 1;
        func->arg_max = 1;
        if      (istring_eq(func_name, "ABS")){ return func; }
        else if (istring_eq(func_name, "ASCII")){ return func; }
        else if (istring_eq(func_name, "CAST")) /* ???? */{ return func; }
        else if (istring_eq(func_name, "CEILING")){ return func; }
        else if (istring_eq(func_name, "CHAR")){ return func; }
        else if (istring_eq(func_name, "DATALENGTH")) { return func; }
            //_type = COL_INT;
            //func->caller = &sql::DATALENGTH;
        else if (istring_eq(func_name, "DAY")){ return func; }
        else if (istring_eq(func_name, "FLOOR")){ return func; }
        else if (istring_eq(func_name, "ISDATE")){ return func; }
        else if (istring_eq(func_name, "ISNUMERIC")){ return func; }
        else if (istring_eq(func_name, "LEN")) { return func; }
        else if (istring_eq(func_name, "LOWER")){ return func; }
        else if (istring_eq(func_name, "LTRIM")){ return func; }
        else if (istring_eq(func_name, "MONTH")){ return func; }
        else if (istring_eq(func_name, "NCHAR")){ return func; }
        else if (istring_eq(func_name, "RTRIM")){ return func; }
        else if (istring_eq(func_name, "SESSIONPROPERTY")){ return func; }
        else if (istring_eq(func_name, "SPACE")){ return func; }
        else if (istring_eq(func_name, "TRY_CAST")){ return func; }
        else if (istring_eq(func_name, "UPPER")){ return func; }
        else if (istring_eq(func_name, "SIGN")){ return func; }
        else if (istring_eq(func_name, "YEAR")){ return func; }

        if(func->caller)
            return func;

        func->arg_min = 1;
        func->arg_max = 3;
        if (istring_eq(func_name, "STR")){ return func; }

        if(func->caller)
            return func;

        func->arg_min = 2;
        func->arg_max = 2;
        if      (istring_eq(func_name, "DATENAME")){ return func; }
        else if (istring_eq(func_name, "DATEPART")){ return func; }
        else if (istring_eq(func_name, "ISNULL")){ return func; }
        else if (istring_eq(func_name, "LEFT")) { return func; }
        else if (istring_eq(func_name, "NULLIF")){ return func; }
        else if (istring_eq(func_name, "PATINDEX")){ return func; }
        else if (istring_eq(func_name, "RIGHT")) { return func; }

        if(func->caller)
            return func;

        func->arg_min = 2;
        func->arg_max = 3;
        if      (istring_eq(func_name, "CONVERT")){ return func; }
        else if (istring_eq(func_name, "ROUND")){ return func; }
        else if (istring_eq(func_name, "TRY_CONVERT")){ return func; }

        if(func->caller)
            return func;

        func->arg_min = 3;
        func->arg_max = 3;
        if      (istring_eq(func_name, "CHARINDEX")){ return func; }
        else if (istring_eq(func_name, "DATEADD")){ return func; }
        else if (istring_eq(func_name, "DATEDIFF")){ return func; }
        else if (istring_eq(func_name, "REPLACE")){ return func; }
        else if (istring_eq(func_name, "SUBSTRING")) { return func; }

        if(func->caller)
            return func;

        func->arg_min = 4;
        func->arg_max = 4;
        if (istring_eq(func_name, "STUFF")){ return func; }

        if(func->caller)
            return func;

        func->arg_min = 2;
        func->arg_max = 10000;
        if      (istring_eq(func_name, "COALESCE")){ return func; }
        else if (istring_eq(func_name, "CONCAT")){ return func; }

        if(!func->caller) {
                fprintf(stderr, "Function not implemented: %s\n", func_name);
                //function_free(func);
                //return NULL;
        }
        return func;
}

void function_free(Function* func)
{
        free_(func);
}

int function_validate(Function* func)
{
        int argc = func->args->size;
        if (argc >= func->arg_min && argc <= func->arg_max) {
                return FQL_GOOD;
        }
        if (func->arg_min == func->arg_max) {
                fprintf(stderr, 
                        "Function %s expected %d argument(s)... Found %d\n", 
                        func->name, func->arg_min, argc);
        } else {
                fprintf(stderr, 
                        "Function %s expected between %d and %d arguments... Found %d\n", 
                        func->name, func->arg_min, func->arg_max, argc);
        }
        return FQL_FAIL;
}

void function_add_column(Function* func, Column* col)
{
        vec_push_back(func->args, &col); 
}
