#include "order.h"
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "record.h"
#include "field.h"
#include "process.h"
#include "util/util.h"

struct _entry {
	size_t idx;
	size_t offset;
	unsigned len;
};

order* order_construct(order* self, const char* filename)
{
	*self = (order) {
		 { 0 }  /* columns */
		,{ 0 }  /* entries */
		,{ 0 }  /* order_data */
		,{ 0 }  /* filename */
		,stdout /* dump_file */
	};

	vec_construct_(&self->columns, column*);
	vec_construct_(&self->entries, struct _entry);
	flex_construct(&self->order_data);
	string_construct_from_char_ptr(&self->filename, filename) ;
	return self;
}

void order_destroy(order* self)
{
	vec_destroy(&self->columns);
	vec_destroy(&self->entries);
	flex_destroy(&self->order_data);
	string_destroy(&self->filename);
	if (self->dump_file 
	 && self->dump_file != stdout) {
		fclose(self->dump_file);
	}
}

int order_add_column(order* self, column* col)
{
	vec_push_back(&self->columns, col);
	return FQL_GOOD;
}

void order_cat_description(order* self, process* proc)
{
	column** it = vec_begin(&self->columns);
	for (; it != vec_end(&self->columns); ++it) {
		if (it != vec_begin(&self->columns)) {
			string_strcat(proc->action_msg, ",");
		}
		column_cat_description(*it, proc->action_msg);
	}
}

int order_add_record(order* self, vec* recs)
{
	record** top = vec_begin(recs);
	struct _entry entry = {
		 flex_size(&self->order_data) /* idx */
		,(*top)->offset               /* offset */
		,(*top)->select_len           /* len */
	};
	column** cols = vec_begin(&self->columns);

	int i = 0;
	for (; i < self->columns.size; ++i) {
		switch (cols[i]->field_type) {
		case FIELD_INT:
		 {
			/* NOTE: storing numbers in binary */
			long num_i = 0;
			try_ (column_get_int(&num_i, cols[i], recs));
			flex_push_back_(&self->order_data, &num_i, long);
			break;
		 }
		case FIELD_FLOAT:
		 {
			double num_f = 0;
			try_ (column_get_float(&num_f, cols[i], recs));
			flex_push_back_(&self->order_data, &num_f, double);
			break;
		 }
		case FIELD_STRING:
		 {
			stringview sv;
			try_ (column_get_stringview(&sv, cols[i], recs));
			flex_push_back(&self->order_data,
				       (void*)sv.data,
				       sv.len);
			break;
		 }
		default:
			fputs("Undefined field type\n", stderr);
			return FQL_FAIL;
		}
		
		vec_push_back(&self->entries, &entry);
	}

	return FQL_GOOD;
}

int _compare(const void* a, const void* b, void* data)
{
	order* orderby = data;
	const struct _entry* p0 = a;
	const struct _entry* p1 = b;

	int ret = 0;
	column** it = vec_begin(&orderby->columns);
	for (; ret == 0 && it != vec_end(&orderby->columns); ++it) {
		switch ((*it)->field_type) {
		case FIELD_INT:
		 {
			long* num0 = flex_at(&orderby->order_data, p0->idx);
			long* num1 = flex_at(&orderby->order_data, p1->idx);
			ret = num_compare_(*num0, *num1);
			break;
		 }
		case FIELD_FLOAT:
		 {
			double* num0 = flex_at(&orderby->order_data, p0->idx);
			double* num1 = flex_at(&orderby->order_data, p1->idx);
			ret = num_compare_(*num0, *num1);
			break;
		 }
		case FIELD_STRING:
		 {
			stringview sv0 = flex_pair_at(
					&orderby->order_data,
					p0->idx
				);
			stringview sv1 = flex_pair_at(
					&orderby->order_data,
					p1->idx
				);
			ret = stringview_compare_nocase_rtrim(&sv0, &sv1);
		 }
		default:
			;
		}
	}
	return ret;
}

int _dump(order* self)
{
	int fd = open(string_c_str(&self->filename), O_RDONLY);
	if (fd == -1) {
		perror(string_c_str(&self->filename));
		return FQL_FAIL;
	}
	
	struct stat sb;
	if (fstat(fd, &sb) == -1) {
		perror(string_c_str(&self->filename));
		return FQL_FAIL;
	}
	
	char* select_mmap = mmap(NULL,
	                         sb.st_size,
	                         PROT_READ,
	                         MAP_PRIVATE,
	                         fd,
	                         0);
	if (select_mmap == MAP_FAILED) {
		perror(string_c_str(&self->filename));
		return FQL_FAIL;
	}
	madvise(select_mmap, sb.st_size, MADV_RANDOM);
	
	struct _entry* it = vec_begin(&self->entries);
	for (; it != vec_end(&self->entries); ++it) {
		fprintf(self->dump_file, 
			"%.*s",
			it->len,
			&select_mmap[it->offset]);
	}

	munmap(select_mmap, sb.st_size);
	return FQL_GOOD;
}

int order_sort(order* self)
{
	vec_sort_r(&self->entries, &_compare, self);
	return _dump(self);
}
