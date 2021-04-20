#include "record.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/stringview.h"

Record* record_new(unsigned idx, unsigned n, _Bool owns_recs)
{
	Record* new_record = NULL;
	malloc_(new_record, sizeof(*new_record));

	return record_construct(new_record, idx, n, owns_recs);
}

Record* record_construct(Record* rec, unsigned idx, unsigned n, _Bool owns_recs)
{
	*rec = (Record) {
		 vec_new_(StringView)   /* fields */
		,NULL                   /* _field_data */
		,csv_record_new()       /* libcsv_rec */
		,string_new()           /* rec_cpy */
		,{ 0 }                  /* rec_raw */
		,idx                    /* idx */
		,0                      /* ref_count */
		,true                   /* is_recyclable */
	};

	vec_resize(rec->fields, n);
	if (owns_recs) {
		rec->_field_data = vec_new_(String);
		vec_resize(rec->_field_data, n);
		String* it = vec_begin(rec->_field_data);
		for (; it != vec_end(rec->_field_data); ++it) {
			string_construct(it);
		}
	}

	return rec;
}

void record_free(Record* rec)
{
	record_destroy(rec);
	free_(rec);
}

void record_destroy(Record* rec)
{
	vec_free(rec->fields);
	csv_record_free(rec->libcsv_rec);
	string_free(rec->rec_cpy);
}
