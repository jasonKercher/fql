#include "schema.h"

#include <dirent.h>
#include <csv.h>

#include "fql.h"
#include "io.h"
#include "misc.h"
#include "query.h"
#include "table.h"
#include "group.h"
#include "order.h"
#include "logic.h"
#include "reader.h"
#include "function.h"
#include "aggregate.h"
#include "fqlselect.h"
#include "operation.h"
#include "switchcase.h"
#include "expression.h"
#include "util/util.h"
#include "util/vec.h"
#include "util/stringview.h"

int _resolve_query(struct fql_handle*, query*, enum io union_io);

schema* schema_construct(schema* self)
{
	*self = (schema) {
	        new_t_(vec, expression*), /* expressions */
	        NULL,                     /* expr_map */
	        NULL,                     /* schema_path */
	        NULL,                     /* name */
	        "",                       /* delimiter */
	        "",                       /* line_ending */
	        0,                        /* strictness */
	        IO_UNDEFINED,             /* io_type */
	        IO_UNDEFINED,             /* write_io_type */
	        true,                     /* is_default */
	        false,                    /* delim_is_set */
	};

	return self;
}

void schema_destroy(void* generic_schema)
{
	schema* self = generic_schema;

	expression** it = vec_begin(self->expressions);
	for (; it != vec_end(self->expressions); ++it) {
		delete_(expression, *it);
	}

	delete_(vec, self->expressions);
	delete_if_exists_(string, self->name);
	delete_if_exists_(string, self->schema_path);
	delete_if_exists_(multimap, self->expr_map);
}

void schema_set_delim(schema* self, const char* delim)
{
	strncpy_(self->delimiter, delim, DELIM_LEN_MAX);
	self->delim_is_set = true;
}

bool schema_eq(const schema* s1, const schema* s2)
{
	if (s1->io_type != s2->io_type) {
		return false;
	}
	if (s1->io_type == IO_LIBCSV) {
		return string_eq(s1->delimiter, s2->delimiter);
	}
	return true;
}

void schema_add_expression(schema* self, expression* expr, int src_idx)
{
	expr->src_idx = src_idx;
	expr->index = self->expressions->size;
	vec_push_back(self->expressions, &expr);
}

void schema_apply_expression_alias(schema* self, const char* alias)
{
	expression** expr = vec_back(self->expressions);
	string_strcpy(&(*expr)->alias, alias);
}

void schema_preflight(schema* self)
{
	/* Let's not leak if this was called from order.c */
	if (self->expr_map != NULL) {
		return;
	}

	/* this is a separate step because between adding
	 * a new expression and _here_, we could have applied a
	 * new alias to the expression. we don't care about a
	 * expression name once it has an alias.
	 */
	self->expr_map = new_t_(multimap,
	                        expression*,
	                        self->expressions->size * 2,
	                        HASHMAP_PROP_NOCASE);

	unsigned i = 0;
	expression** expr = vec_begin(self->expressions);
	for (; i < self->expressions->size; ++i) {
		expr[i]->index = i; /* this is occasionally redundant */
		multimap_set(self->expr_map, expr[i]->alias.data, &expr[i]);
	}

	if (!self->delim_is_set) {
		schema_set_delim(self, ",");
	}

	if (self->rec_terminator[0] == '\0') {
		self->rec_terminator[0] = '\n';
		self->rec_terminator[1] = '\0';
	}
}

int _fuzzy_resolve_file(string* dest, const string* input, int strictness)
{
	string basedata;
	string dirdata;
	string file_noext;

	string_construct(&basedata);
	string_construct(&dirdata);
	string_construct(&file_noext);

	string_copy(&basedata, input);
	string_copy(&dirdata, input);

	char* dir = dirname(dirdata.data);
	char* base = basename(basedata.data);

	node* files = dir_list_files(dir);
	node* node = files;

	int matches = 0;
	/* match exact */
	for (; node; node = node->next) {
		if (string_eq(node->data, base)) {
			++matches;
			string_sprintf(dest, "%s/%s", dir, node->data);
			goto fuzzy_file_match_success;
		}
	}

	if (strictness) {
		goto fuzzy_file_match_fail;
	}

	/* match exact ignoring case */
	for (node = files; node; node = node->next) {
		if (istring_eq(node->data, base)) {
			++matches;
			string_sprintf(dest, "%s/%s", dir, node->data);
		}
	}

	if (matches > 1) {
		fprintf(stderr, "%s: ambiguous file name\n", string_c_str(input));
		goto fuzzy_file_match_fail;
	} else if (matches) {
		goto fuzzy_file_match_success;
	}

	/* match file without extension */
	for (node = files; node; node = node->next) {
		string_resize(&file_noext, strlen(node->data));
		getnoext(file_noext.data, node->data);
		if (string_eq(file_noext.data, base)) {
			++matches;
			string_sprintf(dest, "%s/%s", dir, node->data);
		}
	}

	if (matches > 1) {
		fprintf(stderr, "%s.*: ambiguous file\n", base);
		goto fuzzy_file_match_fail;
	} else if (matches) {
		goto fuzzy_file_match_success;
	}

	/* match file without extension ignoring case */
	for (node = files; node; node = node->next) {
		string_resize(&file_noext, strlen(node->data));
		getnoext(file_noext.data, node->data);
		if (istring_eq(file_noext.data, base)) {
			++matches;
			string_sprintf(dest, "%s/%s", dir, node->data);
		}
	}

	if (matches > 1) {
		fprintf(stderr, "%s.*: ambiguous file\n", base);
		goto fuzzy_file_match_fail;
	} else if (matches) {
		goto fuzzy_file_match_success;
	}

	fprintf(stderr,
	        "%s: unable to find matching file (directory: `%s')\n",
	        base,
	        dir);

fuzzy_file_match_fail:
	node_free_data(&files);
	string_destroy(&file_noext);
	string_destroy(&basedata);
	string_destroy(&dirdata);
	return FQL_FAIL;

fuzzy_file_match_success:
	node_free_data(&files);
	string_destroy(&basedata);
	string_destroy(&dirdata);
	string_destroy(&file_noext);
	return FQL_GOOD;
}

int _resolve_file(table* table, int strictness)
{
	if (table->source_type == SOURCE_SUBQUERY) {
		return FQL_GOOD;
	}

	return _fuzzy_resolve_file(&table->reader->file_name, &table->name, strictness);
}

void schema_assign_header(table* table, record* rec, int src_idx)
{
	int i = 0;
	stringview* it = vec_begin(&rec->fields);
	for (; it != vec_end(&rec->fields); ++it) {
		string expr_str;
		string_construct_from_stringview(&expr_str, it);
		expression* new_expr =
		        new_(expression, EXPR_COLUMN_NAME, expr_str.data, "");
		schema_add_expression(table->schema, new_expr, src_idx);

		new_expr->index = i++;
		new_expr->table = table;
		new_expr->field_type = FIELD_STRING;

		string_destroy(&expr_str);
	}

	schema_preflight(table->schema);
}

/* Schema path is determined by the following hierarchy:
 *  1. fql->props.schema_path
 *  2. FQL_SCHEMA_PATH environment variable
 *  3. $HOME/.config/fql
 *  4. ./ OR should probably just throw error here...
 */
int _resolve_schema_path(schema* self, struct fql_handle* fql)
{
	self->schema_path = new_(string);
	DIR* schema_dir = NULL;

	if (!string_empty(fql->props.schema_path)) {
		string_copy(self->schema_path, fql->props.schema_path);
		goto dir_check;
	}

	const char* path_from_env = getenv("FQL_SCHEMA_PATH");
	if (path_from_env != NULL) {
		string_strcpy(self->schema_path, path_from_env);
		goto dir_check;
	}

	const char* home_path = getenv("HOME");
	if (home_path != NULL) {
		string_sprintf(self->schema_path, "%s/.config/fql", home_path);
		goto dir_check;
	}

dir_check:
	schema_dir = opendir(string_c_str(self->schema_path));
	if (schema_dir) {
		closedir(schema_dir);
		return FQL_GOOD;
	}

	/* Optionally, we could default to CWD */
	fprintf(stderr,
	        "Schema path `%s' could not be accessed. Consider creating it.\n",
	        string_c_str(self->schema_path));
	return FQL_FAIL;
}

int _set_variable_from_rec(schema* self, struct csv_record* rec)
{
	if (rec->size != 2) {
		fprintf(stderr,
		        "invalid schema record: `%.*s'\n",
		        (int)rec->reclen,
		        rec->rec);
		return FQL_FAIL;
	}
	fprintf(stderr,
	        "%.*s = %.*s: ignored schema assignment\n",
	        rec->fields[0].len,
	        rec->fields[0].data,
	        rec->fields[1].len,
	        rec->fields[1].data);
	return FQL_GOOD;
}

int _add_expression_from_rec(table* table,
                             struct csv_record* rec,
                             size_t* running_offset,
                             int src_idx)
{
	schema* self = table->schema;
	if (rec->size == 0 || rec->size > 3) {
		fprintf(stderr,
		        "Invalid schema record `%.*s'\n",
		        (int)rec->reclen,
		        rec->rec);
		return FQL_FAIL;
	}
	stringview field_name = {rec->fields[0].data, rec->fields[0].len};
	string expr_str;
	string_construct_from_stringview(&expr_str, &field_name);
	expression* new_expr = new_(expression, EXPR_COLUMN_NAME, expr_str.data, "");
	new_expr->table = table;
	string_destroy(&expr_str);
	switch (rec->size) {
	case 1: /* <field>, assume char */
		table->reader->type = IO_LIBCSV;
		new_expr->field_type = FIELD_STRING;
		break;
	case 2: /* <field>, <type> */
		table->reader->type = IO_LIBCSV;
		/* TODO type parsing */
		new_expr->field_type = FIELD_STRING;
		break;
	case 3: { /* <field>, <offset>, <type> */
		table->reader->type = IO_FIXED;
		/* TODO type parsing */
		new_expr->field_type = FIELD_STRING;
		long width = 0;
		stringview sv = {rec->fields[1].data, rec->fields[1].len};
		string* dup = string_from_stringview(&sv);
		if (str2long(&width, string_c_str(dup)) || width <= 0) {
			fprintf(stderr,
			        "Could not parse expression offset `%s'\n",
			        string_c_str(dup));
			delete_(string, dup);
			delete_(expression, new_expr);
			return FQL_FAIL;
		}

		delete_(string, dup);

		new_expr->location = *running_offset;
		new_expr->width = width;

		*running_offset += width;
	}
	}
	new_expr->index = self->expressions->size;
	schema_add_expression(self, new_expr, src_idx);
	return rec->size;
}

int _parse_schema_file(table* table, struct fql_handle* fql, int src_idx)
{
	schema* self = table->schema;
	struct csv_reader* schema_csv = csv_reader_new();
	schema_csv->trim = 1;
	csv_reader_set_delim(schema_csv, ",");

	struct csv_reader* assignment_csv = csv_reader_new();
	assignment_csv->trim = 1;
	csv_reader_set_delim(assignment_csv, "=");

	struct csv_record* expression_rec = csv_record_new();
	struct csv_record* assignment_rec = csv_record_new();

	int ret = 0;
	if (csv_reader_open(schema_csv, string_c_str(self->schema_path))) {
		ret = FQL_FAIL;
		goto parse_schema_return;
	}

	size_t running_offset = 0;
	int first_count = 0;

	/* Schema file parsing */
	while ((ret = csv_get_record(schema_csv, expression_rec)) == CSV_GOOD) {

		/* Is this a variable assignment? */
		csv_nparse(assignment_csv,
		           assignment_rec,
		           expression_rec->rec,
		           expression_rec->reclen);
		if (assignment_rec->size > 1) {
			if (_set_variable_from_rec(self, assignment_rec) == FQL_FAIL) {
				ret = FQL_FAIL;
				break;
			}
			continue;
		}

		/* we ARE defining a expression */
		int count = _add_expression_from_rec(table,
		                                     expression_rec,
		                                     &running_offset,
		                                     src_idx);
		if (count == FQL_FAIL) {
			ret = FQL_FAIL;
			break;
		}
		if (!first_count) {
			first_count = count;
		} else if (first_count != count) {
			fprintf(stderr,
			        "mis-matched schema record at `%.*s'\n",
			        (int)expression_rec->reclen,
			        expression_rec->rec);
			ret = FQL_FAIL;
			break;
		}
	}
	table->reader->reclen = running_offset;

parse_schema_return:
	csv_record_free(assignment_rec);
	csv_record_free(expression_rec);
	csv_reader_free(assignment_csv);
	csv_reader_free(schema_csv);

	if (ret == FQL_FAIL) {
		return FQL_FAIL;
	}
	return FQL_GOOD;
}

int _load_by_name(table* table, struct fql_handle* fql, int src_idx)
{
	schema* self = table->schema;
	try_(_resolve_schema_path(self, fql));

	string* full_path_temp = new_(string);
	string_sprintf(full_path_temp,
	               "%s/%s",
	               string_c_str(self->schema_path),
	               string_c_str(self->name));

	if (_fuzzy_resolve_file(self->schema_path,
	                        full_path_temp,
	                        fql->props.strictness)) {
		delete_(string, full_path_temp);
		return FQL_FAIL;
	}
	delete_(string, full_path_temp);

	try_(_parse_schema_file(table, fql, src_idx));

	return FQL_GOOD;
}

int _resolve_source(struct fql_handle* fql, table* table, int src_idx)
{
	schema* self = table->schema;
	if (self && !vec_empty(self->expressions)) {
		return FQL_GOOD; /* self already set */
	}

	if (self && (self->name || !string_empty(fql->props.schema))) {
		if (self->name == NULL) {
			self->name = string_from_string(fql->props.schema);
		}

		/* If the name of the schema is default, do nothing */
		const char* DEFAULT = "default";
		stringview default_sv = {DEFAULT, 7};
		stringview schema_name_sv = {self->name->data, self->name->size};
		if (stringview_compare_nocase_rtrim(&default_sv, &schema_name_sv)) {
			/* If we are loading the schema by name, we assume
			 * data begins on the first row. The default schema
			 * assumes that the first row is headers
			 */
			self->is_default = false;
			table->reader->skip_rows = 0;
			try_(_load_by_name(table, fql, src_idx));
		}
	}

	if (table->source_type == SOURCE_SUBQUERY) {
		try_(_resolve_query(fql, table->subquery, IO_UNDEFINED));
		fqlselect* select = table->subquery->op;
		table->schema = select->schema;
		self = table->schema;
		table->reader->type = IO_SUBQUERY;
	} else {
		/* if we've made it this far, we want to try
		 * and determine schema by reading the top
		 * row of the file and assume a delimited
		 * list of field names.
		 * This is the "default schema".
		 */
		try_(_resolve_file(table, fql->props.strictness));
		if (self->is_default) {
			table->reader->type = IO_LIBCSV;
		}
	}
	try_(reader_assign(table->reader, table, fql));

	switch (table->reader->type) {
	case IO_FIXED:
		schema_preflight(self);
		return FQL_GOOD;
	case IO_SUBQUERY:
		return FQL_GOOD;
	case IO_LIBCSV:
	default:;
	}

	//node* rg = new_(node, 0);
	node rg = {0};
	rg.data = new_(record, 0);
	table->reader->max_idx = INT_MAX;
	table->reader->get_record__(table->reader, &rg);
	table->reader->max_idx = 0;

	/* redundant if default schema */
	table->reader->reset__(table->reader);

	record* rec = rg.data;

	if (self->is_default) {
		schema_assign_header(table, rec, src_idx);
	} else {
		/* TODO: in the future, maybe just set these SQL NULL */
		expression** it = vec_at(self->expressions, rec->fields.size);
		for (; it != vec_end(self->expressions); ++it) {
			delete_(expression, *it);
		}
		vec_resize(self->expressions, rec->fields.size);
		schema_preflight(self);
	}
	delete_(record, rg.data);

	return FQL_GOOD;
}

int _evaluate_if_const(expression* expr)
{
	function* func = expr->field.fn;
	expression** it = vec_begin(func->args);
	for (; it != vec_end(func->args); ++it) {
		if ((*it)->expr != EXPR_CONST) {
			return FQL_GOOD;
		}
	}

	union field new_field;
	if (expr->field_type == FIELD_STRING) {
		new_field.s = &expr->buf;
		string_clear(new_field.s);
	}
	try_(func->call__(func, &new_field, NULL));

	delete_(function, func);
	expr->expr = EXPR_CONST;
	expr->field = new_field;

	return FQL_GOOD;
}

int _assign_expressions_limited(vec* expressions, vec* sources, int, int);

int _assign_expression_limited(expression* expr, vec* sources, int limit, int strictness)
{
	switch (expr->expr) {
	case EXPR_SWITCH_CASE: {
		switchcase* sc = expr->field.sc;
		logicgroup** lg_iter = vec_begin(&sc->tests);
		for (; lg_iter != vec_end(&sc->tests); ++lg_iter) {
			try_(_assign_expressions_limited((*lg_iter)->expressions,
			                                 sources,
			                                 limit,
			                                 strictness));
		}
		try_(_assign_expressions_limited(&sc->values,
		                                 sources,
		                                 limit,
		                                 strictness));
		try_(switchcase_resolve_type(sc, expr));
		break;
	}
	case EXPR_FUNCTION: {
		function* func = expr->field.fn;
		try_(_assign_expressions_limited(func->args, sources, limit, strictness));
		try_(function_op_resolve(func, &expr->field_type));
		try_(function_validate(func, expr));
		try_(_evaluate_if_const(expr));
		break;
	}
	case EXPR_SUBQUERY:
		try_(fqlselect_resolve_type_from_subquery(expr));
		break;
	case EXPR_FULL_RECORD:
	case EXPR_COLUMN_NAME: {
		if (expr->data_source != NULL) {
			return FQL_GOOD;
		}

		int matches = 0;
		int j = 0;

		for (; j <= limit; ++j) {
			table* search_table = vec_at(sources, j);
			if (string_empty(&expr->table_name)
			    || istring_eq(expr->table_name.data,
			                  search_table->alias.data)) {
				int n = expression_try_assign_source(expr, search_table);
				if (n > 1 && !strictness) {
					n = 1;
				}
				matches += n;
			}
		}

		if (matches > 1) {
			fprintf(stderr,
			        "%s: ambiguous expression\n",
			        (char*)expr->name.data);
			return FQL_FAIL;
		}

		if (matches == 0) {
			fprintf(stderr,
			        "%s: cannot find expression\n",
			        (char*)expr->name.data);
			return FQL_FAIL;
		}
	}
	default:;
	}

	return FQL_GOOD;
}

int _assign_expressions_limited(vec* expressions, vec* sources, int limit, int strictness)
{
	expression** it = vec_begin(expressions);
	for (; it != vec_end(expressions); ++it) {
		try_(_assign_expression_limited(*it, sources, limit, strictness));
	}

	return FQL_GOOD;
}

int _assign_expressions(vec* expressions, vec* sources, int strictness)
{
	return _assign_expressions_limited(expressions,
	                                   sources,
	                                   sources->size - 1,
	                                   strictness);
}

int _asterisk_resolve(vec* expressions, vec* sources)
{
	unsigned i = 0;
	for (; i < expressions->size; ++i) {
		int expr_idx = i;
		expression** expr = vec_at(expressions, i);
		if ((*expr)->expr != EXPR_ASTERISK) {
			continue;
		}
		int matches = 0;
		unsigned j = 0;
		for (; j < sources->size; ++j) {
			table* search_table = vec_at(sources, j);
			if (string_empty(&(*expr)->table_name)
			    || istring_eq((*expr)->table_name.data,
			                  search_table->alias.data)) {
				if (matches > 0) {
					expression* new_expr =
					        new_(expression, EXPR_ASTERISK, NULL, "");
					new_expr->src_idx = j;
					vec_insert_at(expressions, ++i, &new_expr, 1);
					expr = vec_at(expressions, expr_idx);
				} else {
					(*expr)->src_idx = j;
				}
				++matches;
			}
		}

		if (matches == 0) {
			fprintf(stderr,
			        "could not locate table `%s'\n",
			        (char*)(*expr)->table_name.data);
			return FQL_FAIL;
		}
	}

	return FQL_GOOD;
}

enum join_side _get_join_side(expression* expr, int right_idx)
{
	switch (expr->expr) {
	case EXPR_FULL_RECORD:
	case EXPR_COLUMN_NAME:
		return (expr->data_source->src_idx < right_idx) ? SIDE_LEFT : SIDE_RIGHT;
	case EXPR_FUNCTION: {
		function* func = expr->field.fn;
		expression** it = vec_begin(func->args);
		enum join_side side0 = SIDE_UNDEF;
		for (; it != vec_end(func->args); ++it) {
			enum join_side side1 = _get_join_side(*it, right_idx);
			if (side0 == SIDE_UNDEF) {
				side0 = side1;
			} else if (side1 == SIDE_UNDEF) {
				continue;
			} else if (side0 != side1) {
				return SIDE_MIXED;
			}
		}
		return side0;
	}
	default:
		return SIDE_UNDEF;
	}
}

void _resolve_join_conditions(table* right_table, int right_idx)
{
	if (right_table->condition == NULL || right_table->condition->joinable == NULL) {
		return;
	}

	vec* joinable = right_table->condition->joinable;

	logic** it = vec_begin(joinable);
	for (; it != vec_end(joinable); ++it) {
		enum join_side side0 = _get_join_side((*it)->expr[0], right_idx);
		if (side0 == SIDE_MIXED) {
			continue;
		}
		enum join_side side1 = _get_join_side((*it)->expr[1], right_idx);
		if (side0 != side1) {
			if (!logic_can_be_false(right_table->condition, *it)) {
				right_table->condition->join_logic = *it;
				hashjoin* hj = new_(hashjoin);
				if (side0 == SIDE_RIGHT) {
					hj->right_expr = (*it)->expr[0];
					hj->left_expr = (*it)->expr[1];
				} else {
					hj->right_expr = (*it)->expr[1];
					hj->left_expr = (*it)->expr[0];
				}
				right_table->join_data = hj;
				break;
			}
		}
	}
}

enum _expr_type {
	MAP_CONST_INT,
	MAP_CONST_FLOAT,
	MAP_CONST_STRING,
	MAP_COLUMN,
	MAP_AGGREGATE,
	MAP_SUBQUERY,
	MAP_FUNCTION,
	MAP_FUNCTION_END,
	MAP_SWITCHCASE,
	MAP_SWITCHCASE_END,
};

int _map_expression(vec* key, expression* expr)
{
	static const enum _expr_type _c_int = MAP_CONST_INT;
	static const enum _expr_type _c_float = MAP_CONST_FLOAT;
	static const enum _expr_type _c_string = MAP_CONST_STRING;
	static const enum _expr_type _expr = MAP_COLUMN;
	static const enum _expr_type _agg = MAP_AGGREGATE;
	static const enum _expr_type _sub = MAP_SUBQUERY;
	static const enum _expr_type _func = MAP_FUNCTION;
	static const enum _expr_type _func_end = MAP_FUNCTION_END;
	static const enum _expr_type _switchcase = MAP_SWITCHCASE;
	static const enum _expr_type _switchcase_end = MAP_SWITCHCASE_END;

	stringview type_sv = {NULL, sizeof(enum _expr_type)};
	stringview val_sv;

	switch (expr->expr) {
	case EXPR_CONST:
		switch (expr->field_type) {
		case FIELD_INT:
			type_sv.data = (void*)&_c_int;
			vec_push_back(key, &type_sv);
			stringview_nset(&val_sv, (char*)&expr->field.f, sizeof(double));
			vec_push_back(key, &val_sv);
			break;
		case FIELD_FLOAT:
			type_sv.data = (void*)&_c_float;
			vec_push_back(key, &type_sv);
			stringview_nset(&val_sv, (char*)&expr->field.f, sizeof(double));
			vec_push_back(key, &val_sv);
			break;
		case FIELD_STRING:
			type_sv.data = (void*)&_c_string;
			vec_push_back(key, &type_sv);
			stringview_set_string(&val_sv, expr->field.s);
			vec_push_back(key, &val_sv);
		default:;
		}

		break;
	case EXPR_FULL_RECORD:
	case EXPR_COLUMN_NAME:
		type_sv.data = (void*)&_expr;
		vec_push_back(key, &type_sv);
		stringview_nset(&val_sv, (char*)&expr->data_source, sizeof(expression*));
		vec_push_back(key, &val_sv);
		break;
	case EXPR_FUNCTION: {
		type_sv.data = (void*)&_func;
		vec_push_back(key, &type_sv);
		stringview_nset(&val_sv,
		                (char*)&expr->field.fn->type,
		                sizeof(enum scalar_function));
		vec_push_back(key, &val_sv);

		expression** it = vec_begin(expr->field.fn->args);
		for (; it != vec_end(expr->field.fn->args); ++it) {
			try_(_map_expression(key, *it));
		}

		type_sv.data = (void*)&_func_end;
		vec_push_back(key, &type_sv); /* SIGNIFY FUNCTION ARGS END */
		break;
	}
	case EXPR_AGGREGATE:
		type_sv.data = (void*)&_agg;
		vec_push_back(key, &type_sv);
		stringview_nset(&val_sv,
		                (char*)&expr->field.agg->agg_type,
		                sizeof(enum aggregate_function));
		vec_push_back(key, &val_sv);
		break;
	case EXPR_SWITCH_CASE: {
		type_sv.data = (void*)&_switchcase;
		vec_push_back(key, &type_sv);

		expression** it = vec_begin(&expr->field.sc->values);
		for (; it != vec_end(&expr->field.sc->values); ++it) {
			try_(_map_expression(key, *it));
		}

		type_sv.data = (void*)&_switchcase_end;
		vec_push_back(key, &type_sv); /* SIGNIFY CASE EXPRESSION END */
		break;
	}
	/* Maybe let's not group by a subquery expression?? */
	case EXPR_SUBQUERY:
		type_sv.data = (void*)&_sub;
		vec_push_back(key, &type_sv);
		stringview_nset(&val_sv, (char*)&expr->subquery, sizeof(void*));
		vec_push_back(key, &val_sv);
		break;
	default:
		fputs("unexpected expression\n", stderr);
		return FQL_FAIL;
	}

	return FQL_GOOD;
}

int _op_find_group(compositemap* expr_map, expression* expr, vec* key, bool loose_groups)
{
	if (expr->expr == EXPR_CONST || expr->expr == EXPR_SUBQUERY) {
		return FQL_GOOD;
	}

	if (expr->expr == EXPR_AGGREGATE) {
		expr->src_idx = 0;
		expr->field_type = expr->data_source->field_type;
		return FQL_GOOD;
	}

	vec_clear(key);
	try_(_map_expression(key, expr));

	expression** result = compositemap_get(expr_map, key);
	if (result != NULL) {
		if (loose_groups) {
			return FQL_GOOD;
		}
		if (expr->expr == EXPR_FUNCTION) {
			delete_(function, expr->field.fn);
		}
		expr->src_idx = 0;
		expr->data_source = *result;
		expr->field_type = (*result)->field_type;
		expr->expr = EXPR_GROUPING;
		return FQL_GOOD;
	}

	if (expr->expr == EXPR_FUNCTION) {
		expression** it = vec_begin(expr->field.fn->args);
		for (; it != vec_end(expr->field.fn->args); ++it) {
			try_(_op_find_group(expr_map, *it, key, loose_groups));
		}
		return FQL_GOOD;
	}

	if (expr->expr == EXPR_SWITCH_CASE) {
		expression** it = vec_begin(&expr->field.sc->values);
		for (; it != vec_end(&expr->field.sc->values); ++it) {
			try_(_op_find_group(expr_map, *it, key, loose_groups));
		}

		logicgroup** lg_iter = vec_begin(&expr->field.sc->tests);
		for (; lg_iter != vec_end(&expr->field.sc->tests); ++lg_iter) {
			it = vec_begin((*lg_iter)->expressions);
			for (; it != vec_end((*lg_iter)->expressions); ++it) {
				try_(_op_find_group(expr_map, *it, key, loose_groups));
			}
		}
		return FQL_GOOD;
	}

	if (loose_groups) {
		return FQL_GOOD;
	}

	if (expr->expr == EXPR_COLUMN_NAME) {
		fprintf(stderr,
		        "expression `%s' does not match a grouping\n",
		        (char*)expr->alias.data);
		return FQL_FAIL;
	}

	fprintf(stderr,
	        "expression `%s' unexpected expression\n",
	        (char*)expr->alias.data);
	return FQL_FAIL;
}

int _map_groups(struct fql_handle* fql, query* query)
{
	/* verify group expressions and build composite key for each */
	vec* group_exprs = &query->groupby->expressions;
	try_(_assign_expressions(group_exprs, query->sources, fql->props.strictness));

	compositemap* expr_map = new_t_(compositemap,
	                                expression*,
	                                group_exprs->size * 2,
	                                HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM);
	query->groupby->expr_map = expr_map;
	vec key;
	vec_construct_(&key, stringview);

	expression** it = vec_begin(group_exprs);
	for (; it != vec_end(group_exprs); ++it) {
		if ((*it)->expr != EXPR_AGGREGATE) {
			vec_clear(&key);
			if (_map_expression(&key, *it)) {
				vec_destroy(&key);
				return FQL_FAIL;
			}
			compositemap_set(expr_map, &key, it);
			continue;
		}
		aggregate* agg = (*it)->field.agg;
		try_(_assign_expressions(agg->args,
		                         query->sources,
		                         fql->props.strictness));
		try_(aggregate_resolve(agg, *it));
		(*it)->field_type = agg->data_type;
		agg->linked_expression->field_type = agg->data_type;
	}

	vec_destroy(&key);
	return FQL_GOOD;
}

int _group_validation(query* query, vec* exprs, vec* op_exprs, bool loose_groups)
{
	compositemap* expr_map = query->groupby->expr_map;
	vec key;
	vec_construct_(&key, stringview);
	expression** it = vec_begin(exprs);
	for (; it != vec_end(exprs); ++it) {
		if ((*it)->expr == EXPR_GROUPING) {
			expression** matched = vec_at(op_exprs, (*it)->index);
			(*it)->src_idx = 0;
			(*it)->data_source = (*matched)->data_source;
			(*it)->field_type = (*matched)->field_type;
			continue;
		}
		if (_op_find_group(expr_map, *it, &key, loose_groups)) {
			vec_destroy(&key);
			return FQL_FAIL;
		}
	}

	vec_destroy(&key);
	return FQL_GOOD;
}

int _resolve_unions(struct fql_handle* fql, query* aquery)
{
	/* As we loop through union queries, we want to
	 * also verify that we have the same number of
	 * expressions.
	 */
	enum io main_io_type = op_get_schema(aquery->op)->io_type;

	unsigned expected = fqlselect_get_field_count(aquery->op);
	query** query_iter = vec_begin(aquery->unions);
	for (; query_iter != vec_end(aquery->unions); ++query_iter) {
		try_(_resolve_query(fql, *query_iter, main_io_type));
		if (expected != fqlselect_get_field_count((*query_iter)->op)) {
			fputs("UNION query schema mis-match\n", stderr);
			return FQL_FAIL;
		}
	}

	/* We need to resolve final types for each expression */
	try_(op_resolve_final_types(aquery->op));

	return FQL_GOOD;
}


/* Tie everything together and verify correctness. */
int _resolve_query(struct fql_handle* fql, query* aquery, enum io union_io)
{
	/* First let's resolve any subqueries that can be
	 * classified as an expression.  They should be
	 * constant value and aren't tied to parent query.
	 */
	vec* subqueries = aquery->subquery_const_vec;
	query** query_iter = vec_begin(subqueries);
	for (; query_iter != vec_end(subqueries); ++query_iter) {
		try_(_resolve_query(fql, *query_iter, union_io));
	}

	/* Next, we must resolve any TOP expressions */
	if (aquery->top_expr != NULL) {
		vec temp;
		vec_construct_(&temp, expression*);
		vec_push_back(&temp, &aquery->top_expr);
		int ret = _assign_expressions_limited(&temp,
		                                      NULL,
		                                      0,
		                                      fql->props.strictness);
		vec_destroy(&temp);
		if (ret == FQL_FAIL) {
			return FQL_FAIL;
		}
		if (aquery->top_expr->expr != EXPR_CONST) {
			fputs("Could not resolve top expression\n", stderr);
			return FQL_FAIL;
		}
		if (aquery->top_expr->field_type != FIELD_INT) {
			fputs("Input to TOP clause must be an integer\n", stderr);
			return FQL_FAIL;
		}
		if (aquery->top_expr->field.i < 0) {
			fputs("Input to TOP clause cannot be negative\n", stderr);
			return FQL_FAIL;
		}
		aquery->top_count = aquery->top_expr->field.i;
		delete_(expression, aquery->top_expr);
	}

	/* If there is an order by, make sure NOT to send top_count
	 * to the operation. However, if there is a union, this does
	 * not apply.  If there is a union, the top count belongs to
	 * the operation (select can be assumed). The only goal is to
	 * make sure ALL the selected records are ordered
	 */
	if (aquery->orderby != NULL && vec_empty(aquery->unions)) {
		aquery->orderby->top_count = aquery->top_count;
	} else {
		op_set_top_count(aquery->op, aquery->top_count);
	}

	/* Now, we should verify that all sources
	 * exist and populate schemas.  As we loop, we
	 * resolve the expressions that are listed in join
	 * clauses because of the following caveat:
	 *
	 * SELECT *
	 * FROM T1
	 * JOIN T2 ON T1.FOO = T3.FOO -- Cannot read T3 yet!
	 * JOIN T3 ON T2.FOO = T3.FOO
	 */
	unsigned i = 0;
	vec* sources = aquery->sources;
	for (; i < sources->size; ++i) {
		table* table = vec_at(aquery->sources, i);
		try_(_resolve_source(fql, table, i));
		try_(table_resolve_schema(table, fql));

		if (union_io != IO_UNDEFINED) {
			op_get_schema(aquery->op)->write_io_type = union_io;
		} else if (op_get_schema(aquery->op)->write_io_type == IO_UNDEFINED) {
			op_set_schema(aquery->op, table->schema);
		}

		/* Validate expressions used in JOIN clauses */
		if (table->condition != NULL) {
			try_(_assign_expressions_limited(table->condition->expressions,
			                                 sources,
			                                 i,
			                                 fql->props.strictness));
		}

		if (i > 0 && !fql->props.force_cartesian) {
			_resolve_join_conditions(table, i);
		}
	}

	schema* op_schema = op_get_schema(aquery->op);
	if (op_schema->write_io_type == IO_UNDEFINED) {
		op_set_schema(aquery->op, NULL);
	}

	/* Validate WHERE expressions */
	if (aquery->where != NULL) {
		try_(_assign_expressions(aquery->where->expressions,
		                         aquery->sources,
		                         fql->props.strictness));
	}

	/* Validate the expressions from the operation.
	 * (e.g. expressions listed in SELECT).
	 */
	vec* op_exprs = op_get_expressions(aquery->op);
	try_(_asterisk_resolve(op_exprs, sources));
	try_(_assign_expressions(op_exprs, sources, fql->props.strictness));

	op_exprs = op_get_additional_exprs(aquery->op);
	if (op_exprs != NULL) {
		try_(_assign_expressions(op_exprs, sources, fql->props.strictness));
		try_(op_resolve_additional(aquery->op, aquery));
	}

	/* Validate HAVING expressions */
	vec* having_exprs = NULL;
	if (aquery->having != NULL) {
		having_exprs = aquery->having->expressions;
		try_(_assign_expressions(having_exprs,
		                         aquery->sources,
		                         fql->props.strictness));
	}

	/* Validate ORDER BY expressions if they exist */
	vec* order_exprs = NULL;
	if (aquery->orderby) {
		order_exprs = &aquery->orderby->expressions;
		try_(order_preresolve_expressions(aquery->orderby, aquery->op));
		try_(_assign_expressions(order_exprs,
		                         aquery->sources,
		                         fql->props.strictness));
	}

	/* Do GROUP BY last. There are less caveats having
	 * waited until everything else is already resolved
	 */
	if (aquery->groupby != NULL) {
		try_(_map_groups(fql, aquery));

		/* Now that we have mapped the groups,
		 * we must re-resolve each operation,
		 * HAVING and ORDER BY expression to a group.
		 */
		try_(_group_validation(aquery, op_exprs, NULL, fql->props.loose_groups));

		if (having_exprs) {
			try_(_group_validation(aquery,
			                       having_exprs,
			                       NULL,
			                       fql->props.loose_groups));
		}

		/* exceptions: ordinal ordering or matched by alias */
		if (order_exprs) {
			try_(_group_validation(aquery,
			                       order_exprs,
			                       op_exprs,
			                       fql->props.loose_groups));
		}
	} else if (aquery->orderby) {
		/* This is normally handled in _op_find_group,
		 * but if there is no GROUP BY, just assign the
		 * pre-resolved ORDER BY expressions.
		 */
		expression** it = vec_begin(order_exprs);
		for (; it != vec_end(order_exprs); ++it) {
			if ((*it)->expr != EXPR_GROUPING) {
				continue;
			}
			expression** matched = vec_at(op_exprs, (*it)->index);
			(*it)->data_source = (*matched)->data_source;
			(*it)->field_type = (*matched)->field_type;
		}
	}

	if (!vec_empty(aquery->unions)) {
		try_(_resolve_unions(fql, aquery));
	}

	try_(op_writer_init(aquery, fql));
	schema_preflight(op_schema);

	return FQL_GOOD;
}

int schema_resolve(struct fql_handle* fql)
{
	node* query_node = fql->query_list;
	for (; query_node; query_node = query_node->next) {
		query* query = query_node->data;

		if (fql->props._out_delim_set) {
			op_set_delim(query->op, fql->props.out_delim);
		}

		if (fql->props.rec_terminator[0]) {
			op_set_rec_terminator(query->op, fql->props.rec_terminator);
		}

		try_(_resolve_query(fql, query, IO_UNDEFINED));
	}

	return FQL_GOOD;
}
