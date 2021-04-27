#include "schema.h"

#include <dirent.h>
#include <csv.h>

#include "fql.h"
#include "column.h"
#include "function.h"
#include "query.h"
#include "select.h"
#include "table.h"
#include "reader.h"
#include "operation.h"
#include "util/util.h"
#include "util/vec.h"

Schema* schema_new()
{
	Schema* new_schema = NULL;
	malloc_(new_schema, sizeof(*new_schema));

	return schema_construct(new_schema);
}

Schema* schema_construct(Schema* schema)
{
	*schema = (Schema) {
		 vec_new_(Column*)      /* columns */
		,NULL                   /* col_map */
		,""                     /* name */
		,""                     /* delimiter */
	};

	return schema;
}

void schema_free(void* generic_schema)
{
	Schema* schema = generic_schema;

	Column** it = vec_begin(schema->columns);
	for (; it != vec_end(schema->columns); ++it) {
		column_free(*it);
	}

	vec_free(schema->columns);
	if (schema->col_map != NULL) {
		hashmap_free(schema->col_map);
	}
	free_(schema);
}

void schema_add_column(Schema* schema, Column* col)
{
	vec_push_back(schema->columns, &col);
}

void schema_apply_column_alias(Schema* schema, const char* alias)
{
	Column** col = vec_back(schema->columns);
	string_strcpy(&(*col)->alias, alias);
}

void schema_finalize(Schema* schema)
{
	/* This is a separate step because between adding
	 * a new column and _here_, we could have applied a
	 * new alias to the column. We don't care about a
	 * column name once it has an alias.
	 */
	schema->col_map = hashmap_new_(Column*,
	                               schema->columns->size * 2,
	                               HASHMAP_PROP_NOCASE);

	int i = 0;
	Column** col = vec_begin(schema->columns);
	for (; i < schema->columns->size; ++i) {
		col[i]->location = i;
		hashmap_set(schema->col_map, col[i]->alias.data, &col[i]);
	}

	if (schema->delimiter[0] == '\0') {
		schema->delimiter[0] = ',';
		schema->delimiter[1] = '\0';
	}
}

int schema_resolve_file(Table* table)
{
	if (table->source_type == SOURCE_SUBQUERY) {
		return FQL_GOOD;
	}
	String table_name_base;
	String table_name_dir;
	String file_noext;

	string_construct(&table_name_base);
	string_construct(&table_name_dir);
	string_construct(&file_noext);

	string_copy(&table_name_base, &table->name);
	string_copy(&table_name_dir, &table->name);

	char* dir = dirname(table_name_dir.data);
	char* base = basename(table_name_base.data);

	Queue* files = dir_list_files(dir);
	Queue* node = files;

	int matches = 0;
	/* Match exact */
	for (; node; node = node->next) {
		if (string_eq(node->data, base)) {
			++matches;
			string_sprintf(&table->reader->file_name,
				       "%s/%s", dir, node->data);
		}
	}

	if (matches) {
		goto success_return;
	}

	/* Match exact ignoring case */
	for (node = files; node; node = node->next) {
		if (istring_eq(node->data, base)) {
			++matches;
			string_sprintf(&table->reader->file_name,
				       "%s/%s", dir, node->data);
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

	/* Match file without extension */
	for (node = files; node; node = node->next) {
		string_resize(&file_noext, strlen(node->data));
		getnoext(file_noext.data, node->data);
		if (string_eq(file_noext.data, base)) {
			++matches;
			string_sprintf(&table->reader->file_name,
				       "%s/%s", dir, node->data);
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

	/* Match file without extension ignoring case */
	for (node = files; node; node = node->next) {
		string_resize(&file_noext, strlen(node->data));
		getnoext(file_noext.data, node->data);
		if (istring_eq(file_noext.data, base)) {
			++matches;
			string_sprintf(&table->reader->file_name,
				       "%s/%s", dir, node->data);
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

void schema_assign_header(Table* table, Record* rec)
{
	int i = 0;
	StringView* it = vec_begin(rec->fields);
	for (; it != vec_end(rec->fields); ++it) {
		String col_str;
		string_construct_from_stringview(&col_str, it);
		Column* new_col = column_new(EXPR_COLUMN_NAME, col_str.data, "");
		schema_add_column(table->schema, new_col);

		new_col->location = i++;
		new_col->table = table;
		new_col->field_type = FIELD_STRING;

		string_destroy(&col_str);
	}

	schema_finalize(table->schema);
}

int schema_resolve_source(struct fql_handle* fql, Table* table)
{
	if (!vec_empty(table->schema->columns)) {
		return FQL_GOOD;  /* Schema already set */
	}

	if (table->schema->name[0]) {
		fputs("Not loading schema by name yet\n", stderr);
		return FQL_FAIL;  /* TODO: load schema by name */
	}

	if (table->source_type == SOURCE_SUBQUERY) {
		schema_resolve_query(fql, table->subquery);
		Select* select = table->subquery->op;
		table->schema = select->schema;
		table->reader->type = READ_SUBQUERY;
	} else {

		/* If we've made it this far, we want to try
		 * and determine schema by reading the top
		 * row of the file and assume a delimited
		 * list of field names.
		 */
		if (schema_resolve_file(table)) {
			return FQL_FAIL;
		}

		/* Retrieve schema using libcsv */
		if (table->join_type == JOIN_FROM) {
			table->reader->type = READ_LIBCSV;
		} else {
			table->reader->type = READ_MMAPCSV;
		}
	}
	reader_assign(table->reader, table);

	/* Skip here if we *now* know schema */
	if (!vec_empty(table->schema->columns)) {
		return FQL_GOOD;
	}

	Record rec;
	record_construct(&rec, 0, 0, false);
	table->reader->max_col_idx = INT_MAX;
	table->reader->get_record__(table->reader, &rec);
	char* delim = table_get_delim(table);
	strncpy_(table->schema->delimiter, delim, DELIM_LEN_MAX);
	table->reader->max_col_idx = 0;

	schema_assign_header(table, &rec);
	record_destroy(&rec);

	return FQL_GOOD;
}

int _evaluate_if_const(Column* col)
{
	Function* func = col->field.fn;
	Column** it = vec_begin(func->args);
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
	if (func->call__(func, &new_field, NULL)) {
		return FQL_FAIL;
	}

	function_free(func);
	col->expr = EXPR_CONST;
	col->field = new_field;

	return FQL_GOOD;
}

int schema_assign_columns_limited(Vec* columns, Vec* sources, int limit)
{
	Column** it = vec_begin(columns);
	for (; it != vec_end(columns); ++it) {
		if ((*it)->expr == EXPR_FUNCTION) {
			Function* func = (*it)->field.fn;
			if (schema_assign_columns_limited(func->args, sources, limit)) {
				return FQL_FAIL;
			}
			if(function_op_resolve(func, &(*it)->field_type)) {
				return FQL_FAIL;
			}
			if (function_validate(func)) {
				return FQL_FAIL;
			}
			if (_evaluate_if_const(*it)) {
				return FQL_FAIL;
			}
			continue;
		}
		if ((*it)->expr != EXPR_COLUMN_NAME) {
			continue;
		}

		int matches = 0;
		int j = 0;

		for (; j <= limit; ++j) {
			Table* search_table = vec_at(sources, j);
			if (string_empty(&(*it)->table_name) ||
			    istring_eq((*it)->table_name.data, search_table->alias.data)) {
				matches += column_try_assign_source(*it, search_table, j);
			}
		}

		if (matches > 1) {
			fprintf(stderr, "%s: ambiguous column\n", (*it)->name.data);
			return FQL_FAIL;
		}

		if (matches == 0) {
			fprintf(stderr, "%s: cannot find column\n", (*it)->name.data);
			return FQL_FAIL;
		}
	}

	return FQL_GOOD;
}

int schema_assign_columns(Vec* columns, Vec* sources)
{
	return schema_assign_columns_limited(columns,
					     sources,
					     sources->size - 1);
}

int _asterisk_resolve(Vec* columns, Vec* sources)
{
	int i = 0;
	for (; i < columns->size; ++i) {
		int col_idx = i;
		Column** col = vec_at(columns, i);
		if ((*col)->expr != EXPR_ASTERISK) {
			continue;
		}
		int matches = 0;
		int j = 0;
		for (; j < sources->size; ++j) {
			Table* search_table = vec_at(sources, j);
			if (string_empty(&(*col)->table_name) ||
			    istring_eq((*col)->table_name.data, search_table->alias.data)) {
				if (matches > 0) {
					Column* new_col = column_new(EXPR_ASTERISK, NULL, "");
					new_col->src_idx = j;
					vec_insert(columns, ++i, &new_col);
					col = vec_at(columns, col_idx);
				} else {
					(*col)->src_idx = j;
				}
				++matches;
			}
		}


		if (matches == 0) {
			fprintf(stderr, "Could not locate table `%s'\n", (*col)->table_name.data);
			return FQL_FAIL;
		}
	}

	return FQL_GOOD;
}

enum join_side _get_join_side(Column* col, int right_idx)
{
	switch (col->expr) {
	case EXPR_COLUMN_NAME:
		return (col->src_idx < right_idx) ? SIDE_LEFT : SIDE_RIGHT;
	case EXPR_FUNCTION:
	{
		Function* func = col->field.fn;
		Column** it = vec_begin(func->args);
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


void _resolve_join_conditions(Table* right_table, int right_idx)
{
	if (right_table->condition == NULL ||
	    right_table->condition->joinable == NULL) {
		return;
	}

	Vec* joinable = right_table->condition->joinable;

	Logic** it = vec_begin(joinable);
	for (; it != vec_end(joinable); ++it) {
		enum join_side side0 = _get_join_side((*it)->col[0], right_idx);
		if (side0 == SIDE_MIXED) {
			continue;
		}
		enum join_side side1 = _get_join_side((*it)->col[1], right_idx);
		if (side0 != side1) {
			if (!logic_can_be_false(right_table->condition, *it)) {
				right_table->condition->join_logic = *it;
				struct hashjoin* hj = hashjoin_new();
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

	/* No more need for this vector */
	//vec_free(right_table->condition->joinable);
}

int schema_resolve_query(struct fql_handle* fql, Query* query)
{
	Vec* sources = query->sources;

	int i = 0;
	for (; i < sources->size; ++i) {
		Table* table = vec_at(query->sources, i);
		if (schema_resolve_source(fql, table)) {
			return FQL_FAIL;
		}

		if (i == 0 && !op_has_delim(query->op)) {
			op_set_delim(query->op, table->schema->delimiter);
		}

		if (schema_assign_columns_limited(table->validation_list,
					          sources, i)) {
			return FQL_FAIL;
		}

		if (i > 0 && !fql->props.force_cartesian) {
			_resolve_join_conditions(table, i);
		}
	}

	if (schema_assign_columns(query->validation_list, query->sources)) {
		return FQL_FAIL;
	}

	Vec* op_cols = op_get_validation_list(query->op);
	if (_asterisk_resolve(op_cols, sources)) {
		return FQL_FAIL;
	}
	if (schema_assign_columns(op_cols, sources)) {
		return FQL_FAIL;
	}

	op_finalize(query);

	return FQL_GOOD;
}

int schema_resolve(struct fql_handle* fql)
{
	Queue* query_node = fql->query_list;
	for (; query_node; query_node = query_node->next) {
		Query* query = query_node->data;

		if (fql->props.out_delim[0]) {
			op_set_delim(query->op, fql->props.out_delim);
		}

		if (schema_resolve_query(fql, query)) {
			return FQL_FAIL;
		}
	}

	return FQL_GOOD;
}


