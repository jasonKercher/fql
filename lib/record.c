#include "record.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/stringview.h"

record* record_construct(record* rec, unsigned idx, unsigned n, _Bool owns_recs)
{
	*rec = (record) {
		 new_t_(vec, stringview) /* fields */
		,NULL                    /* _field_data */
		,csv_record_new()        /* libcsv_rec */
		,new_(string)            /* rec_cpy */
		,{ 0 }                   /* rec_raw */
		,idx                     /* idx */
		,0                       /* ref_count */
		,true                    /* is_recyclable */
	};

	vec_resize(rec->fields, n);
	if (owns_recs) {
		rec->_field_data = new_t_(vec, string);
		vec_resize(rec->_field_data, n);
		string* it = vec_begin(rec->_field_data);
		for (; it != vec_end(rec->_field_data); ++it) {
			string_construct(it);
		}
	}

	return rec;
}

void record_free(record* rec)
{
	record_destroy(rec);
	free_(rec);
}

void record_destroy(record* rec)
{
	delete_(vec, rec->fields);
	delete_(string, rec->rec_cpy);
	csv_record_free(rec->libcsv_rec);
}
