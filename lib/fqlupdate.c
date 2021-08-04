#include "fqlupdate.h"
#include "expression.h"
#include "process.h"
#include "util.h"
#include "writer.h"
#include "order.h"
#include "schema.h"
#include "query.h"
#include "fql.h"

int _update_writer(fqlupdate*, node*);

fqlupdate* fqlupdate_construct(fqlupdate* self)
{
	*self = (fqlupdate) {
	        OP_UPDATE,       /* oper_type */
	        new_(schema),    /* schema */
	        NULL,            /* writer */
	        &_update_writer, /* update__ */
	        0,               /* update_idx */
	        0,               /* rows_affected */
	        -1,              /* top_count */
	        {0},             /* update_columns */
	        {0},             /* value_expressions */
	        0,               /* table_idx */
	        FILTER_OPEN,     /* state */
	        false,           /* has_matched_alias */
	};

	expression* lol_expr = new_(expression, EXPR_ASTERISK, NULL, "");
	schema_add_expression(self->schema, lol_expr, 0);

	vec_construct_(&self->update_columns, expression*);
	vec_construct_(&self->value_expressions, expression*);

	return self;
}

void fqlupdate_destroy(fqlupdate* self)
{
	if (self == NULL) {
		return;
	}
	delete_if_exists_(writer, self->writer);
	delete_(schema, self->schema);
}

int fqlupdate_add_expression(fqlupdate* self, const expression* expr)
{
	if (self->update_columns.size == self->value_expressions.size) {
		vec_push_back(&self->update_columns, &expr);
		if (expr->expr != EXPR_COLUMN_NAME) {
			fputs("Unexpected expression in update element\n", stderr);
			return FQL_FAIL;
		}
	} else {
		vec_push_back(&self->value_expressions, &expr);
	}
	return FQL_GOOD;
}

int fqlupdate_apply_process(query* query, plan* plan)
{
	fqlupdate* self = query->op;
	if (query->sources->size == 1) {
		process* proc = plan->op_false->data;
		proc->action__ = &fql_update;
		proc->wait_for_in0_end = true;
		proc->proc_data = self;

		string_strcpy(proc->plan_msg, "UPDATE");

		proc = plan->op_true->data;
		proc->is_passive = true;

		writer_set_delimiter(self->writer, self->schema->delimiter);
		writer_set_rec_terminator(self->writer, self->schema->rec_terminator);
		return FQL_GOOD;
	}

	if (self->table_idx != 0) {
		fputs("Currently can only update from left side of join\n", stderr);
		return FQL_FAIL;
	}

	/* At this point, we are dealing with a join */
	process* update_filter_proc = new_(process, "UPDATE FILTER", plan);
	update_filter_proc->action__ = &fql_update_filter;
	update_filter_proc->proc_data = self;
	update_filter_proc->wait_for_in0_end = true;
	update_filter_proc->is_dual_link = true;
	update_filter_proc->has_second_input = true;
	dnode* filter_node = dgraph_add_data(plan->processes, update_filter_proc);

	plan->op_false->out[0] = filter_node;
	plan->op_true->out[0] = filter_node;

	process* proc = plan->op_false->data;
	proc->is_passive = true;
	proc = plan->op_true->data;
	proc->is_passive = true;

	writer_set_delimiter(self->writer, self->schema->delimiter);
	writer_set_rec_terminator(self->writer, self->schema->rec_terminator);

	return FQL_GOOD;
}

void fqlupdate_preop(fqlupdate* self, query* query)
{
	if (!self->schema->is_default) {
		return;
	}

	vec header;
	vec_construct_(&header, expression*);

	/* print header */
	expression** aster = vec_begin(self->schema->expressions);
	table* aster_src = vec_at(query->sources, (*aster)->src_idx);
	vec* aster_exprs = aster_src->schema->expressions;
	expression** it = vec_begin(aster_exprs);
	for (; it != vec_end(aster_exprs); ++it) {
		expression* field_expr = new_(expression, EXPR_CONST, NULL, "");
		string_copy(&field_expr->buf, &(*it)->alias);
		field_expr->field.s = &field_expr->buf;
		field_expr->field_type = FIELD_STRING;
		vec_push_back(&header, &field_expr);
	}

	self->writer->write_record__(self->writer, &header, NULL, NULL);

	it = vec_begin(&header);
	for (; it != vec_end(&header); ++it) {
		delete_(expression, *it);
	}
	vec_destroy(&header);
}

int _update_writer(fqlupdate* self, node* rg)
{
	return self->writer->write_record__(self->writer,
	                                    self->schema->expressions,
	                                    rg,
	                                    NULL);
}

int fqlupdate_resolve_additional(fqlupdate* self, query* query)
{
	expression** it = vec_begin(&self->value_expressions);
	for (; it != vec_end(&self->value_expressions); ++it) { }
	return FQL_GOOD;
}

/* called from schema.c: _resolve_unions */
int fqlupdate_resolve_final_types(fqlupdate* self)
{
	return FQL_GOOD;
}
