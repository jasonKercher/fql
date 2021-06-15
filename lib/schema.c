#include "schema.h"

#include <dirent.h>
#include <csv.h>

#include "fql.h"
#include "misc.h"
#include "query.h"
#include "table.h"
#include "group.h"
#include "order.h"
#include "logic.h"
#include "reader.h"
#include "column.h"
#include "function.h"
#include "aggregate.h"
#include "fqlselect.h"
#include "operation.h"
#include "util/util.h"
#include "util/vec.h"

schema* schema_construct(schema* self)
{
	*self = (schema) {
	        new_t_(vec, column*), /* columns */
	        NULL,                 /* col_map */
	        "",                   /* name */
	        "",                   /* delimiter */
	        0,                    /* strictness */
	};

	return self;
}

void schema_destroy(void* generic_schema)
{
	schema* self = generic_schema;

	column** it = vec_begin(self->columns);
	for (; it != vec_end(self->columns); ++it) {
		delete_(column, *it);
	}

	delete_(vec, self->columns);
	if (self->col_map != NULL) {
		delete_(multimap, self->col_map);
	}
}

void schema_add_column(schema* self, column* col)
{
	vec_push_back(self->columns, &col);
}

void schema_apply_column_alias(schema* self, const char* alias)
{
	column** col = vec_back(self->columns);
	string_strcpy(&(*col)->alias, alias);
}

void schema_preflight(schema* self)
{
	/* this is a separate step because between adding
	 * a new column and _here_, we could have applied a
	 * new alias to the column. we don't care about a
	 * column name once it has an alias.
	 */
	self->col_map = new_t_(multimap,
	                       column*,
	                       self->columns->size * 2,
	                       HASHMAP_PROP_NOCASE);

	unsigned i = 0;
	column** col = vec_begin(self->columns);
	for (; i < self->columns->size; ++i) {
		col[i]->location = i;
		multimap_set(self->col_map, col[i]->alias.data, &col[i]);
	}

	if (self->delimiter[0] == '\0') {
		self->delimiter[0] = ',';
		self->delimiter[1] = '\0';
	}
}

int schema_resolve_file(table* table, int strictness)
{
	if (table->source_type == SOURCE_SUBQUERY) {
		return FQL_GOOD;
	}
	string table_name_base;
	string table_name_dir;
	string file_noext;

	string_construct(&table_name_base);
	string_construct(&table_name_dir);
	string_construct(&file_noext);

	string_copy(&table_name_base, &table->name);
	string_copy(&table_name_dir, &table->name);

	char* dir = dirname(table_name_dir.data);
	char* base = basename(table_name_base.data);

	queue* files = dir_list_files(dir);
	queue* node = files;

	int matches = 0;
	/* match exact */
	for (; node; node = node->next) {
		if (string_eq(node->data, base)) {
			++matches;
			string_sprintf(&table->reader->file_name,
			               "%s/%s",
			               dir,
			               node->data);
		}
	}

	if (matches) {
		goto success_return;
	}

	if (strictness) {
		goto fail_return;
	}

	/* match exact ignoring case */
	for (node = files; node; node = node->next) {
		if (istring_eq(node->data, base)) {
			++matches;
			string_sprintf(&table->reader->file_name,
			               "%s/%s",
			               dir,
			               node->data);
		}
	}

	if (matches > 1) {
		fprintf(stderr,
		        "%s: ambiguous file name\n",
		        (char*)table->name.data);
		return FQL_FAIL;
	} else if (matches) {
		goto success_return;
	}

	/* match file without extension */
	for (node = files; node; node = node->next) {
		string_resize(&file_noext, strlen(node->data));
		getnoext(file_noext.data, node->data);
		if (string_eq(file_noext.data, base)) {
			++matches;
			string_sprintf(&table->reader->file_name,
			               "%s/%s",
			               dir,
			               node->data);
		}
	}

	if (matches > 1) {
		fprintf(stderr,
		        "%s: ambiguous file name\n",
		        (char*)table->name.data);
		return FQL_FAIL;
	} else if (matches) {
		goto success_return;
	}

	/* match file without extension ignoring case */
	for (node = files; node; node = node->next) {
		string_resize(&file_noext, strlen(node->data));
		getnoext(file_noext.data, node->data);
		if (istring_eq(file_noext.data, base)) {
			++matches;
			string_sprintf(&table->reader->file_name,
			               "%s/%s",
			               dir,
			               node->data);
		}
	}

	if (matches > 1) {
		fprintf(stderr,
		        "%s: ambiguous file name\n",
		        (char*)table->name.data);
		return FQL_FAIL;
	} else if (matches) {
		goto success_return;
	}

fail_return:
	fprintf(stderr,
	        "%s: unable to find matching file\n",
	        (char*)table->name.data);
	return FQL_FAIL;

success_return:
	queue_free_data(&files);
	string_destroy(&table_name_base);
	string_destroy(&table_name_dir);
	string_destroy(&file_noext);
	return FQL_GOOD;
}

void schema_assign_header(table* table, record* rec, int src_idx)
{
	int i = 0;
	stringview* it = vec_begin(rec->fields);
	for (; it != vec_end(rec->fields); ++it) {
		string col_str;
		string_construct_from_stringview(&col_str, it);
		column* new_col =
		        new_(column, EXPR_COLUMN_NAME, col_str.data, "");
		schema_add_column(table->schema, new_col);

		new_col->src_idx = src_idx;
		new_col->location = i++;
		new_col->table = table;
		new_col->field_type = FIELD_STRING;

		string_destroy(&col_str);
	}

	schema_preflight(table->schema);
}

int schema_resolve_source(struct fql_handle* fql, table* table, int src_idx)
{
	if (!vec_empty(table->schema->columns)) {
		return FQL_GOOD; /* self already set */
	}

	if (table->schema->name[0]) {
		fputs("not loading schema by name yet\n", stderr);
		return FQL_FAIL; /* TODO: load self by name */
	}

	if (table->source_type == SOURCE_SUBQUERY) {
		schema_resolve_query(fql, table->subquery);
		fqlselect* select = table->subquery->op;
		table->schema = select->schema;
		table->reader->type = READ_SUBQUERY;
	} else {
		/* if we've made it this far, we want to try
		 * and determine schema by reading the top
		 * row of the file and assume a delimited
		 * list of field names.
		 */
		try_(schema_resolve_file(table, fql->props.strictness));
		table->reader->type = READ_LIBCSV;
	}
	reader_assign(table->reader, table);

	/* skip here if we *now* know self */
	if (!vec_empty(table->schema->columns)) {
		return FQL_GOOD;
	}

	record rec;
	record_construct(&rec, 0, 0, false);
	table->reader->max_col_idx = INT_MAX;
	table->reader->get_record__(table->reader, &rec);
	const char* delim = table_get_delim(table);
	strncpy_(table->schema->delimiter, delim, DELIM_LEN_MAX);
	table->reader->max_col_idx = 0;

	schema_assign_header(table, &rec, src_idx);
	record_destroy(&rec);

	return FQL_GOOD;
}

int _evaluate_if_const(column* col)
{
	function* func = col->field.fn;
	column** it = vec_begin(func->args);
	for (; it != vec_end(func->args); ++it) {
		if ((*it)->expr != EXPR_CONST) {
			return FQL_GOOD;
		}
	}

	union field new_field;
	if (col->field_type == FIELD_STRING) {
		//new_field.s = &func->ret_buf;
		new_field.s = &col->buf;
		string_clear(new_field.s);
	}
	try_(func->call__(func, &new_field, NULL));

	delete_(function, func);
	col->expr = EXPR_CONST;
	col->field = new_field;

	return FQL_GOOD;
}

int _assign_columns_limited(vec* columns,
                            vec* sources,
                            int limit,
                            int strictness)
{
	column** it = vec_begin(columns);
	for (; it != vec_end(columns); ++it) {
		switch ((*it)->expr) {
		case EXPR_FUNCTION: {
			function* func = (*it)->field.fn;
			try_(_assign_columns_limited(func->args,
			                             sources,
			                             limit,
			                             strictness));
			try_(function_op_resolve(func, &(*it)->field_type));
			try_(function_validate(func));
			try_(_evaluate_if_const(*it));
			break;
		}
		case EXPR_SUBQUERY:
			try_(fqlselect_resolve_type_from_subquery(*it));
			break;
		case EXPR_COLUMN_NAME: {
			if ((*it)->data_source != NULL) {
				continue;
			}

			int matches = 0;
			int j = 0;

			for (; j <= limit; ++j) {
				table* search_table = vec_at(sources, j);
				if (string_empty(&(*it)->table_name)
				    || istring_eq((*it)->table_name.data,
				                  search_table->alias.data)) {
					int n = column_try_assign_source(
					        *it,
					        search_table);
					if (n > 1 && !strictness) {
						n = 1;
					}
					matches += n;
				}
			}

			if (matches > 1) {
				fprintf(stderr,
				        "%s: ambiguous column\n",
				        (char*)(*it)->name.data);
				return FQL_FAIL;
			}

			if (matches == 0) {
				fprintf(stderr,
				        "%s: cannot find column\n",
				        (char*)(*it)->name.data);
				return FQL_FAIL;
			}
		}
		default:;
		}
	}

	return FQL_GOOD;
}

int _assign_columns(vec* columns, vec* sources, int strictness)
{
	return _assign_columns_limited(columns,
	                               sources,
	                               sources->size - 1,
	                               strictness);
}

int _asterisk_resolve(vec* columns, vec* sources)
{
	unsigned i = 0;
	for (; i < columns->size; ++i) {
		int col_idx = i;
		column** col = vec_at(columns, i);
		if ((*col)->expr != EXPR_ASTERISK) {
			continue;
		}
		int matches = 0;
		unsigned j = 0;
		for (; j < sources->size; ++j) {
			table* search_table = vec_at(sources, j);
			if (string_empty(&(*col)->table_name)
			    || istring_eq((*col)->table_name.data,
			                  search_table->alias.data)) {
				if (matches > 0) {
					column* new_col = new_(column,
					                       EXPR_ASTERISK,
					                       NULL,
					                       "");
					new_col->src_idx = j;
					vec_insert_at(columns,
					              ++i,
					              &new_col,
					              1);
					col = vec_at(columns, col_idx);
				} else {
					(*col)->src_idx = j;
				}
				++matches;
			}
		}

		if (matches == 0) {
			fprintf(stderr,
			        "could not locate table `%s'\n",
			        (char*)(*col)->table_name.data);
			return FQL_FAIL;
		}
	}

	return FQL_GOOD;
}

enum join_side _get_join_side(column* col, int right_idx)
{
	switch (col->expr) {
	case EXPR_COLUMN_NAME:
		return (col->src_idx < right_idx) ? SIDE_LEFT : SIDE_RIGHT;
	case EXPR_FUNCTION: {
		function* func = col->field.fn;
		column** it = vec_begin(func->args);
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
	if (right_table->condition == NULL
	    || right_table->condition->joinable == NULL) {
		return;
	}

	vec* joinable = right_table->condition->joinable;

	logic** it = vec_begin(joinable);
	for (; it != vec_end(joinable); ++it) {
		enum join_side side0 = _get_join_side((*it)->col[0], right_idx);
		if (side0 == SIDE_MIXED) {
			continue;
		}
		enum join_side side1 = _get_join_side((*it)->col[1], right_idx);
		if (side0 != side1) {
			if (!logic_can_be_false(right_table->condition, *it)) {
				right_table->condition->join_logic = *it;
				hashjoin* hj = new_(hashjoin);
				if (side0 == SIDE_RIGHT) {
					hj->right_col = (*it)->col[0];
					hj->left_col = (*it)->col[1];
				} else {
					hj->right_col = (*it)->col[1];
					hj->left_col = (*it)->col[0];
				}
				right_table->join_data = hj;
				break;
			}
		}
	}
}

enum _expr_type {
	MAP_UNDEFINED,
	MAP_CONST_INT,
	MAP_CONST_FLOAT,
	MAP_CONST_STRING,
	MAP_COLUMN,
	MAP_FUNCTION,
	MAP_AGGREGATE,
	MAP_SUBQUERY,
};

int _map_expression(vec* key, column* col)
{
	/* I mean... heh? */
	static const enum _expr_type _undef = MAP_UNDEFINED;
	static const enum _expr_type _c_int = MAP_CONST_INT;
	static const enum _expr_type _c_float = MAP_CONST_FLOAT;
	static const enum _expr_type _c_string = MAP_CONST_STRING;
	static const enum _expr_type _col = MAP_COLUMN;
	static const enum _expr_type _func = MAP_FUNCTION;
	static const enum _expr_type _agg = MAP_AGGREGATE;
	static const enum _expr_type _sub = MAP_SUBQUERY;

	const enum _expr_type* map_type = &_undef;
	stringview type_sv;
	stringview val_sv;
	switch (col->expr) {
	case EXPR_CONST:
		switch (col->field_type) {
		case FIELD_INT:
			map_type = &_c_int;
			stringview_nset(&val_sv,
			                (char*)&col->field.f,
			                sizeof(double));
			break;
		case FIELD_FLOAT:
			map_type = &_c_float;
			stringview_nset(&val_sv,
			                (char*)&col->field.f,
			                sizeof(double));
			break;
		case FIELD_STRING:
			map_type = &_c_string;
			stringview_set_string(&val_sv, col->field.s);
		default:;
		}

		break;
	case EXPR_COLUMN_NAME:
		map_type = &_col;
		stringview_nset(&val_sv,
		                (char*)&col->data_source,
		                sizeof(column*));
		break;
	case EXPR_FUNCTION:
		map_type = &_func;
		stringview_nset(&val_sv,
		                (char*)&col->field.fn->type,
		                sizeof(enum scalar_function));
		break;
	case EXPR_AGGREGATE:
		map_type = &_agg;
		stringview_nset(&val_sv,
		                (char*)&col->field.agg->agg_type,
		                sizeof(enum aggregate_function));
		break;
	/* Maybe let's not group by a subquery expression?? */
	case EXPR_SUBQUERY:
		map_type = &_sub;
		stringview_nset(&val_sv, (char*)&col->subquery, sizeof(void*));
		break;
	default:
		fputs("unexpected expression\n", stderr);
		return FQL_FAIL;
	}

	stringview_nset(&type_sv, (char*)map_type, sizeof(enum _expr_type));

	vec_push_back(key, &type_sv);
	vec_push_back(key, &val_sv);

	if (*map_type != MAP_FUNCTION) {
		return FQL_GOOD;
	}

	column** it = vec_begin(col->field.fn->args);
	for (; it != vec_end(col->field.fn->args); ++it) {
		try_(_map_expression(key, *it));
	}

	return FQL_GOOD;
}

int _op_find_group(compositemap* expr_map, column* col, vec* key)
{
	if (col->expr == EXPR_CONST || col->expr == EXPR_SUBQUERY) {
		return FQL_GOOD;
	}

	if (col->expr == EXPR_AGGREGATE) {
		col->src_idx = 0;
		col->field_type = col->data_source->field_type;
		return FQL_GOOD;
	}

	vec_clear(key);
	try_(_map_expression(key, col));

	column** result = compositemap_get(expr_map, key);
	if (result != NULL) {
		col->src_idx = 0;
		col->data_source = *result;
		col->is_resolved_to_group = true;
		/* the operation (select) will not be evaluating
		 * the expression.  it is simply going to read
		 * from the grouping as if it were a column.
		 */
		col->expr = EXPR_COLUMN_NAME;
		return FQL_GOOD;
	}

	/* TODO: this logic */
	if (col->expr == EXPR_COLUMN_NAME) {
		fprintf(stderr,
		        "column `%s' does not match a grouping\n",
		        (char*)col->alias.data);
		return FQL_FAIL;
	} else if (col->expr != EXPR_FUNCTION) {
		fprintf(stderr,
		        "column `%s' unexpected expression\n",
		        (char*)col->alias.data);
		return FQL_FAIL;
	}

	column** it = vec_begin(col->field.fn->args);
	for (; it != vec_end(col->field.fn->args); ++it) {
		try_(_op_find_group(expr_map, *it, key));
	}
	return FQL_GOOD;
}

int _map_groups(struct fql_handle* fql, query* query)
{
	/* verify group columns and build composite key for each */
	vec* group_cols = &query->groupby->columns;
	try_(_assign_columns(group_cols,
	                     query->sources,
	                     fql->props.strictness));

	compositemap* expr_map =
	        new_t_(compositemap,
	               column*,
	               group_cols->size * 2,
	               HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM);
	query->groupby->expr_map = expr_map;
	vec key;
	vec_construct_(&key, stringview);

	column** it = vec_begin(group_cols);
	for (; it != vec_end(group_cols); ++it) {
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
		try_(_assign_columns(agg->args,
		                     query->sources,
		                     fql->props.strictness));
		try_(aggregate_resolve(agg, *it));
		(*it)->field_type = agg->data_type;
		agg->linked_column->field_type = agg->data_type;
	}

	vec_destroy(&key);
	return FQL_GOOD;
}

int _group_validation(query* query, vec* cols)
{
	compositemap* expr_map = query->groupby->expr_map;
	vec key;
	vec_construct_(&key, stringview);
	column** it = vec_begin(cols);
	for (; it != vec_end(cols); ++it) {
		if ((*it)->is_resolved_to_group) {
			continue;
		}
		if (_op_find_group(expr_map, *it, &key)) {
			vec_destroy(&key);
			return FQL_FAIL;
		}
	}

	vec_destroy(&key);
	return FQL_GOOD;
}

int schema_resolve_query(struct fql_handle* fql, query* aquery)
{
	vec* subqueries = aquery->subquery_const_vec;
	query** it = vec_begin(subqueries);
	for (; it != vec_end(subqueries); ++it) {
		schema_resolve_query(fql, *it);
	}

	vec* sources = aquery->sources;

	if (aquery->top_expr != NULL) {
		vec temp;
		vec_construct_(&temp, column*);
		vec_push_back(&temp, &aquery->top_expr);
		_assign_columns_limited(&temp, NULL, 0, fql->props.strictness);
		vec_destroy(&temp);
		if (aquery->top_expr->expr != EXPR_CONST) {
			fputs("Could not resolve top expression\n", stderr);
			return FQL_FAIL;
		}
		if (aquery->top_expr->field_type != FIELD_INT) {
			fputs("Input to TOP clause must be an integer\n",
			      stderr);
			return FQL_FAIL;
		}
		if (aquery->top_expr->field.i < 0) {
			fputs("Input to TOP clause cannot be negative\n",
			      stderr);
			return FQL_FAIL;
		}
		aquery->top_count = aquery->top_expr->field.i;
		delete_(column, aquery->top_expr);
	}

	/* Oh this is fun.  Let's try to link and verify
	 * _everything_. First, let's verify the sources
	 * exist and populate schemas.  As we loop, we
	 * resolve the columns that are listed in join
	 * clauses because of the following caveat:
	 *
	 * SELECT *
	 * FROM T1
	 * JOIN T2 ON T1.FOO = T3.FOO -- Cannot read T3 yet!
	 * JOIN T3 ON T2.FOO = T3.FOO
	 */
	unsigned i = 0;
	for (; i < sources->size; ++i) {
		table* table = vec_at(aquery->sources, i);
		try_(schema_resolve_source(fql, table, i));

		if (i == 0 && !op_has_delim(aquery->op)) {
			op_set_delim(aquery->op, table->schema->delimiter);
		}

		/* Validate columns used in JOIN clauses */
		if (table->condition != NULL) {
			try_(_assign_columns_limited(table->condition->columns,
			                             sources,
			                             i,
			                             fql->props.strictness));
		}

		if (i > 0 && !fql->props.force_cartesian) {
			_resolve_join_conditions(table, i);
		}
	}

	/* Validate WHERE columns */
	if (aquery->where != NULL) {
		try_(_assign_columns(aquery->where->columns,
		                     aquery->sources,
		                     fql->props.strictness));
	}

	/* Validate the columns from the operation.
	 * (e.g. columns listed in SELECT).
	 */
	vec* op_cols = op_get_validation_list(aquery->op);
	try_(_asterisk_resolve(op_cols, sources));
	try_(_assign_columns(op_cols, sources, fql->props.strictness));

	/* Validate HAVING columns */
	vec* having_cols = NULL;
	if (aquery->having != NULL) {
		having_cols = aquery->having->columns;
		try_(_assign_columns(having_cols,
		                     aquery->sources,
		                     fql->props.strictness));
	}

	/* Validate ORDER BY columns if they exist */
	vec* order_cols = NULL;
	if (aquery->orderby) {
		order_cols = &aquery->orderby->columns;
		try_(order_preresolve_columns(aquery->orderby, aquery->op));
		try_(_assign_columns(order_cols,
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
		 * HAVING and ORDER BY column to a group.
		 */
		try_(_group_validation(aquery, op_cols));

		if (having_cols) {
			try_(_group_validation(aquery, having_cols));
		}

		/* exceptions: ordinal ordering or matched by alias */
		if (order_cols) {
			try_(_group_validation(aquery, order_cols));
		}
	}

	op_preflight(aquery);

	return FQL_GOOD;
}

int schema_resolve(struct fql_handle* fql)
{
	queue* query_node = fql->query_list;
	for (; query_node; query_node = query_node->next) {
		query* query = query_node->data;

		if (fql->props.out_delim[0]) {
			op_set_delim(query->op, fql->props.out_delim);
		}

		try_(schema_resolve_query(fql, query));
	}

	return FQL_GOOD;
}
