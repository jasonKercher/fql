#include "record.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/stringview.h"

record* record_construct(record* self)
{
	*self = (record) {
	        {0},   /* fields */
	        NULL,  /* subquery_strings */
	        NULL,  /* group_strings */
	        NULL,  /* libcsv_rec */
	        {0},   /* rec_ref */
	        0,     /* offset */
	        0,     /* max_subquery_strings */
	        0,     /* max_group_strings */
	        false, /* is_ref */
	};

	vec_construct_(&self->fields, stringview);

	return self;
}

void record_destroy(record* self)
{
	if (self->group_strings != NULL) {
		string* it = vec_begin(self->group_strings);
		for (; it != vec_end(self->group_strings); ++it) {
			string_destroy(it);
		}
		delete_(vec, self->group_strings);
	}

	if (self->subquery_strings != NULL) {
		string* it = vec_begin(self->subquery_strings);
		for (; it != vec_end(self->subquery_strings); ++it) {
			string_destroy(it);
		}
		delete_(vec, self->subquery_strings);
	}

	if (self->libcsv_rec != NULL) {
		csv_record_free(self->libcsv_rec);
	}

	vec_destroy(&self->fields);
}

void record_resize(record* self, unsigned size)
{
	vec_resize(&self->fields, size);
}

void record_swap(record* self, record* src)
{
	vec_clear(&self->fields);
	vec_extend(&self->fields, &src->fields);

	/* save allocations by swapping */
	vec* swap_strings = self->subquery_strings;
	self->subquery_strings = src->subquery_strings;
	src->subquery_strings = swap_strings;

	unsigned swap_count = self->max_subquery_count;
	self->max_subquery_count = src->max_subquery_count;
	src->max_subquery_count = swap_count;
}

void record_clear_strings(record* self, vec* strings)
{
	if (strings != NULL) {
		vec_clear(strings);
	}
}

string* record_generate_subquery_string(record* self)
{
	if (self->subquery_strings == NULL) {
		self->subquery_strings = new_t_(vec, string);
	}

	string* ret = vec_add_one(self->subquery_strings);
	if (self->subquery_strings->size <= self->max_subquery_count) {
		return ret;
	}

	unsigned i = (self->max_subquery_count) ? self->max_subquery_count : 0;
	for (; i < self->subquery_strings->size; ++i) {
		string_construct(ret);
	}

	self->max_subquery_count = self->subquery_strings->size;

	return ret;
}

string* record_generate_groupby_string(record* self)
{
	if (self->group_strings == NULL) {
		self->group_strings = new_t_(vec, string);
	}

	string* ret = vec_add_one(self->group_strings);
	if (self->group_strings->size <= self->max_group_count) {
		return ret;
	}

	unsigned i = (self->max_group_count) ? self->max_group_count : 0;
	for (; i < self->group_strings->size; ++i) {
		string_construct(ret);
	}

	self->max_group_count = self->group_strings->size;

	return ret;
}

recgroup* recgroup_construct(recgroup* self, unsigned _fifo_idx)
{
	memset(self, 0, sizeof(*self));

	/* recgroup struct is squeezed into 16
	 * bytes because it used to be the main
	 * type that passed through the fifos
	 */
	*((uint32_t*)self->_fifo_idx) = _fifo_idx;

	self->records = new_t_(vec, record);
	return self;
}

void recgroup_destroy(recgroup* self)
{
	record* it = vec_begin(self->records);
	for (; it != vec_end(self->records); ++it) {
		record_destroy(it);
	}
	delete_(vec, self->records);
}

/* fifo idx not directly accessible */
uint32_t recgroup_get_idx(const recgroup* self)
{
	uint32_t idx = *(uint32_t*)self->_fifo_idx;
	return idx & 0x00FFFFFF;
}

record* recgroup_rec_begin(const recgroup* self)
{
	return vec_at(self->records, 0);
}

record* recgroup_rec_back(const recgroup* self)
{
	return vec_at(self->records, self->records->size - 1);
}

record* recgroup_rec_at(const recgroup* self, unsigned idx)
{
	return vec_at(self->records, idx);
}

void recgroup_rec_set(recgroup* self, unsigned idx, const record* rec)
{
	vec_set(self->records, idx, rec);
}

void recgroup_rec_set_ref(recgroup* self, unsigned idx, const record* src)
{
	record* dest = recgroup_rec_at(self, idx);

	vec_clear(&dest->fields);
	vec_extend(&dest->fields, &src->fields);

	dest->rec_ref = src->rec_ref;

	dest->is_ref = true;
}

void recgroup_rec_push_back(recgroup* self, const record* src)
{
	vec_insert_at(self->records, self->records->size, src, 1);
	++self->_max_sources;
}


void recgroup_resize(recgroup* self, unsigned size)
{
	vec_resize(self->records, size);
	if (size <= self->_max_sources) {
		return;
	}

	unsigned i = (self->_max_sources) ? self->_max_sources - 1 : 0;
	for (; i < size; ++i) {
		record* rec = vec_at(self->records, i);
		record_construct(rec);
	}

	self->_max_sources = size;
}

record* recgroup_rec_add_one(recgroup* self)
{
	recgroup_resize(self, self->records->size + 1);
	return recgroup_rec_back(self);
}

record* recgroup_rec_add_one_front(recgroup* self)
{
	record* new_rec = vec_add_one_front(self->records);

	if (self->records->size > self->_max_sources) {
		record_construct(new_rec);
		++self->_max_sources;
	}

	return vec_begin(self->records);
}

void recgroup_rec_add_front(recgroup* self, size_t n)
{
	unsigned i = 0;
	for (; i < n; ++i) {
		recgroup_rec_add_one_front(self);
	}
}

record* recgroup_rec_pop(recgroup* self)
{
	record* back = vec_pop_back(self->records);
	--self->_max_sources;
	return back;
}

void recgroup_clear_refs(recgroup* self)
{
	unsigned i = 0;
	while (i < self->records->size) {
		record* rec = vec_at(self->records, i);
		if (rec->is_ref) {
			vec_erase_at(self->records, i, 1);
			--self->_max_sources;
		} else {
			++i;
		}
	}
}
