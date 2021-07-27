#include "reader.h"
#include "fqlselect.h"
#include "misc.h"

subquery* subquery_construct(subquery* self, fqlselect* select)
{
	self->select = select;
	/* basically just swap space */
	record_construct(&self->copy_data, 0);

	return self;
}

void subquery_free(void* generic)
{
	subquery* self = generic;
	record_destroy(&self->copy_data);
	free_(self);
}


/* NOTE: dest here actually belongs to the record group
 *       pointed to by _rg_ref. SO WE CANNOT touch dest
 *       until we've copied all stringviews to copy_data.
 *       Then, move that shit into dest afterward.
 */
int subquery_get_record(reader* reader, node* rg)
{
	//record* dest = node_rec_at(rg, 0);

	subquery* self = reader->reader_data;
	vec* expressions = self->select->_selection_exprs;

	record_clear_strings(&self->copy_data, self->copy_data.subquery_strings);
	vec_clear(&self->copy_data.fields);

	expression** it = vec_begin(expressions);
	for (; it != vec_end(expressions); ++it) {
		stringview* copy_sv = vec_add_one(&self->copy_data.fields);
		int ret = try_(expression_get_stringview(copy_sv, *it, rg));

		if (ret == FQL_NULL) {
			// TODO: do something about it...
		}

		switch ((*it)->expr) {
		case EXPR_GROUPING:
		case EXPR_FUNCTION: {
			/* NOTE: we actually need to copy some data here
			 *       functions own their buffers because they
			 *       only exist within their *clause*. Consider:
			 *
			 *       SELECT SUBSTRING(foo, 2, 5) AS foo
			 *       FROM t1
			 *       WHERE LEFT(foo, 1) = 'A'
			 *
			 *       The SELECT clause does not care about the
			 *       value returned by LEFT, and the WHERE
			 *       clause doesn't care about the value returned
			 *       by SUBSTRING. But if there is a subquery,
			 *
			 *       SELECT truncated_foo
			 *       FROM (
			 *           SELECT LEFT(foo, 5) truncated_foo
			 *           FROM T1
			 *       ) X
			 *
			 *       the parent SELECT *does* care about the
			 *       return value of LEFT. The result of the
			 *       function must outlive the select clause.
			 */
			string* s = record_generate_subquery_string(&self->copy_data);
			string_copy_from_stringview(s, copy_sv);
			*copy_sv = (stringview) {s->data, s->size};
			break;
		}
		default:
			break;
		}
	}

	record* dest = rg->data;
	record_swap(dest, &self->copy_data);

	return FQL_GOOD;
}

int subquery_get_record_at(reader* unused_self, node* unused_rg, const char* unused_loc)
{
	fputs("cannot seek through subquery\n", stderr);
	return FQL_FAIL;
}

int subquery_reset(reader* unused)
{
	fputs("cannot reset subquery\n", stderr);
	return FQL_FAIL;
}
