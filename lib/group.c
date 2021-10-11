#include "group.h"
#include "misc.h"
#include "record.h"
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
	vec_construct_(&self->expressions, expression*);
	vec_construct_(&self->aggregates, aggregate*);
	vec_construct_(&self->_composite, stringview);
	//vec_construct_(&self->_roots, dnode*);
	flex_construct(&self->group_data);

	return self;
}

void group_destroy(group* self)
{
	delete_if_exists_(compositemap, self->expr_map);
	compositemap_destroy(&self->val_map);
	vec_destroy(&self->aggregates);
	vec_destroy(&self->_composite);
	flex_destroy(&self->group_data);

	expression** it = vec_begin(&self->expressions);
	for (; it != vec_end(&self->expressions); ++it) {
		/* Avoid double free on aggregate */
		if ((*it)->expr == EXPR_AGGREGATE) {
			(*it)->expr = EXPR_UNDEFINED;
		}
		delete_(expression, *it);
	}
	vec_destroy(&self->expressions);
	//vec_destroy(&self->_roots);
}

void distinct_destroy(group* self)
{
	vec_clear(&self->expressions);
	group_destroy(self);
}

int group_reset(group* self)
{
	if (self == NULL) {
		return FQL_GOOD;
	}

	aggregate** it = vec_begin(&self->aggregates);
	for (; it != vec_end(&self->aggregates); ++it) {
		aggregate_reset(*it);
	}
	compositemap_clear(&self->val_map);
	flex_clear(&self->group_data);

	self->_dump_idx = 0;

	return FQL_GOOD;
}

void group_add_expression(group* self, expression* expr)
{
	expr->index = self->expressions.size;
	vec_push_back(&self->expressions, &expr);
	if (expr->expr != EXPR_AGGREGATE) {
		vec_add_one(&self->_composite);
	}
}

void group_cat_description(group* self, process* proc)
{
	expression** it = vec_begin(&self->expressions);
	for (; it != vec_end(&self->expressions); ++it) {
		if (it != vec_begin(&self->expressions)) {
			string_strcat(proc->plan_msg, ",");
		}
		expression_cat_description(*it, proc->plan_msg);
	}
}

int _add_agg_result(group* self, node* rg)
{
	aggregate** it = vec_begin(&self->aggregates);
	for (; it != vec_end(&self->aggregates); ++it) {
		struct aggresult* result = vec_add_one(&(*it)->results);
		*result = (struct aggresult) {0};
		if ((*it)->data_type == FIELD_STRING) {
			string_construct(&result->data.s);
		}
		try_((*it)->call__(*it, self, result, rg));
		++result->qty;
	}
	return FQL_GOOD;
}

int _update_agg_result(group* self, node* rg, unsigned idx)
{
	aggregate** it = vec_begin(&self->aggregates);
	for (; it != vec_end(&self->aggregates); ++it) {
		struct aggresult* result = vec_at(&(*it)->results, idx);
		try_((*it)->call__(*it, self, result, rg));
		++result->qty;
	}
	return FQL_GOOD;
}

int group_record(group* self, node* rg)
{
	size_t org_size = flex_size(&self->group_data);

	expression** exprs = vec_begin(&self->expressions);
	stringview* sv = vec_begin(&self->_composite);
	unsigned i = 0;
	for (; i < self->expressions.size; ++i) {
		if (exprs[i]->expr == EXPR_AGGREGATE) {
			continue;
		}
		switch (exprs[i]->field_type) {
		case FIELD_STRING:
			try_(expression_get_stringview(sv, exprs[i], rg));
			flex_push_back(&self->group_data, (void*)sv->data, sv->len);
			break;
		case FIELD_INT: {
			long num_i = 0;
			try_(expression_get_int(&num_i, exprs[i], rg));
			flex_push_back_str_int(&self->group_data, num_i);
			break;
		}
		case FIELD_FLOAT: {
			double num_f = 0;
			try_(expression_get_float(&num_f, exprs[i], rg));
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
		compositemap_set(&self->val_map, &self->_composite, &group_count);
		try_(_add_agg_result(self, rg));
		ret = 1;
	} else {
		flex_resize(&self->group_data, org_size);
		try_(_update_agg_result(self, rg, *idx_ptr));
	}

	return ret;
}

/* TODO: eliminate switch with a function pointer to
 *       a function that dumps the aggregate results.
 */
void _read_aggregate(expression* agg_expr, string* raw, stringview* sv, size_t idx)
{
	aggregate* agg = agg_expr->field.agg;
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

	record_resize(rec, self->expressions.size);

	expression** group_exprs = vec_begin(&self->expressions);
	stringview* rec_svs = vec_begin(&rec->fields);

	record_clear_strings(rec, rec->group_strings);

	unsigned i = 0;
	for (; i < self->expressions.size; ++i) {
		if (group_exprs[i]->expr == EXPR_AGGREGATE) {
			string* s = record_generate_groupby_string(rec);
			_read_aggregate(group_exprs[i], s, &rec_svs[i], self->_dump_idx);
			continue;
		}
		rec_svs[i] = flex_pair_at(&self->group_data, idx++);
	}

	++self->_dump_idx;
	return FQL_GOOD;
}
