#include "group.h"
#include "aggregate.h"
#include "util/util.h"
#include "util/stringview.h"

group* group_construct(group* self)
{
	memset(self, 0, sizeof(*self));
	compositemap_construct_(&self->val_map,
			        unsigned,
				128,
				HASHMAP_PROP_NOCASE | HASHMAP_PROP_RTRIM);
	vec_construct_(&self->columns, column*);
	vec_construct_(&self->aggregates, aggregate*);
	vec_construct_(&self->_indicies, size_t);
	vec_construct_(&self->_raw, char);
	vec_construct_(&self->_composite, stringview);

	size_t zero = 0;
	vec_push_back(&self->_indicies, &zero);

	return self;
}

void group_destroy(group* self)
{
	if (self->expr_map != NULL) {
		delete_ (compositemap, self->expr_map);
	}
	compositemap_destroy(&self->val_map);
	vec_destroy(&self->columns);
	vec_destroy(&self->aggregates);
	vec_destroy(&self->_indicies);
	vec_destroy(&self->_raw);
	vec_destroy(&self->_composite);
}

void group_add_column(group* self, column* col)
{
	col->location = self->columns.size;
	vec_push_back(&self->columns, &col);
	if (col->expr != EXPR_AGGREGATE) {
		vec_add_one(&self->_composite);
	}
}

void group_cat_description(group* self, process* proc)
{
	column** it = vec_begin(&self->columns);
	for (; it != vec_end(&self->columns); ++it) {
		if (it != vec_begin(&self->columns)) {
			string_strcat(proc->action_msg, ",");
		}
		column_cat_description(*it, proc->action_msg);
	}
}

int _add_agg_result(group* self, vec* recs)
{
	aggregate** it = vec_begin(&self->aggregates);
	for (; it != vec_end(&self->aggregates); ++it) {
		struct aggresult* result = vec_add_one(&(*it)->results);
		*result = (struct aggresult) { 0 };
		if ((*it)->data_type == FIELD_STRING) {
			string_construct(&result->data.s);
		}
		(*it)->call__(*it, self, result, recs);
	}
	return FQL_GOOD;
}

int _update_agg_result(group* self, vec* recs, unsigned idx)
{
	aggregate** it = vec_begin(&self->aggregates);
	for (; it != vec_end(&self->aggregates); ++it) {
		struct aggresult* result = vec_at(&(*it)->results, idx);
		(*it)->call__(*it, self, result, recs);
	}
	return FQL_GOOD;
}
int group_record(group* self, vec* recs)
{
	size_t raw_size = self->_raw.size;
	size_t index_count = self->_indicies.size;
	size_t running_size = raw_size;

	column** cols = vec_begin(&self->columns);
	stringview* sv = vec_begin(&self->_composite);
	int i = 0;
	for (; i < self->columns.size; ++i) {
		if (cols[i]->expr == EXPR_AGGREGATE) {
			continue;
		}
		switch (cols[i]->field_type) {
		case FIELD_STRING:
			try_ (column_get_stringview(sv, cols[i], recs));
			vec_append(&self->_raw, sv->data, sv->len);
			break;
		case FIELD_INT:
		 {
			long num_i = 0;
			try_ (column_get_int(&num_i, cols[i], recs));
			size_t old_size = self->_raw.size;
			unsigned len = snprintf(NULL, 0, "%ld", num_i);
			vec_resize(&self->_raw, old_size + len);
			char* end = vec_at(&self->_raw, old_size);
			snprintf(end, len+1, "%ld", num_i);
			stringview_nset(sv, end, len);
			break;
		 }
		case FIELD_FLOAT:
		 {
			double num_f = 0;
			try_ (column_get_float(&num_f, cols[i], recs));
			size_t old_size = self->_raw.size;
			unsigned len = snprintf(NULL, 0, "%f", num_f);
			vec_resize(&self->_raw, old_size + len);
			char* end = vec_at(&self->_raw, old_size);
			snprintf(end, len+1, "%f", num_f);
			stringview_nset(sv, end, len);
		 }
		default:
			;
		}

		/* NOTE: running size is incremented first, because it should
		 *       mark the beginning of the next element. _indicies.size
		 *       should always be number of elements + 1.
		 */
		running_size += sv->len;
		++sv;

		vec_push_back(&self->_indicies, &running_size);
	}

	unsigned group_count = self->val_map.values.size;
	unsigned* idx_ptr = compositemap_get(&self->val_map, &self->_composite);
	int ret = 0;
	if (idx_ptr == NULL) {
		compositemap_set(&self->val_map,
				 &self->_composite,
				 &group_count);
		_add_agg_result(self, recs);
		ret = 1;
	} else {
		vec_resize(&self->_raw, raw_size);
		vec_resize(&self->_indicies, index_count);
		_update_agg_result(self, recs, *idx_ptr);
	}

	return ret;
}

/* TODO: eliminate switch with a function pointer to
 *       a function that dumps the aggregate results.
 */
void _read_aggregate(column* agg_col, string* raw, stringview* sv, size_t idx)
{
	aggregate* agg = agg_col->field.agg;
	struct aggresult* result = vec_at(&agg->results, idx);
	switch (agg->data_type) {
	case FIELD_INT:
		string_sprintf(raw, "%ld", result->data.i);
		stringview_set_string(sv, raw);
		break;
	case FIELD_FLOAT:
		string_sprintf(raw, "%f", result->data.f);
		stringview_set_string(sv, raw);
		break;
	case FIELD_STRING:
		stringview_set_string(sv, &result->data.s);
	default:
		;
	}
}

int group_dump_record(group* self, record* rec)
{
	if (self->_dump_idx >= self->val_map.values.size) {
		return -1;
	}

	unsigned comp_count = self->_composite.size;
	size_t* idx = vec_at(&self->_indicies, comp_count * self->_dump_idx);

	int i = 0;
	column** group_cols = vec_begin(&self->columns);
	stringview* rec_svs = vec_begin(rec->fields);
	for (; i < self->columns.size; ++i) {
		if (group_cols[i]->expr == EXPR_AGGREGATE) {
			_read_aggregate(group_cols[i],
					vec_at(rec->_field_data, i),
					&rec_svs[i],
					self->_dump_idx);
			continue;
		}
		stringview_nset(&rec_svs[i],
				vec_at(&self->_raw, *idx),
				*(idx+1) - *idx);
		++idx;
	}

	++self->_dump_idx;
	return FQL_GOOD;
}
