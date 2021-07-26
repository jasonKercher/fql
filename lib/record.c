#include "record.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/stringview.h"

record* record_construct(record* self, unsigned idx)
{
	*self = (record) {
	        {0},   /* fields */
	        NULL,  /* subquery_strings */
	        NULL,  /* group_strings */
	        NULL,  /* libcsv_rec */
	        {0},   /* rec_ref */
	        0,     /* offset */
	        idx,   /* rec_idx */
	        0,     /* select_len */
	        0,     /* max_subquery_strings */
	        0,     /* max_group_strings */
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

void record_copy(record* self, const record* src)
{
	vec_clear(&self->fields);
	vec_extend(&self->fields, &src->fields);

	self->rec_ref = src->rec_ref;
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

	unsigned i = self->max_subquery_count;
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

	unsigned i = self->max_group_count;
	for (; i < self->group_strings->size; ++i) {
		string_construct(ret);
	}

	self->max_group_count = self->group_strings->size;

	return ret;
}

//recgroup* recgroup_construct(recgroup* self, unsigned fifo_idx)
//{
//	*self = (recgroup) {
//	        NULL,     /* rec_list */
//	        0,        /* select_len */
//	        fifo_idx, /* _fifo_idx */
//	};
//	return self;
//}

//void recgroup_destroy(recgroup* _unused) { }
//
//record* recgroup_rec_begin(const recgroup* self)
//{
//	return node_at(self->rec_list, 0)->data;
//}
//
//record* recgroup_rec_back(const recgroup* self)
//{
//	return node_back(self->rec_list)->data;
//}
//
//record* recgroup_rec_at(const recgroup* self, unsigned idx)
//{
//	return node_at(self->rec_list, idx)->data;
//}
//
//void recgroup_rec_push_back(recgroup* self, node* src)
//{
//	node_enqueue_import(&self->rec_list, src);
//}
//
//void recgroup_rec_push_front(recgroup* self, node* newnode)
//{
//	node_push_import(&self->rec_list, newnode);
//}
//
//struct node* recgroup_rec_pop_back(recgroup* self)
//{
//	return node_dequeue_export(&self->rec_list);
//}
