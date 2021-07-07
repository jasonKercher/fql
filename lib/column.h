#ifndef COLUMN_H
#define COLUMN_H

#include "fqlimits.h"
#include "table.h"
#include "field.h"
#include "util/stringview.h"
#include "util/stringy.h"

/** expression **/
enum expr_type {
	EXPR_UNDEFINED,
	EXPR_FULL_RECORD, /* This is for keyword __REC */
	EXPR_COLUMN_NAME,
	EXPR_CONST,
	EXPR_ASTERISK,
	EXPR_FUNCTION,
	EXPR_AGGREGATE,
	EXPR_SOURCE,
	EXPR_SUBQUERY,
};

/** column **/
struct column {
	enum expr_type expr;
	struct table* table;
	struct column* data_source;
	struct query* subquery;
	string name;
	string alias;
	string table_name;
	string buf;
	enum field_type field_type;
	union field field;
	unsigned index;
	unsigned location;
	unsigned width;
	int src_idx;
	bool is_resolved_to_group;
	bool descending;
};
typedef struct column column;

struct column* column_construct(struct column*, enum expr_type, void* data, const char*);
void column_destroy(void*);

void column_link(struct column* dest, struct column* src);
void column_cat_description(struct column* col, string*);
int column_try_assign_source(struct column*, struct table*);

/* data access */
int column_get_int(long*, struct column*, struct vec*);
int column_get_float(double*, struct column*, struct vec*);
int column_get_stringview(struct stringview*, struct column*, struct vec*);

#endif /* COLUMN_H */
