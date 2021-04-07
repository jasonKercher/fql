#include "record.h"
#include "util/vec.h"
#include "util/util.h"
#include "util/stringview.h"

Record* record_new(unsigned idx)
{
	Record* new_record = NULL;
	malloc_(new_record, sizeof(*new_record));

	return record_construct(new_record, idx);
}

Record* record_construct(Record* rec, unsigned idx)
{
	*rec = (Record) {
		 vec_new_(StringView)   /* fields */
		,csv_record_new()       /* libcsv_rec */
		,string_new()           /* rec_cpy */
		,{ 0 }                  /* rec_raw */
		,idx                    /* idx */
		,0                      /* ref_count */
		,true                   /* is_recyclable */
	};

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
