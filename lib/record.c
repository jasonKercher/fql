#include "record.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/stringview.h"

record* record_construct(record* self)
{
	*self = (record) {
	        {0},  /* fields */
	        NULL, /* string_fields */
	        NULL, /* libcsv_rec */
	        {0},  /* rec_ref */
	        0,    /* offset */
	};

	vec_construct_(&self->fields, stringview);

	return self;
}

void record_destroy(record* self)
{
	if (self->field_strings != NULL) {
		string* it = vec_begin(self->field_strings);
		for (; it != vec_end(self->field_strings); ++it) {
			string_destroy(it);
		}
		delete_(vec, self->field_strings);
	}
	vec_destroy(&self->fields);
	csv_record_free(self->libcsv_rec);
}

void record_resize(record* self, unsigned size)
{
	vec_resize(&self->fields, size);
}

void record_init_strings(record* self)
{
	vec_resize(self->field_strings, self->fields.size);
	string* it = vec_begin(self->field_strings);
	for (; it != vec_end(self->field_strings); ++it) {
		string_construct(it);
	}
}

recgroup* recgroup_construct(recgroup* self, unsigned idx)
{
	*self = (recgroup) {
	        {0}, /* records*/
	        0,   /* max_sources */
	        0,   /* select_len */
	        idx, /* idx */
	};

	vec_construct_(&self->records, record);
	return self;
}

void recgroup_destroy(recgroup* self)
{
	record* it = vec_begin(&self->records);
	for (; it != vec_end(&self->records); ++it) {
		record_destroy(it);
	}
	vec_destroy(&self->records);
}

struct record* recgroup_rec_at(const recgroup* self, unsigned idx)
{
	return vec_at(&self->records, idx);
}

void recgroup_resize(recgroup* self, unsigned size)
{
	vec_resize(&self->records, size);
	if (size <= self->max_sources) {
		return;
	}

	unsigned i = (self->max_sources) ? self->max_sources - 1 : 0;
	for (; i < size; ++i) {
		record* rec = vec_at(&self->records, i);
		record_construct(rec);
	}

	self->max_sources = size;
}
