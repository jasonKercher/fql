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
	        new_(string),             /* schema_path */
	        NULL,                     /* name */
	        "",                       /* delimiter */
	        "",                       /* line_ending */
	        0,                        /* strictness */
	        IO_UNDEFINED,             /* io_type */
	        IO_UNDEFINED,             /* write_io_type */
	        true,                     /* is_default */
	        false,                    /* delim_is_set */
	        false,                    /* is_preresolved */
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
		multimap_nset(self->expr_map,
		              expr[i]->alias.data,
		              &expr[i],
		              expr[i]->alias.size);
	}

	if (!self->delim_is_set) {
		schema_set_delim(self, ",");
	}

	if (self->rec_terminator[0] == '\0') {
		self->rec_terminator[0] = '\n';
		self->rec_terminator[1] = '\0';
	}
}

enum _fuzzy_return {
	FUZZY_SUCCESS,
	FUZZY_AMBIGUOUS,
	FUZZY_NOTFOUND,
};


enum _fuzzy_return _fuzzy_resolve_file(string* dest, const string* input, int strictness)
{
	enum _fuzzy_return ret = FUZZY_NOTFOUND;
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
			ret = FUZZY_SUCCESS;
			goto fuzzy_file_match_return;
		}
	}

	if (strictness) {
		goto fuzzy_file_match_return;
	}

	/* match exact ignoring case */
	for (node = files; node; node = node->next) {
		if (istring_eq(node->data, base)) {
			++matches;
			string_sprintf(dest, "%s/%s", dir, node->data);
		}
	}

	if (matches) {
		ret = (matches > 1) ? FUZZY_AMBIGUOUS : FUZZY_SUCCESS;
		goto fuzzy_file_match_return;
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

	if (matches) {
		ret = (matches > 1) ? FUZZY_AMBIGUOUS : FUZZY_SUCCESS;
		goto fuzzy_file_match_return;
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

	if (matches) {
		ret = (matches > 1) ? FUZZY_AMBIGUOUS : FUZZY_SUCCESS;
	}

fuzzy_file_match_return:
	node_free_data(&files);
	string_destroy(&basedata);
	string_destroy(&dirdata);
	string_destroy(&file_noext);
	return ret;
}

int _resolve_file(struct fql_handle* fql, query* query, table* table)
{
	if (table->is_stdin) {
		return FQL_GOOD;
	}

	switch (_fuzzy_resolve_file(&table->reader->file_name,
	                            &table->name,
	                            fql->props.strictness)) {
	case FUZZY_AMBIGUOUS:
		fprintf(stderr,
		        "Table name ambiguous: %s\n",
		        string_c_str(&table->reader->file_name));
		return FQL_FAIL;
	case FUZZY_NOTFOUND:
		fprintf(stderr,
		        "Table not resolved: %s\n",
		        string_c_str(&table->reader->file_name));
		return FQL_FAIL;
	default:;
	}

	char absolute_path[PATH_MAX];
	if (!realpath(string_c_str(&table->reader->file_name), absolute_path)) {
		perror(string_c_str(&table->reader->file_name));
		return FQL_FAIL;
	}

	schema** match = hashmap_get(fql->schema_map, absolute_path);
	if (match == NULL) {
		return FQL_GOOD;
	}

	vec* src_vec = (*match)->expressions;

	expression** it = vec_begin(src_vec);
	for (; it != vec_end(src_vec); ++it) {
		expression* new_expr =
		        new_(expression, EXPR_COLUMN_NAME, (*it)->alias.data, "");
		new_expr->field_type = (*it)->field_type;
		new_expr->location = (*it)->location;
		new_expr->width = (*it)->width;
		schema_add_expression(table->schema, new_expr, table->idx);
	}

	table->must_reopen = true;
	table->schema->is_preresolved = true;
	table->reader->type = (*match)->write_io_type;
	table->schema->is_default = (*match)->is_default;
	return FQL_GOOD;
}

void schema_assign_header(table* table, record* rec, int src_idx)
{
	//int i = 0;
	stringview* it = vec_begin(&rec->fields);
	for (; it != vec_end(&rec->fields); ++it) {
		string expr_str;
		string_construct_from_stringview(&expr_str, it);
		expression* new_expr =
		        new_(expression, EXPR_COLUMN_NAME, expr_str.data, "");
		schema_add_expression(table->schema, new_expr, src_idx);

		//new_expr->index = i++;
		//new_expr->table = table;
		//new_expr->src_idx = table->idx;
		new_expr->field_type = FIELD_STRING;

		string_destroy(&expr_str);
	}

	schema_preflight(table->schema);
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
	//new_expr->table = table;
	new_expr->src_idx = table->idx;
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

		/* we ARE defining an expression */
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
	string* full_path_temp = new_(string);
	enum _fuzzy_return ret = FUZZY_NOTFOUND;

	string** path_iter = vec_begin(fql->schema_paths);
	for (; path_iter != vec_end(fql->schema_paths) && ret == FUZZY_NOTFOUND;
	     ++path_iter) {
		string_sprintf(full_path_temp,
		               "%s/%s",
		               string_c_str(*path_iter),
		               string_c_str(self->name));

		ret = _fuzzy_resolve_file(self->schema_path,
		                          full_path_temp,
		                          fql->props.strictness);
		if (ret == FUZZY_AMBIGUOUS) {
			fprintf(stderr,
			        "schema name `%s' ambiguous in directory `%s'\n",
			        string_c_str(self->name),
			        string_c_str(*path_iter));
			return FQL_FAIL;
		}
	}

	delete_(string, full_path_temp);

	if (ret == FUZZY_NOTFOUND) {
		fprintf(stderr,
		        "schema name `%s' could not be found\n",
		        string_c_str(self->name));
		return FQL_FAIL;
	}

	try_(_parse_schema_file(table, fql, src_idx));

	return FQL_GOOD;
}

int _resolve_source(struct fql_handle* fql, query* query, table* table, int src_idx)
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
		if (string_empty(&table->alias)) {
			fputs("subquery sources require an alias\n", stderr);
			return FQL_FAIL;
		}
		try_(_resolve_query(fql, table->subquery, IO_UNDEFINED));
		fqlselect* select = table->subquery->op;
		table->schema = select->schema;
		self = table->schema;
		self->is_preresolved = true;
		table->reader->type = IO_SUBQUERY;
	} else {
		try_(_resolve_file(fql, query, table));
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

	node rg = {0};
	record* rec = new_(record, 0);
	rg.data = rec;

	table->reader->max_idx = INT_MAX;
	table->reader->get_record__(table->reader, &rg);
	table->reader->max_idx = 0;

	if (!table->is_stdin) {
		table->reader->reset__(table->reader);
	}

	/* if we've made it this far, we want to try
	 * and determine schema by reading the top
	 * row of the file and assume a delimited
	 * list of field names.
	 */
	if (self->is_default) {
		if (!self->is_preresolved) {
			schema_assign_header(table, rec, src_idx);
		}
	} else {
		unsigned newsize = (rec->fields.size) ? rec->fields.size : 1;
		if (newsize < self->expressions->size) {
			expression** it = vec_at(self->expressions, newsize);
			for (; it != vec_end(self->expressions); ++it) {
				delete_(expression, *it);
			}
			vec_resize(self->expressions, newsize);
		}
	}

	schema_preflight(self);

	if (self->is_default || !table->is_stdin) {
		delete_(record, rec);
	} else {
		table->reader->first_rec = rec;
	}

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

int _resolve_asterisk(vec* expressions, vec* sources)
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
			        string_c_str(&(*expr)->table_name));
			return FQL_FAIL;
		}
	}

	return FQL_GOOD;
}

enum join_side _get_join_side(expression* expr, int right_idx)
{
	switch (expr->expr) {
	case EXPR_FULL_RECORD:
		return (expr->src_idx < right_idx) ? SIDE_LEFT : SIDE_RIGHT;
	case EXPR_COLUMN_NAME:
		if (expr->subquery_src_idx == -1) {
			return (expr->data_source->src_idx < right_idx) ? SIDE_LEFT
			                                                : SIDE_RIGHT;
		} else {
			return (expr->subquery_src_idx < right_idx) ? SIDE_LEFT
			                                            : SIDE_RIGHT;
		}
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
				hj->comp_type =
				        field_determine_type(hj->right_expr->field_type,
				                             hj->left_expr->field_type);
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
	MAP_ASTERISK,
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
	static const enum _expr_type _expr_aster = MAP_ASTERISK;
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
			stringview_nset(&val_sv, &expr->field.f, sizeof(double));
			vec_push_back(key, &val_sv);
			break;
		case FIELD_FLOAT:
			type_sv.data = (void*)&_c_float;
			vec_push_back(key, &type_sv);
			stringview_nset(&val_sv, &expr->field.f, sizeof(double));
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
		stringview_nset(&val_sv, &expr->data_source, sizeof(expression*));
		vec_push_back(key, &val_sv);
		break;
	case EXPR_FUNCTION: {
		type_sv.data = (void*)&_func;
		vec_push_back(key, &type_sv);
		stringview_nset(&val_sv,
		                &expr->field.fn->type,
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
		                &expr->field.agg->agg_type,
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
		stringview_nset(&val_sv, &expr->subquery, sizeof(void*));
		vec_push_back(key, &val_sv);
		break;
	case EXPR_ASTERISK:
		type_sv.data = (void*)&_expr_aster;
		vec_push_back(key, &type_sv);
		stringview_nset(&val_sv, &expr->src_idx, sizeof(expr->src_idx));
		vec_push_back(key, &val_sv);
		break;
	default:
		fputs("unexpected expression mapping\n", stderr);
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

	fprintf(stderr, "`%s' unexpected expression\n", (char*)expr->alias.data);
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
		if ((*it)->expr == EXPR_REFERENCE) {
			expression** matched = vec_at(op_exprs, (*it)->index);
			(*it)->src_idx = 0;
			(*it)->field_type = (*matched)->field_type;
			(*it)->data_source = *matched;
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

	unsigned expected = fqlselect_get_field_count(aquery->op, aquery->sources);
	query** query_iter = vec_begin(aquery->unions);
	for (; query_iter != vec_end(aquery->unions); ++query_iter) {
		try_(_resolve_query(fql, *query_iter, main_io_type));
		if (expected
		    != fqlselect_get_field_count((*query_iter)->op,
		                                 (*query_iter)->sources)) {
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
		try_(_resolve_source(fql, aquery, table, i));
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
	if (*(enum op*)aquery->op == OP_SELECT) {
		try_(_resolve_asterisk(op_exprs, sources));
	}
	try_(_assign_expressions(op_exprs, sources, fql->props.strictness));

	vec* op_add_exprs = op_get_additional_exprs(aquery->op);
	if (op_add_exprs != NULL) {
		try_(_assign_expressions(op_add_exprs, sources, fql->props.strictness));
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
		try_(order_preresolve_expressions(aquery->orderby,
		                                  aquery->op,
		                                  aquery->sources));
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
	}

	if (!vec_empty(aquery->unions)) {
		try_(_resolve_unions(fql, aquery));
	}

	try_(op_writer_init(aquery, fql));

	if (aquery->groupby == NULL && aquery->orderby) {
		/* This is normally handled in _op_find_group,
		 * but if there is no GROUP BY, just assign the
		 * pre-resolved ORDER BY expressions.
		 */
		expression** it = vec_begin(order_exprs);
		for (; it != vec_end(order_exprs); ++it) {
			if ((*it)->expr != EXPR_REFERENCE) {
				continue;
			}
			expression** matched = vec_at(op_exprs, (*it)->index);
			(*it)->data_source = *matched;
			(*it)->field_type = (*matched)->field_type;
		}
	}

	schema_preflight(op_schema);

	/* If this query will be writing changes to the file system,
	 * we need to be aware of this when parsing future queries.
	 * These are mapped as absolute paths. First check that the
	 * file exists first. If it doesn't, create it now so that
	 * realpath works. Creating the file also has the affect of
	 * making fuzzy file discovery possible on a file that did
	 * not previously exist.
	 */
	if (aquery->into_table_name != NULL) {
		if (access(aquery->into_table_name, F_OK) != 0) {
			/* NOTE: that we may enter this block for a number of
			 *       reasons other than "file doesn't exist." We
			 *       rely on fopen errors to catch other errors.
			 */
			FILE* place_holder = fopen(aquery->into_table_name, "w");
			if (place_holder == NULL) {
				perror(aquery->into_table_name);
				return FQL_FAIL;
			}

			if (fclose(place_holder) == EOF) {
				perror(aquery->into_table_name);
				return FQL_FAIL;
			}
		}

		char absolute_path[PATH_MAX];
		if (!realpath(aquery->into_table_name, absolute_path)) {
			perror(aquery->into_table_name);
			return FQL_FAIL;
		}

		hashmap_set(fql->schema_map, absolute_path, &op_schema);
	}

	return FQL_GOOD;
}

int _add_schema_path(struct fql_handle* fql, string* schema_path)
{
	DIR* schema_dir = opendir(string_c_str(schema_path));
	if (!schema_dir) {
		delete_(string, schema_path);
		return FQL_FAIL;
	}

	closedir(schema_dir);

	vec_push_back(fql->schema_paths, &schema_path);

	return FQL_GOOD;
}

/* Schema path is determined by the following hierarchy:
 *  1. fql->props.schema_path
 *  2. FQL_SCHEMA_PATH environment variable
 *  3. $HOME/.config/fql/schema/
 *  4. $datarootdir/fql/schema/
 */
int _resolve_schema_paths(struct fql_handle* fql)
{
	if (fql->schema_paths != NULL) {
		return FQL_GOOD;
	}

	fql->schema_paths = new_t_(vec, string*);

	/* Add --schema-path */
	if (!string_empty(fql->props.schema_path)) {
		string* schema_path = new_(string);
		string_copy(schema_path, fql->props.schema_path);
		if (_add_schema_path(fql, schema_path)) {
			fprintf(stderr,
			        "schema path defined by variable "
			        "props.schema_path (%s) is invalid\n",
			        string_c_str(fql->props.schema_path));
			return FQL_FAIL;
		}
	}

	/* Add FQL_SCHEMA_PATH */
	const char* path_from_env = getenv("FQL_SCHEMA_PATH");
	if (path_from_env != NULL) {
		string* schema_path = string_from_char_ptr(path_from_env);
		if (_add_schema_path(fql, schema_path)) {
			fprintf(stderr,
			        "schema path defined by environment variable "
			        "FQL_SCHEMA_PATH (%s) is invalid\n",
			        path_from_env);
			return FQL_FAIL;
		}
	}

	/* NOTE: We do not check the return codes for ~/.config/fql/schema
	 *       or ${datarootdir}/fql/schema because, we should not error
	 *       out if they don't exist.
	 */

	/* Add ~/.config/fql/schema */
	const char* home_path = getenv("HOME");
	if (home_path != NULL) {
		string* schema_path = new_(string);
		string_sprintf(schema_path, "%s/.config/fql/schema", home_path);
		_add_schema_path(fql, schema_path);
	}

	/* add ${datarootdir}/fql/schema/ */
	string* schema_path = string_from_char_ptr(quote_macro_(SCHEMA_DATADIR));
	_add_schema_path(fql, schema_path);

	return FQL_GOOD;
}

int schema_resolve(struct fql_handle* fql)
{
	try_(_resolve_schema_paths(fql));

	query** it = vec_begin(fql->query_vec);
	for (; it != vec_end(fql->query_vec); ++it) {
		if (fql->_out_delim_set) {
			op_set_delim((*it)->op, fql->props.out_delim);
		}

		if (fql->props.rec_terminator[0]) {
			op_set_rec_terminator((*it)->op, fql->props.rec_terminator);
		}

		try_(_resolve_query(fql, *it, IO_UNDEFINED));
	}

	return FQL_GOOD;
}
