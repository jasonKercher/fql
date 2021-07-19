#include "fqlselect.h"
#include "csv.h"
#include "field.h"
#include "fql.h"
#include "misc.h"
#include "group.h"
#include "order.h"
#include "logic.h"
#include "query.h"
#include "reader.h"
#include "expression.h"
#include "schema.h"
#include "process.h"
#include "util/stringy.h"
#include "util/util.h"
#include "writer.h"

int _select_record(fqlselect*, struct recgroup*);
int _select_record_api(fqlselect*, struct recgroup*);
int _select_record_order_api(fqlselect*, struct recgroup*);
int _select_record_to_list(fqlselect*, struct recgroup*);
int _select_record_to_const(fqlselect*, struct recgroup*);
int _select_subquery(fqlselect*, struct recgroup*);

fqlselect* fqlselect_construct(fqlselect* self)
{
	*self = (fqlselect) {
	        OP_SELECT,       /* oper_type */
	        NULL,            /* api */
	        new_(schema),    /* schema */
	        NULL,            /* writer */
	        NULL,            /* list_data */
	        NULL,            /* const_dest */
	        NULL,            /* union_selects */
	        NULL,            /* _selection_exprs */
	        &_select_record, /* select__ */
	        0,               /* offset */
	        0,               /* rows_affected */
	        -1,              /* top_count */
	        0,               /* rownum */
	        false,           /* is_const */
	};

	self->_selection_exprs = self->schema->expressions;

	return self;
}

void fqlselect_destroy(fqlselect* self)
{
	if (self == NULL) {
		return;
	}
	//queue_free(&self->union_selects);
	delete_if_exists_(writer, self->writer);
	delete_(schema, self->schema);
}

unsigned fqlselect_get_field_count(fqlselect* self)
{
	unsigned field_count = 0;
	expression** it = vec_begin(self->schema->expressions);
	for (; it != vec_end(self->schema->expressions); ++it) {
		switch ((*it)->expr) {
		case EXPR_ASTERISK:
			field_count += (*it)->table->schema->expressions->size;
			break;
		default:
			++field_count;
		}
	}

	return field_count;
}

void fqlselect_set_delim(fqlselect* self, const char* delim)
{
	strncpy_(self->schema->delimiter, delim, DELIM_LEN_MAX);
}

void fqlselect_set_rec_terminator(fqlselect* self, const char* term)
{
	strncpy_(self->schema->rec_terminator, term, DELIM_LEN_MAX);
}

void fqlselect_set_schema(fqlselect* self, const schema* src_schema)
{
	if (src_schema == NULL) {
		fqlselect_set_delim(self, ",");
		self->schema->io_type = IO_LIBCSV;
		self->schema->write_io_type = IO_LIBCSV;
		self->schema->is_default = true;
		return;
	}
	if (!self->schema->delimiter[0]) {
		fqlselect_set_delim(self, src_schema->delimiter);
	}
	self->schema->io_type = src_schema->io_type;
	self->schema->write_io_type = src_schema->write_io_type;
	self->schema->is_default = src_schema->is_default;
}

void fqlselect_add_expression(fqlselect* self, expression* expr)
{
	schema_add_expression(self->schema, expr, 0);
}

void _resize_raw_rec(vec* raw_rec, unsigned size)
{
	unsigned org_size = raw_rec->size;
	vec_resize(raw_rec, size);
	string* s = vec_at(raw_rec, org_size);
	for (; s != vec_end(raw_rec); ++s) {
		string_construct(s);
	}
}

/* this should be in schema.c */
int _expand_asterisk(vec* expr_vec, table* table, unsigned src_idx, unsigned* expr_idx)
{
	vec* src_expr_vec = table->schema->expressions;

	table->reader->max_idx = src_expr_vec->size - 1;

	expression** it = vec_begin(src_expr_vec);
	for (; it != vec_end(src_expr_vec); ++it) {
		//string* expr_name = string_from_string(&(*it)->alias);
		expression* new_expr =
		        new_(expression, EXPR_COLUMN_NAME, (*it)->alias.data, "");
		new_expr->data_source = *it;
		new_expr->src_idx = src_idx;
		new_expr->field_type = (*it)->field_type;
		++(*expr_idx);
		vec_insert_at(expr_vec, *expr_idx, &new_expr, 1);
	}

	return table->schema->expressions->size;
}

void _expand_asterisks(query* query, bool force_expansion)
{
	fqlselect* self = query->op;
	vec* expr_vec = self->schema->expressions;
	unsigned i = 0;

	for (; i < expr_vec->size; ++i) {
		expression** expr = vec_at(expr_vec, i);
		if ((*expr)->expr != EXPR_ASTERISK) {
			continue;
		}

		table* table = vec_at(query->sources, (*expr)->src_idx);

		if (table->subquery == NULL /* is not a subquery source */
		    && !force_expansion && query->query_id == 0 /* is in main query */
		    && schema_eq(table->schema, self->schema)) {
			continue;
		}

		unsigned asterisk_index = i;
		_expand_asterisk(expr_vec, table, (*expr)->src_idx, &i);

		expression** asterisk_expr = vec_at(expr_vec, asterisk_index);
		delete_(expression, *asterisk_expr);
		vec_erase_at(expr_vec, asterisk_index, 1);
		--i;
	}

	expression_update_indicies(expr_vec);

	if (self->writer == NULL) {
		return;
	}
	_resize_raw_rec(self->writer->raw_rec, expr_vec->size);
}

int fqlselect_resolve_type_from_subquery(expression* expr)
{
	query* subquery = expr->subquery;
	fqlselect* subselect = subquery->op;
	if (subselect->schema->expressions->size > 1) {
		fputs("Only one expression can be supplied if subquery is an "
		      "expression\n",
		      stderr);
		return FQL_FAIL;
	}
	expression** subexpr = vec_begin(subselect->schema->expressions);
	if ((*subexpr)->subquery == NULL) {
		expr->field_type = (*subexpr)->field_type;
		return FQL_GOOD;
	}
	try_(fqlselect_resolve_type_from_subquery(*subexpr));
	expr->field_type = (*subexpr)->field_type;

	return FQL_GOOD;
}

/* Unions have the ability to change the types of
 * the parent select. For example, the following
 * query should fail:
 *
 * SELECT 'A' AS foo UNION ALL SELECT 1
 *
 * The correct type of column foo is int!
 */
int fqlselect_resolve_final_types(fqlselect* self)
{
	if (self->union_selects == NULL) {
		return FQL_GOOD;
	}

	vec* expr_vec = self->schema->expressions;

	queue* union_node = self->union_selects;
	for (; union_node; union_node = union_node->next) {
		fqlselect* union_select = union_node->data;
		vec* union_expr_vec = union_select->schema->expressions;
		unsigned i = 0;
		for (; i < expr_vec->size; ++i) {
			expression** expr1 = vec_at(expr_vec, i);
			expression** expr2 = vec_at(union_expr_vec, i);
			enum field_type type1 = (*expr1)->field_type;
			enum field_type type2 = (*expr2)->field_type;
			enum field_type new_type = field_determine_type(type1, type2);
			if (new_type != type1) {
				try_(expression_cast(*expr1, new_type));
			}
			if (new_type != type2) {
				try_(expression_cast(*expr2, new_type));
			}
		}
	}

	return FQL_GOOD;
}

int fqlselect_connect_api(query* query, vec* api)
{
	fqlselect* self = query->op;
	if (query->orderby == NULL) {
		self->select__ = &_select_record_api;
	} else {
		self->select__ = &_select_record_order_api;
	}
	_expand_asterisks(query, true);

	vec* exprs = self->schema->expressions;
	vec_resize(api, exprs->size);

	expression** it = vec_begin(exprs);
	unsigned i = 0;
	for (; i < exprs->size; ++i) {
		if ((*it)->subquery != NULL) {
			try_(fqlselect_resolve_type_from_subquery(*it));
		}
		struct fql_field* field = vec_at(api, i);
		switch (it[i]->field_type) {
		case FIELD_INT:
			field->type = FQL_INT;
			break;
		case FIELD_FLOAT:
			field->type = FQL_FLOAT;
			break;
		case FIELD_STRING:
			field->type = FQL_STRING;
			field->_in = new_(string);
			break;
		default:
			fputs("Undefined API type\n", stderr);
			return FQL_FAIL;
		}
	}

	self->api = api;
	return FQL_GOOD;
}

void fqlselect_apply_process(query* query, plan* plan, bool is_subquery_source)
{
	fqlselect* self = query->op;
	process* proc = plan->op_true->data;
	proc->action__ = &fql_select;
	proc->wait_for_in0_end = true;
	proc->proc_data = self;

	if (self->const_dest != NULL) {
		self->select__ = &_select_record_to_const;
	} else if (is_subquery_source) {
		self->select__ = &_select_subquery;
	}

	string_strcpy(proc->plan_msg, "SELECT ");

	vec* expr_vec = self->schema->expressions;
	expression** expr = vec_begin(expr_vec);
	for (; expr != vec_end(expr_vec); ++expr) {
		if (expr != vec_begin(expr_vec)) {
			string_strcat(proc->plan_msg, ",");
		}
		expression_cat_description(*expr, proc->plan_msg);
	}

	proc = plan->op_false->data;
	proc->is_passive = true;

	if (self->writer == NULL) {
		return;
	}
	writer_set_delimiter(self->writer, self->schema->delimiter);
	writer_set_rec_terminator(self->writer, self->schema->rec_terminator);
}

void fqlselect_apply_expression_alias(fqlselect* self, const char* alias)
{
	schema_apply_expression_alias(self->schema, alias);
}

int fqlselect_set_as_inlist(fqlselect* self, inlist* inlist)
{
	if (self->schema->expressions->size == 1) {
		expression** expr = vec_begin(self->schema->expressions);
		if ((*expr)->expr == EXPR_ASTERISK) {
			_expand_asterisks(inlist->subquery, true);
		}
	}
	if (self->schema->expressions->size != 1) {
		fputs("Only one expression can be specified in subquery\n", stderr);
		return FQL_FAIL;
	}
	inlist->list_data = new_(set, 16, HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM);
	self->list_data = inlist->list_data;
	self->select__ = &_select_record_to_list;
	return FQL_GOOD;
}

/* NOTE: do not allow any writer initialization if a union query */
int fqlselect_writer_init(fqlselect* self, query* query, struct fql_handle* fql)
{
	if (!query->union_id) {
		self->writer =
		        new_(writer, self->schema->write_io_type, fql->props.strictness);
	}

	if (!query->union_id && query->into_table_name) {
		if (!fql->props.overwrite && access(query->into_table_name, F_OK) == 0) {
			fprintf(stderr,
			        "Cannot SELECT INTO: file `%s' already exists\n",
			        query->into_table_name);
			return FQL_FAIL;
		}
		try_(writer_open(self->writer, query->into_table_name));
	}

	if (!query->union_id && query->orderby != NULL) {
		char* out_name = writer_take_filename(self->writer);
		const char* in_name = writer_get_tempname(self->writer);

		try_(order_init_io(query->orderby, in_name, out_name));
		/* take_filename takes ownership, must free */
		free_(out_name);
	}

	_expand_asterisks(query, (!query->groupby || !query->distinct));

	/* TODO: verify this does not cause double free */
	if (query->distinct != NULL) {
		expression** it = vec_begin(self->schema->expressions);
		for (; it != vec_end(self->schema->expressions); ++it) {
			group_add_expression(query->distinct, *it);
		}
	}

	schema_preflight(self->schema);

	return FQL_GOOD;
}

int fqlselect_next_union(fqlselect* self)
{
	if (self->union_selects == NULL) {
		return 0;
	}

	fqlselect* union_select = queue_dequeue(&self->union_selects);
	self->_selection_exprs = union_select->schema->expressions;
	self->top_count = union_select->top_count;
	self->is_const = union_select->is_const;
	self->rows_affected = 0;

	return 1;
}

void fqlselect_preop(fqlselect* self, query* query)
{
	if (!self->schema->is_default) {
		return;
	}

	vec header;
	vec_construct_(&header, expression*);

	/* print header */
	expression** it = vec_begin(self->schema->expressions);
	for (; it != vec_end(self->schema->expressions); ++it) {
		if ((*it)->expr == EXPR_ASTERISK) {
			table* aster_src = vec_at(query->sources, (*it)->src_idx);
			vec* aster_exprs = aster_src->schema->expressions;
			expression** it2 = vec_begin(aster_exprs);
			for (; it2 != vec_end(aster_exprs); ++it2) {
				expression* field_expr =
				        new_(expression, EXPR_CONST, NULL, "");
				string_copy(&field_expr->buf, &(*it2)->alias);
				field_expr->field.s = &field_expr->buf;
				field_expr->field_type = FIELD_STRING;
				vec_push_back(&header, &field_expr);
			}
			continue;
		}
		expression* field_expr = new_(expression, EXPR_CONST, NULL, "");
		string_copy(&field_expr->buf, &(*it)->alias);
		field_expr->field.s = &field_expr->buf;
		field_expr->field_type = FIELD_STRING;
		vec_push_back(&header, &field_expr);
	}

	writer* writer = self->writer;
	if (query->orderby != NULL) {
		writer->write_record__(writer, &header, NULL, query->orderby->out_file);
	} else {
		writer->write_record__(writer, &header, NULL, NULL);
	}

	it = vec_begin(&header);
	for (; it != vec_end(&header); ++it) {
		delete_(expression, *it);
	}
	vec_destroy(&header);
}

int _select_record(fqlselect* self, recgroup* rg)
{
	++self->rownum;
	writer* writer = self->writer;
	vec* expr_vec = self->_selection_exprs;

	int ret = writer->write_record__(writer, expr_vec, rg, NULL);

	if (ret == FQL_FAIL || rg == NULL) {
		return ret;
	}

	/* pass offset information along
	 * in the top record
	 */
	record* rec = recgroup_rec_at(rg, 0);
	rec->offset = self->offset;
	rg->select_len = ret;

	self->offset += ret;

	return ret;
}

int _select_record_api(fqlselect* self, struct recgroup* rg)
{
	++self->rownum;
	vec* expr_vec = self->_selection_exprs;

	expression** exprs = vec_begin(expr_vec);
	unsigned i = 0;
	for (; i < expr_vec->size; ++i) {
		struct fql_field* field = vec_at(self->api, i);
		switch (exprs[i]->field_type) {
		case FIELD_INT:
			try_(expression_get_int(&field->data.i, exprs[i], rg));
			break;
		case FIELD_FLOAT:
			try_(expression_get_float(&field->data.f, exprs[i], rg));
			break;
		case FIELD_STRING: {
			stringview sv;
			try_(expression_get_stringview(&sv, exprs[i], rg));
			string* s = field->_in;
			string_copy_from_stringview(s, &sv);
			field->data.s = s->data;
			break;
		}
		default:;
		}
	}

	return 1;
}

int _select_record_order_api(fqlselect* self, struct recgroup* rg)
{
	++self->rownum;
	FILE* order_input = writer_get_file(self->writer);

	vec* expr_vec = self->_selection_exprs;
	expression** exprs = vec_begin(expr_vec);
	unsigned i = 0;
	unsigned len = 0;
	for (; i < expr_vec->size; ++i) {
		switch (exprs[i]->field_type) {
		case FIELD_INT: {
			long num = 0;
			try_(expression_get_int(&num, exprs[i], rg));
			fwrite(&num, sizeof(num), 1, order_input);
			len += sizeof(num);
			break;
		}
		case FIELD_FLOAT: {
			double num = 0;
			try_(expression_get_float(&num, exprs[i], rg));
			fwrite(&num, sizeof(num), 1, order_input);
			len += sizeof(num);
			break;
		}
		case FIELD_STRING: {
			stringview sv;
			try_(expression_get_stringview(&sv, exprs[i], rg));
			fwrite(&sv.len, sizeof(sv.len), 1, order_input);
			fwrite(sv.data, 1, sv.len, order_input);
			len += sizeof(sv.len) + sv.len;
			break;
		}
		default:;
		}
	}
	record* rec = recgroup_rec_at(rg, 0);
	rec->offset = self->offset;
	self->offset += len;

	return 1;
}

int _select_record_to_list(fqlselect* self, recgroup* rg)
{
	++self->rownum;
	set* list = self->list_data;

	expression* expr = *(expression**)vec_begin(self->_selection_exprs);

	switch (expr->field_type) {
	case FIELD_INT: {
		long num = 0;
		try_(expression_get_int(&num, expr, rg));
		set_nadd(list, (char*)&num, sizeof(num));
		return 1;
	}
	case FIELD_FLOAT: {
		double num = 0;
		try_(expression_get_float(&num, expr, rg));
		set_nadd(list, (char*)&num, sizeof(num));
		return 1;
	}
	case FIELD_STRING: {
		stringview sv;
		try_(expression_get_stringview(&sv, expr, rg));
		set_nadd(list, sv.data, sv.len);
		return 1;
	}
	default:
		fputs("Unexpected type in select list\n", stderr);
		return FQL_FAIL;
	}
}

int _select_record_to_const(fqlselect* self, recgroup* rg)
{
	//bool fail_if_on_result = (self->const_dest->expr == EXPR_CONST) {
	++self->rownum;
	if (self->const_dest->expr == EXPR_CONST) {
		fputs("subquery returned more than 1 value as expression\n", stderr);
		return FQL_FAIL;
	}

	expression* expr = *(expression**)vec_begin(self->_selection_exprs);

	self->const_dest->field_type = expr->field_type;
	self->const_dest->expr = EXPR_CONST;

	switch (expr->field_type) {
	case FIELD_INT: {
		long num = 0;
		try_(expression_get_int(&num, expr, rg));
		self->const_dest->field.i = num;
		return 1;
	}
	case FIELD_FLOAT: {
		double num = 0;
		try_(expression_get_float(&num, expr, rg));
		self->const_dest->field.f = num;
		return 1;
	}
	case FIELD_STRING: {
		stringview sv;
		try_(expression_get_stringview(&sv, expr, rg));
		string_copy_from_stringview(&self->const_dest->buf, &sv);
		self->const_dest->field.s = &self->const_dest->buf;
		return 1;
	}
	default:
		fputs("Unexpected type in select list\n", stderr);
		return FQL_FAIL;
	}
}

/* This is correct.  The only reason this exists is
 * to take advantage of the fact that fql_select will
 * move through all unions via fqlselect_next_union.
 */
int _select_subquery(fqlselect* self, recgroup* rg)
{
	++self->rownum;
	return FQL_GOOD;
}
