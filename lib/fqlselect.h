#ifndef SELECT_H
#define SELECT_H

#include "query.h"
#include "writer.h"
#include "schema.h"
#include "expression.h"
#include "operation.h"

struct record;
struct fqlselect;
struct inlist;
typedef int (*select_fn)(struct fqlselect*, struct recgroup*);

struct fqlselect {
	enum op oper_type;
	struct vec* api;
	struct schema* schema;
	struct writer* writer;
	set* list_data;
	struct expression* const_dest;
	struct queue* union_expression_vecs;
	struct vec* _selection_exprs;
	select_fn select__;
	size_t offset;
	unsigned rownum;
};
typedef struct fqlselect fqlselect;

struct fqlselect* fqlselect_construct(struct fqlselect*);
void fqlselect_destroy(struct fqlselect*);

unsigned fqlselect_get_field_count(struct fqlselect*);
void fqlselect_set_delim(struct fqlselect*, const char* delim);
void fqlselect_set_rec_terminator(struct fqlselect*, const char* term);
void fqlselect_set_schema(struct fqlselect*, const struct schema*);
void fqlselect_add_expression(struct fqlselect*, struct expression*);
int fqlselect_connect_api(struct query*, struct vec*);
void fqlselect_apply_process(struct query*, struct fql_plan*, bool);
void fqlselect_apply_expression_alias(struct fqlselect*, const char* alias);
int fqlselect_set_as_inlist(struct fqlselect*, struct inlist*);
int fqlselect_resolve_type_from_subquery(struct expression*);
int fqlselect_writer_init(struct fqlselect*, struct query*, struct fql_handle*);
int fqlselect_next_union(struct fqlselect*);
void fqlselect_preop(struct fqlselect*, struct query*);

#endif /* SELECT_H */
