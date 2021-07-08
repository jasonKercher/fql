#include "record.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/stringview.h"

record* record_construct(record* self, unsigned idx)
{
	*self = (record) {
	        new_t_(vec, stringview), /* fields */
	        new_t_(vec, string),     /* _field_data */
	        csv_record_new(),        /* libcsv_rec */
	        {0},                     /* rec_raw */
	        0,                       /* offset */
	        0,                       /* max_size */
	        0,                       /* select_len */
	        idx,                     /* idx */
	        0                        /* ref_count */
	};

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

void rec_resize(record* self, unsigned size)
{
	vec_resize(self->fields, size);
	vec_resize(self->_field_data, size);
	if (size <= self->max_size) {
		return;
	}

	unsigned i = (self->max_size) ? self->max_size - 1 : 0;
	self->max_size = size;
	for (; i < size; ++i) {
		string* s = vec_at(self->_field_data, i);
		string_construct(s);
	}
}
