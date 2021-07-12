#include "reader.h"
#include "fqlselect.h"
#include "misc.h"

subquery* subquery_construct(subquery* self, fqlselect* select)
{
	*self = (subquery) {
	        select, /* select */
	        NULL,   /* _rg_ref */
	        {0}     /* copy_data */
	};

	/* basically just swap space */
	record_construct(&self->copy_data);

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
int subquery_get_record(reader* reader, record* dest)
{
	subquery* self = reader->reader_data;
	vec* columns = self->select->_selection_cols;

	record_clear_strings(&self->copy_data, dest->subquery_strings);
	vec_clear(&self->copy_data.fields);

	column** it = vec_begin(columns);
	for (; it != vec_end(columns); ++it) {
		stringview* copy_sv = vec_add_one(&self->copy_data.fields);
		try_(column_get_stringview(copy_sv, *it, self->_rg_ref));

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
			string* s = record_generate_string(
			        &self->copy_data,
			        self->copy_data.subquery_strings,
			        &self->copy_data.max_subquery_count);
			string_copy_from_stringview(s, copy_sv);
			*copy_sv = (stringview) {s->data, s->size};
			break;
		}
		default:
			break;
		}
	}

	/* put everything into a single record */
	recgroup_resize(self->_rg_ref, 1);
	record_swap(dest, &self->copy_data);

	return FQL_GOOD;
}

int subquery_get_record_at(reader* unused_self,
                           record* unused_rec,
                           const char* unused_loc)
{
	fputs("cannot seek through subquery\n", stderr);
	return FQL_FAIL;
}

int subquery_reset(reader* unused)
{
	fputs("cannot reset subquery\n", stderr);
	return FQL_FAIL;
}
