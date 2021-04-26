#include "function.h"

#include "fql.h"
#include "column.h"
#include "util/util.h"

int not_implemented(Function* fn, union field* f, Vec* rec)
{
	fprintf(stderr, "function not implemented: %s\n", fn->name);
	return 0;
}

Function* function_new(const char* func_name, enum field_type* type)
{
	Function* new_func = NULL;
	malloc_(new_func, sizeof(*new_func));

	return function_construct(new_func, func_name, type);
}

Function* function_new_op(enum expr_operator op)
{
	Function* new_func = NULL;
	malloc_(new_func, sizeof(*new_func));

	*new_func = (Function) {
		 &not_implemented       /* call__ */
		,vec_new_(Column*)      /* args */
		,""                     /* name */
		,op                     /* operator */
		,2                      /* arg_min */
		,2                      /* arg_max */
	};

	return new_func;
}

int _invalid_type(Function* fn)
{
	fprintf(stderr, "Invalid type for %s operation\n", fn->name);
	return FQL_FAIL;
}

int function_op_resolve(Function* func, enum field_type* type)
{
	if (func->op == OPERATOR_NONE) {
		return FQL_GOOD;
	}

	Column** args = func->args->data;
	Column* col0 = args[0];
	Column* col1 = col0;
	if (func->args->size == 2) {
		col1 = args[1];
	}

	*type = field_determine_type(col0->field_type, col1->field_type);

	switch (func->op) {
	case OPERATOR_PLUS:
		strncpy_(func->name, "PLUS", FUNC_NAME_MAX);
		break;
	case OPERATOR_MINUS:
		strncpy_(func->name, "MINUS", FUNC_NAME_MAX);
		break;
	case OPERATOR_MULTIPY:
		strncpy_(func->name, "MULTIPLY", FUNC_NAME_MAX);
		break;
	case OPERATOR_DIVIDE:
		strncpy_(func->name, "DIVIDE", FUNC_NAME_MAX);
		break;
	case OPERATOR_MODULE:
		strncpy_(func->name, "MODULE", FUNC_NAME_MAX);
		break;
	case OPERATOR_BIT_OR:
		strncpy_(func->name, "BIT_OR", FUNC_NAME_MAX);
		break;
	case OPERATOR_BIT_AND:
		strncpy_(func->name, "BIT_AND", FUNC_NAME_MAX);
		break;
	case OPERATOR_BIT_XOR:
		strncpy_(func->name, "BIT_XOR", FUNC_NAME_MAX);
		break;
	case OPERATOR_BIT_NOT:
		strncpy_(func->name, "BIT_NOT", FUNC_NAME_MAX);
		break;
	default:
		;
	}

	func->call__ = scalar_matrix[func->op][*type];
	if (func->call__ == NULL) {
		return _invalid_type(func);
	}

	return FQL_GOOD;
}

Function* function_construct(Function* func, const char* func_name, enum field_type* type)
{
	*func = (Function) {
		 &not_implemented       /* call__ */
		,vec_new_(Column*)      /* args */
		,""                     /* name */
		,OPERATOR_NONE          /* op */
		,0                      /* arg_min */
		,0                      /* arg_max */
	};

	strncpy_(func->name, func_name, FUNC_NAME_MAX);
	//string_construct(&func->ret_buf);

	func->arg_min = 0;
	func->arg_max = 0;
	if      (istring_eq(func_name, "CURRENT_TIMESTAMP")){ return func; }
	else if (istring_eq(func_name, "GETDATE")){ return func; }
	else if (istring_eq(func_name, "GETUTCDATE")){ return func; }
	else if (istring_eq(func_name, "CURRENT_USER")){ return func; }
	else if (istring_eq(func_name, "SYSTEM_USER")){ return func; }
	else if (istring_eq(func_name, "SESSION_USER")){ return func; }

	func->arg_min = 0;
	func->arg_max = 1;
	if (istring_eq(func_name, "RAND")){ return func; }
	if (istring_eq(func_name, "USER_NAME")){ return func; }

	func->arg_min = 1;
	func->arg_max = 1;
	if      (istring_eq(func_name, "ABS")){ return func; }
	else if (istring_eq(func_name, "ASCII")){ return func; }
	else if (istring_eq(func_name, "CAST")) /* ???? */{ return func; }
	else if (istring_eq(func_name, "CEILING")){ return func; }
	else if (istring_eq(func_name, "CHAR")){ return func; }
	else if (istring_eq(func_name, "DATALENGTH")) { return func; }
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

	func->arg_min = 1;
	func->arg_max = 3;
	if (istring_eq(func_name, "STR")){ return func; }

	func->arg_min = 2;
	func->arg_max = 2;
	if      (istring_eq(func_name, "DATENAME")){ return func; }
	else if (istring_eq(func_name, "DATEPART")){ return func; }
	else if (istring_eq(func_name, "ISNULL")){ return func; }
	else if (istring_eq(func_name, "LEFT")) {
		func->call__ = &fql_left;
		*type = FIELD_STRING;
		return func;
	}
	else if (istring_eq(func_name, "NULLIF")){ return func; }
	else if (istring_eq(func_name, "PATINDEX")){ return func; }
	else if (istring_eq(func_name, "RIGHT")) {
		func->call__ = &fql_right;
		*type = FIELD_STRING;
		return func;
	}

	func->arg_min = 2;
	func->arg_max = 3;
	if      (istring_eq(func_name, "CONVERT")){ return func; }
	else if (istring_eq(func_name, "ROUND")){ return func; }
	else if (istring_eq(func_name, "TRY_CONVERT")){ return func; }

	func->arg_min = 3;
	func->arg_max = 3;
	if      (istring_eq(func_name, "CHARINDEX")){ return func; }
	else if (istring_eq(func_name, "DATEADD")){ return func; }
	else if (istring_eq(func_name, "DATEDIFF")){ return func; }
	else if (istring_eq(func_name, "REPLACE")){ return func; }
	else if (istring_eq(func_name, "SUBSTRING")) { return func; }

	func->arg_min = 4;
	func->arg_max = 4;
	if (istring_eq(func_name, "STUFF")){ return func; }

	func->arg_min = 2;
	func->arg_max = 10000;
	if      (istring_eq(func_name, "COALESCE")){ return func; }
	else if (istring_eq(func_name, "CONCAT")){ return func; }

	return func;
}

void function_free(Function* func)
{
	Column** it = vec_begin(func->args);
	for (; it != vec_end(func->args); ++it) {
		column_free(*it);
	}
	vec_free(func->args);
	//string_destroy(&func->ret_buf);
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
			"Function `%s' expected %d argument(s)... Found %d\n",
			func->name, func->arg_min, argc);
	} else {
		fprintf(stderr,
			"Function `%s' expected between %d and %d arguments... Found %d\n",
			func->name, func->arg_min, func->arg_max, argc);
	}
	return FQL_FAIL;
}

void function_add_column(Function* func, void* col)
{
	vec_push_back(func->args, &col);
}
