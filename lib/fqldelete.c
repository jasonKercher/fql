#include "fqldelete.h"
#include "expression.h"
#include "process.h"
#include "util.h"
#include "writer.h"
#include "order.h"
#include "schema.h"
#include "query.h"
#include "fql.h"

int _delete_writer(fqldelete*, node*);

fqldelete* fqldelete_construct(fqldelete* self)
{
	*self = (fqldelete) {
	        OP_DELETE,       /* oper_type */
	        new_(schema),    /* schema */
	        NULL,            /* writer */
	        &_delete_writer, /* delete__ */
	        NULL,            /* delete_table */
	        0,               /* rows_affected */
	        -1,              /* top_count */
	        false,           /* has_matched_alias */
	};

	expression* lol_expr = new_(expression, EXPR_ASTERISK, NULL, "");
	schema_add_expression(self->schema, lol_expr, 0);

	return self;
}

void fqldelete_destroy(fqldelete* self)
{
	if (self == NULL) {
		return;
	}
	delete_if_exists_(writer, self->writer);
	delete_(schema, self->schema);
}

/**
 * Deleting is achieved by writing a new file and
 * renaming that file over top of the original.
 * This protects data from a runtime error. So,
 * the Primary process for delete is the op_false
 * node. If I match a record, the record is *not* 
 * written to the output.  The only resulting ID
 * should come from NOT matching. example:
 *
 * DELETE T1
 * WHERE FOO = BAR
 *
 * If foo = bar, we do *not* write to the new file.
 */
void fqldelete_apply_process(query* query, plan* plan)
{
	fqldelete* self = query->op;
	process* proc = plan->op_false->data;
	proc->action__ = &fql_delete;
	proc->wait_for_in0_end = true;
	proc->proc_data = self;

	string_strcpy(proc->plan_msg, "DELETE");

	proc = plan->op_true->data;
	proc->is_passive = true;

	writer_set_delimiter(self->writer, self->schema->delimiter);
	writer_set_rec_terminator(self->writer, self->schema->rec_terminator);
}

void fqldelete_preop(fqldelete* self, query* query)
{
	if (!self->schema->is_default) {
		return;
	}

	vec header;
	vec_construct_(&header, expression*);

	/* print header
	 * in a delete, we only care about full records. In order to
	 * re-use the select writing implementations, we treat this
	 * like the inverse of select [delete table].*
	 */
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

int _delete_writer(fqldelete* self, node* rg)
{
	return self->writer->write_record__(self->writer,
	                                    self->schema->expressions,
	                                    rg,
	                                    NULL);
}

