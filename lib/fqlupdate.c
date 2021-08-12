#include "fqlupdate.h"
#include "aggregate.h"
#include "expression.h"
#include "process.h"
#include "util.h"
#include "writer.h"
#include "order.h"
#include "schema.h"
#include "query.h"
#include "fql.h"

int _update_writer(fqlupdate*, node*);
int _noupdate_writer(fqlupdate*, node*);

fqlupdate* fqlupdate_construct(fqlupdate* self)
{
	*self = (fqlupdate) {
	        OP_UPDATE,         /* oper_type */
	        new_(schema),      /* schema */
	        NULL,              /* writer */
	        &_update_writer,   /* update__ */
	        &_noupdate_writer, /* noupdate__ */
	        0,                 /* update_idx */
	        0,                 /* rows_affected */
	        -1,                /* top_count */
	        {0},               /* update_expressions */
	        {0},               /* set_columns */
	        {0},               /* value_expressions */
	        0,                 /* table_idx */
	        0,                 /* rownum */
	        FILTER_OPEN,       /* state */
	        false,             /* has_matched_alias */
	};

	expression* lol_expr = new_(expression, EXPR_ASTERISK, NULL, "");
	schema_add_expression(self->schema, lol_expr, 0);

	vec_construct_(&self->update_expressions, expression*);
	vec_construct_(&self->set_columns, expression*);
	vec_construct_(&self->value_expressions, expression*);

	lol_expr = new_(expression, EXPR_ASTERISK, NULL, "");
	vec_push_back(&self->update_expressions, &lol_expr);

	return self;
}

void fqlupdate_destroy(fqlupdate* self)
{
	if (self == NULL) {
		return;
	}

	expression** it = vec_begin(&self->update_expressions);
	for (; it != vec_end(&self->update_expressions); ++it) {
		delete_(expression, *it);
	}
	vec_destroy(&self->update_expressions);

	it = vec_begin(&self->set_columns);
	for (; it != vec_end(&self->set_columns); ++it) {
		delete_(expression, *it);
	}
	vec_destroy(&self->set_columns);
	/* these should all have been copied to update_expressions, 
	 * so no need to free these individually.
	 */
	vec_destroy(&self->value_expressions);
	delete_if_exists_(writer, self->writer);
	delete_(schema, self->schema);
}

int fqlupdate_add_expression(fqlupdate* self, const expression* expr)
{
	if (self->set_columns.size == self->value_expressions.size) {
		vec_push_back(&self->set_columns, &expr);
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
		process* update_proc = plan->op_true->data;
		update_proc->action__ = &fql_update;
		update_proc->wait_for_in0_end = true;
		update_proc->proc_data = self;
		update_proc->has_second_input = true;
		/* If there is no where clause, we need to mark
		 * the read_proc as secondary so it always passes
		 * data to fifo_in[1] on the update process. This
		 * is the "TRUE" input to the update process.
		 */
		if (query->where == NULL) {
			table* update_table = vec_begin(query->sources);
			update_table->read_proc->is_secondary = true;
			update_proc->killed_pipe = 0;
		} else {
			update_proc->is_dual_link = true;
		}

		string_strcpy(update_proc->plan_msg, "UPDATE");

		update_proc = plan->op_false->data;
		update_proc->is_passive = true;

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

	/*** NOTE: UPDATE is becoming op_true!!!! ***/
	plan->op_true = filter_node;

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

/* At this point, the update expressions represent the output
 * expressions as if there were no SET statements.  As we go
 * through the SET columns, overwrite the existing expressions.
 */
int fqlupdate_resolve_additional(fqlupdate* self, query* query)
{
	table* update_table = vec_at(query->sources, self->table_idx);

	/* Identify an "okay to overwrite" record by verifying 
	 * this "is_passthrough" flag on a match.
	 */
	expression** it = vec_begin(&self->update_expressions);
	for (; it != vec_end(&self->update_expressions); ++it) {
		(*it)->is_passthrough = true;
	}

	unsigned i = 0;
	for (; i < self->set_columns.size; ++i) {
		expression** update_col = vec_at(&self->set_columns, i);
		vec* matches = multimap_get(update_table->schema->expr_map,
		                            string_c_str(&(*update_col)->name));
		if (matches == NULL) {
			fprintf(stderr,
			        "unable to match set column `%s'\n",
			        string_c_str(&(*update_col)->name));
			return FQL_FAIL;
		}

		if (matches->size > 1) {
			fprintf(stderr,
			        "ambiguous column name `%s'\n",
			        string_c_str(&(*update_col)->name));
		}

		expression** match_schema_expr = vec_begin(matches);
		expression** match_expr =
		        vec_at(&self->update_expressions, (*match_schema_expr)->index);

		if (!(*match_expr)->is_passthrough) {
			fprintf(stderr,
			        "set column `%s' matched more than once\n",
			        string_c_str(&(*update_col)->name));
			return FQL_FAIL;
		}

		expression** new_val = vec_at(&self->value_expressions, i);
		(*new_val)->location = (*match_expr)->location;
		(*new_val)->width = (*match_expr)->width;

		/* Since we are overwriting its spot any way, free it... */
		delete_(expression, *match_expr);

		/* overwrite that shit */
		*match_expr = *new_val;
	}
	return FQL_GOOD;
}

/* called from schema.c: _resolve_unions */
int fqlupdate_resolve_final_types(fqlupdate* self)
{
	return FQL_GOOD;
}

int _update_writer(fqlupdate* self, node* rg)
{
	++self->rownum;
	return self->writer->write_record__(self->writer,
	                                    &self->update_expressions,
	                                    rg,
	                                    NULL);
}

int _noupdate_writer(fqlupdate* self, node* rg)
{
	return self->writer->write_record__(self->writer,
	                                    self->schema->expressions,
	                                    rg,
	                                    NULL);
}
