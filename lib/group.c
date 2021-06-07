#include "group.h"
#include "misc.h"
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
	vec_construct_(&self->_composite, stringview);
	flex_construct(&self->group_data);

	return self;
}

void group_destroy(group* self)
{
	if (self->expr_map != NULL) {
		delete_(compositemap, self->expr_map);
	}
	compositemap_destroy(&self->val_map);
	vec_destroy(&self->columns);
	vec_destroy(&self->aggregates);
	vec_destroy(&self->_composite);
	flex_destroy(&self->group_data);
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
		*result = (struct aggresult) {0};
		if ((*it)->data_type == FIELD_STRING) {
			string_construct(&result->data.s);
		}
		try_((*it)->call__(*it, self, result, recs));
		++result->qty;
	}
	return FQL_GOOD;
}

int _update_agg_result(group* self, vec* recs, unsigned idx)
{
	aggregate** it = vec_begin(&self->aggregates);
	for (; it != vec_end(&self->aggregates); ++it) {
		struct aggresult* result = vec_at(&(*it)->results, idx);
		try_((*it)->call__(*it, self, result, recs));
		++result->qty;
	}
	return FQL_GOOD;
}
int group_record(group* self, vec* recs)
{
	size_t org_size = flex_size(&self->group_data);

	column** cols = vec_begin(&self->columns);
	stringview* sv = vec_begin(&self->_composite);
	unsigned i = 0;
	for (; i < self->columns.size; ++i) {
		if (cols[i]->expr == EXPR_AGGREGATE) {
			continue;
		}
		switch (cols[i]->field_type) {
		case FIELD_STRING:
			try_(column_get_stringview(sv, cols[i], recs));
			flex_push_back(&self->group_data,
			               (void*)sv->data,
			               sv->len);
			break;
		case FIELD_INT: {
			long num_i = 0;
			try_(column_get_int(&num_i, cols[i], recs));
			flex_push_back_str_int(&self->group_data, num_i);
			break;
		}
		case FIELD_FLOAT: {
			double num_f = 0;
			try_(column_get_float(&num_f, cols[i], recs));
			flex_push_back_str_float(&self->group_data, num_f);
		}
		default:;
		}
	}

	for (i = 0; i < self->_composite.size; ++i) {
		sv[i] = flex_pair_at(&self->group_data, org_size + i);
	}

	unsigned group_count = self->val_map.values->size;
	unsigned* idx_ptr = compositemap_get(&self->val_map, &self->_composite);
	int ret = 0;
	if (idx_ptr == NULL) {
		compositemap_set(&self->val_map,
		                 &self->_composite,
		                 &group_count);
		try_(_add_agg_result(self, recs));
		ret = 1;
	} else {
		flex_resize(&self->group_data, org_size);
		try_(_update_agg_result(self, recs, *idx_ptr));
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
	default:;
	}
}

int group_dump_record(group* self, record* rec)
{
	if (self->_dump_idx >= self->val_map.values->size) {
		return -1;
	}

	size_t idx = self->_composite.size * self->_dump_idx;

	column** group_cols = vec_begin(&self->columns);
	stringview* rec_svs = vec_begin(rec->fields);

	unsigned i = 0;
	for (; i < self->columns.size; ++i) {
		if (group_cols[i]->expr == EXPR_AGGREGATE) {
			_read_aggregate(group_cols[i],
			                vec_at(rec->_field_data, i),
			                &rec_svs[i],
			                self->_dump_idx);
			continue;
		}
		rec_svs[i] = flex_pair_at(&self->group_data, idx++);
	}

	++self->_dump_idx;
	return FQL_GOOD;
}
