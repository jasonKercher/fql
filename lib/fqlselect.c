#include "fqlselect.h"
#include "csv.h"
#include "field.h"
#include "fql.h"
#include "io.h"
#include "misc.h"
#include "group.h"
#include "order.h"
#include "logic.h"
#include "query.h"
#include "writer.h"
#include "reader.h"
#include "schema.h"
#include "process.h"
#include "expression.h"
#include "util/stringy.h"
#include "util/util.h"
#include "util/node.h"

int _select_record(fqlselect*, node*);
int _select_record_api(fqlselect*, node*);
int _select_record_order_api(fqlselect*, node*);
int _select_record_to_list(fqlselect*, node*);
int _select_record_to_const(fqlselect*, node*);
int _select_subquery(fqlselect*, node*);

fqlselect* fqlselect_construct(fqlselect* self)
{
	*self = (fqlselect) {
	        .oper_type = FQL_SELECT,
	        .schema = new_(schema),
	        .select__ = &_select_record,
	        .top_count = -1,
	        .must_run_once = true,
	};

	self->current_select = self;

	return self;
}

void fqlselect_destroy(fqlselect* self)
{
	if (self == NULL) {
		return;
	}

	delete_if_exists_(writer, self->writer);
	delete_(schema, self->schema);
	if (self->header_as_exprs == NULL) {
		return;
	}

	expression** it = vec_begin(self->header_as_exprs);
	for (; it != vec_end(self->header_as_exprs); ++it) {
		delete_(expression, *it);
	}
	delete_(vec, self->header_as_exprs);

	node_free(&self->union_selects);
}

unsigned fqlselect_get_field_count(fqlselect* self, const vec* sources)
{
	table* expr_table = NULL;
	unsigned field_count = 0;
	expression** it = vec_begin(self->schema->expressions);
	for (; it != vec_end(self->schema->expressions); ++it) {
		switch ((*it)->expr) {
		case EXPR_ASTERISK:
			expr_table = vec_at(sources, (*it)->src_idx);
			field_count += expr_table->schema->expressions->size;
			break;
		default:
			++field_count;
		}
	}

	return field_count;
}

void fqlselect_add_expression(fqlselect* self, expression* expr)
{
	schema_add_expression(self->schema, expr, 0);
	switch (expr->expr) {
	case EXPR_ROW_NUMBER:
	case EXPR_AGGREGATE:
		break;
	default:
		if (!expression_is_const(expr)) {
			self->must_run_once = false;
		}
	}
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

	node* union_node = self->union_selects;
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
	op_expand_asterisks(query, true);

	vec* exprs = self->schema->expressions;
	vec_resize(api, exprs->size);

	expression** it = vec_begin(exprs);
	unsigned i = 0;
	for (; i < exprs->size; ++i) {
		if ((*it)->subquery != NULL) {
			try_(fqlselect_resolve_type_from_subquery(*it));
		}
		struct fql_field* field = vec_at(api, i);
		field->is_null = false;
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
	proc->proc_data = self;

	if (self->const_dest != NULL) {
		if (query->orderby == NULL) {
			self->select__ = &_select_record_to_const;
		} else {
			self->select__ = &_select_record_order_api;
		}
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
			op_expand_asterisks(inlist->subquery, true);
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

int fqlselect_next_union(fqlselect* self)
{
	if (self->_union_node == NULL) {
		return 0;
	}

	self->current_select = self->_union_node->data;

	self->_union_node = self->_union_node->next;
	self->rows_affected = 0;

	return 1;
}

/* The "must_run_once" flag at this point means that the
 * conditions are correct to force a SELECT under the
 * assumption that there may be an unreached aggregate in
 * the SELECT list:
 *   SELECT COUNT(*)
 *   FROM T1
 *   WHERE 1=0
 *
 * Even though no records will make it to the SELECT,
 * we must still report our count of 0. If this condition
 * is true, then we must check that there is indeed an
 * aggregate function to warrant forcing a SELECT. We
 * wouldn't want "must_run_once" set true for a query like
 *   SELECT 1
 *   WHERE 1=0
 *
 * TODO: Verify this behavior when UNION is involved
 */
void fqlselect_verify_must_run(fqlselect* self)
{
	if (self->must_run_once) {
		expression** it = vec_begin(self->schema->expressions);
		bool agg_found = false;
		for (; it != vec_end(self->schema->expressions); ++it) {
			if ((*it)->expr == EXPR_AGGREGATE) {
				agg_found = true;
				break;
			}
		}
		if (!agg_found) {
			self->must_run_once = false;
		}
	}
}

int fqlselect_reset(fqlselect* self)
{
	self->offset = 0;
	self->rownum = 0;
	self->rows_affected = 0;
	if (self->const_dest != NULL) {
		self->const_dest->expr = EXPR_SUBQUERY;
	}

	if (self->union_selects != NULL) {
		self->_union_node = self->union_selects;
	}
	self->current_select = self;
	return FQL_GOOD;
}

void _preop_build_header(fqlselect* self, query* query)
{
	self->header_as_exprs = new_t_(vec, expression*);

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
				vec_push_back(self->header_as_exprs, &field_expr);
			}
			continue;
		}
		expression* field_expr = new_(expression, EXPR_CONST, NULL, "");
		string_copy(&field_expr->buf, &(*it)->alias);
		field_expr->field.s = &field_expr->buf;
		field_expr->field_type = FIELD_STRING;
		vec_push_back(self->header_as_exprs, &field_expr);
	}

}


void fqlselect_preop(fqlselect* self, query* query)
{
	self->_union_node = self->union_selects;

	if (self->schema->write_io_type != IO_LIBCSV
	    || (!self->schema->is_default && !query->fqlref->props.add_header)
	    || (self->schema->is_default && !query->fqlref->props.print_header)) {
		return;
	}

	if (self->header_as_exprs == NULL) {
		_preop_build_header(self, query);
	}

	writer* writer = self->writer;
	if (query->orderby != NULL) {
		writer->write_record__(writer,
		                       self->header_as_exprs,
		                       NULL,
		                       query->orderby->out_file);
	} else {
		writer->write_record__(writer, self->header_as_exprs, NULL, NULL);
	}
}

int _select_record(fqlselect* self, node* rg)
{
	++self->rownum;
	writer* writer = self->writer;
	vec* expr_vec = self->current_select->schema->expressions;

	int ret = writer->write_record__(writer, expr_vec, rg, NULL);

	if (ret == FQL_FAIL || rg == NULL) {
		return ret;
	}

	/* pass offset information along
	 * in the top record
	 */
	record* rec = rg->data;
	rec->offset = self->offset;
	rec->select_len = ret;

	self->offset += ret;

	return ret;
}

int _select_record_api(fqlselect* self, node* rg)
{
	++self->rownum;
	vec* expr_vec = self->current_select->schema->expressions;

	expression** exprs = vec_begin(expr_vec);
	unsigned i = 0;
	for (; i < expr_vec->size; ++i) {
		struct fql_field* field = vec_at(self->api, i);
		field->is_null = false;
		switch (exprs[i]->field_type) {
		case FIELD_INT:
			if (try_(expression_get_int(&field->data.i, exprs[i], rg))
			    == FQL_NULL) {
				field->data.i = 0;
				field->is_null = true;
			}
			break;
		case FIELD_FLOAT:
			if (try_(expression_get_float(&field->data.f, exprs[i], rg))
			    == FQL_NULL) {
				field->data.f = -0;
				field->is_null = true;
			}
			break;
		case FIELD_STRING: {
			stringview sv;
			string* s = field->_in;
			if (try_(expression_get_stringview(&sv, exprs[i], rg))
			    == FQL_NULL) {
				string_clear(s);
				field->is_null = true;
			} else {
				string_copy_from_stringview(s, &sv);
			}
			field->data.s = s->data;
			break;
		}
		default:;
		}
	}

	return 1;
}

int _select_record_order_api(fqlselect* self, node* rg)
{
	++self->rownum;
	FILE* order_input = writer_get_file(self->writer);

	vec* expr_vec = self->current_select->schema->expressions;
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
	record* rec = rg->data;
	rec->offset = self->offset;
	self->offset += len;

	return 1;
}

int _select_record_to_list(fqlselect* self, node* rg)
{
	++self->rownum;
	set* list = self->list_data;

	expression* expr =
	        *(expression**)vec_begin(self->current_select->schema->expressions);

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

int _select_record_to_const(fqlselect* self, node* rg)
{
	++self->rownum;
	if (self->const_dest->expr == EXPR_CONST) {
		fputs("subquery returned more than 1 value as expression\n", stderr);
		return FQL_FAIL;
	}

	expression* expr =
	        *(expression**)vec_begin(self->current_select->schema->expressions);

	self->const_dest->field_type = expr->field_type;
	self->const_dest->expr = EXPR_CONST;

	switch (expr->field_type) {
	case FIELD_INT: {
		long num = 0;
		if (try_(expression_get_int(&num, expr, rg))) {
			self->const_dest->expr = EXPR_NULL;
		}
		self->const_dest->field.i = num;
		return 1;
	}
	case FIELD_FLOAT: {
		double num = 0;
		if (try_(expression_get_float(&num, expr, rg))) {
			self->const_dest->expr = EXPR_NULL;
		}
		self->const_dest->field.f = num;
		return 1;
	}
	case FIELD_STRING: {
		stringview sv;
		if (try_(expression_get_stringview(&sv, expr, rg)) == FQL_NULL) {
			self->const_dest->expr = EXPR_NULL;
		}
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
int _select_subquery(fqlselect* self, node* rg)
{
	++self->rownum;
	return FQL_GOOD;
}
