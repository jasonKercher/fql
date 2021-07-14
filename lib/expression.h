#ifndef COLUMN_H
#define COLUMN_H

#include "table.h"
#include "field.h"
#include "record.h"
#include "fqlimits.h"
#include "util/stringy.h"

struct stringview;

/** expression **/
enum expr_type {
	EXPR_UNDEFINED,
	EXPR_COLUMN_NAME,
	EXPR_SWITCH_CASE,
	EXPR_ROW_NUMBER,  /* keyword __ROWNUM */
	EXPR_FULL_RECORD, /* keyword __REC */
	EXPR_GROUPING,
	EXPR_CONST,
	EXPR_ASTERISK,
	EXPR_FUNCTION,
	EXPR_AGGREGATE,
	EXPR_SOURCE,
	EXPR_SUBQUERY,
};

/** expression **/
struct expression {
	enum expr_type expr;
	struct table* table;
	struct expression* data_source;
	struct query* subquery;
	unsigned* rownum_ref;
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
	bool descending;
};
typedef struct expression expression;

struct expression*
expression_construct(struct expression*, enum expr_type, void* data, const char*);
void expression_destroy(void*);

struct expression* expression_copy(const struct expression*);

void expression_link(struct expression* dest, struct expression* src);
void expression_cat_description(struct expression*, string*);
int expression_try_assign_source(struct expression*, struct table*);

/* data access */
int expression_get_int(long*, struct expression*, struct recgroup*);
int expression_get_float(double*, struct expression*, struct recgroup*);
int expression_get_stringview(struct stringview*, struct expression*, struct recgroup*);

#endif /* COLUMN_H */
