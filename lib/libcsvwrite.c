#include "writer.h"
#include <csv.h>
#include "expression.h"
#include "record.h"
#include "misc.h"
#include "util/stringview.h"

int libcsv_write_record(writer* writer, vec* expr_vec, node* rg, FILE* outstream)
{
	csv_writer* handle = writer->writer_data;

	FILE* store_stream = NULL;
	if (outstream == NULL) {
		outstream = csv_writer_get_file(handle);
	} else {
		store_stream = csv_writer_get_file(handle);
		csv_writer_set_file(handle, outstream);
	}

	const struct csv_field delim = csv_writer_get_delim_field(handle);
	const struct csv_field terminator = csv_writer_get_terminator_field(handle);
	int len = 0;

	expression** exprs = vec_begin(expr_vec);
	unsigned i = 0;
	for (; i < expr_vec->size; ++i) {
		stringview sv;
		if (i > 0) {
			fputs(delim.data, outstream);
			len += delim.len;
		}

		if (exprs[i]->expr == EXPR_ASTERISK) {
			int quote_store = handle->quotes;
			handle->quotes = QUOTE_NONE;
			record* rec = node_data_at(rg, exprs[i]->src_idx);
			struct csv_field field = {rec->rec_ref.data, rec->rec_ref.len};
			len += csv_write_field(handle, &field);
			handle->quotes = quote_store;
		} else {
			if (writer->strict && exprs[i]->field_type != FIELD_STRING) {
				try_(expression_type_check(exprs[i], rg));
			}
			struct csv_field field = {NULL, 0};
			if (try_(expression_get_stringview(&sv, exprs[i], rg))
			    != FQL_NULL) {
				field = (struct csv_field) {sv.data, sv.len};
			}
			len += csv_write_field(handle, &field);
		}
	}

	fputs(terminator.data, outstream);
	len += terminator.len;

	if (store_stream) {
		csv_writer_set_file(handle, store_stream);
	}

	return len;
}
