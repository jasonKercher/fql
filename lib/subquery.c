#include "csv.h"
#include "expression.h"
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
	subquery* self = reader->reader_data;
	vec* expressions = self->select->_selection_exprs;

	record* rec = rg->data;
	record_clear_strings(&self->copy_data, self->copy_data.subquery_strings);

	vec_clear(&self->copy_data.fields);

	if (reader->random_access_file) {
		rec->offset = ftello(reader->random_access_file);
	}
	expression** it = vec_begin(expressions);
	for (; it != vec_end(expressions); ++it) {
		stringview* copy_sv = vec_add_one(&self->copy_data.fields);
		try_(expression_get_stringview(copy_sv, *it, rg));

		if (reader->random_access_file) {
			if (it != vec_begin(expressions)) {
				fputc('|', reader->random_access_file);
			}
			struct csv_field field = {copy_sv->data, copy_sv->len};
			csv_write_field(reader->subquery_writer, &field);
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

	if (reader->random_access_file) {
		fputc('\n', reader->random_access_file);
	}

	record_swap(rec, &self->copy_data);

	return FQL_GOOD;
}

int subquery_get_record_at(reader* unused_self, node* unused_rg, size_t unused_offset)
{
	fputs("cannot seek through subquery\n", stderr);
	return FQL_FAIL;
}

int subquery_reset(reader* unused)
{
	fputs("cannot reset subquery\n", stderr);
	return FQL_FAIL;
}

/* If we have been reset from a join, we will no longer
 * need to read from the subquery. We are now 100% backed
 * by a file, so convert reader to a libcsv reader.
 */
int subquery_reset_from_join(reader* reader)
{
	reader_stop_file_backed_input(reader);
	csv_writer_free(reader->subquery_writer);

	subquery* self = reader->reader_data;

	csv_reader* csv = csv_reader_new();
	csv_reader_set_delim(csv, "|");
	reader->reader_data = csv;
	reader->free__ = &libcsv_free;
	reader->get_record__ = &libcsv_get_record;
	reader->get_record_at__ = &libcsv_get_record_at;
	reader->reset__ = &libcsv_reset;

	if (csv_reader_open_mmap(csv, string_c_str(reader->random_access_filename))) {
		csv_perror();
		return FQL_FAIL;
	}

	subquery_free(self);

	return FQL_GOOD;
}

int subquery_start_file_backed_input(reader* reader)
{
	reader_start_file_backed_input(reader);
	reader->subquery_writer = csv_writer_new();
	csv_writer_set_delim(reader->subquery_writer, "|");
	reader->reset__ = &subquery_reset_from_join;
	csv_writer_set_file(reader->subquery_writer, reader->random_access_file);
	return FQL_GOOD;
}
