#include "record.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/stringview.h"

record* record_construct(record* self, unsigned idx, unsigned n, bool owns_recs)
{
	*self = (record) {
	        new_t_(vec, stringview), /* fields */
	        NULL,                    /* _field_data */
	        csv_record_new(),        /* libcsv_rec */
	        {0},                     /* rec_raw */
	        0,                       /* offset */
	        0,                       /* select_len */
	        idx,                     /* idx */
	        0                        /* ref_count */
	};

	vec_resize(self->fields, n);
	if (owns_recs) {
		self->_field_data = new_t_(vec, string);
		vec_resize(self->_field_data, n);
		string* it = vec_begin(self->_field_data);
		for (; it != vec_end(self->_field_data); ++it) {
			string_construct(it);
		}
	}

	return self;
}

void record_destroy(record* self)
{
	delete_(vec, self->fields);
	if (self->_field_data != NULL) {
		string* it = vec_begin(self->_field_data);
		for (; it != vec_end(self->_field_data); ++it) {
			string_destroy(it);
		}
		delete_(vec, self->_field_data);
	}
	csv_record_free(self->libcsv_rec);
}
