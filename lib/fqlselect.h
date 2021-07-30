#ifndef SELECT_H
#define SELECT_H

#include "query.h"
#include "writer.h"
#include "schema.h"
#include "operation.h"
#include "expression.h"

struct record;
struct fqlselect;
struct inlist;
typedef int (*select_fn)(struct fqlselect*, struct node*);

struct fqlselect {
	enum op oper_type;
	struct vec* api;
	struct schema* schema;
	struct writer* writer;
	set* list_data;
	struct expression* const_dest;
	struct node* union_selects;
	struct vec* _selection_exprs;
	select_fn select__;
	size_t offset;
	size_t rows_affected;
	size_t top_count;
	unsigned rownum;
	bool is_const;
};
typedef struct fqlselect fqlselect;

struct fqlselect* fqlselect_construct(struct fqlselect*);
void fqlselect_destroy(struct fqlselect*);

unsigned fqlselect_get_field_count(struct fqlselect*);
void fqlselect_add_expression(struct fqlselect*, struct expression*);
int fqlselect_connect_api(struct query*, struct vec*);
void fqlselect_apply_process(struct query*, struct fql_plan*, bool);
void fqlselect_apply_expression_alias(struct fqlselect*, const char* alias);
int fqlselect_set_as_inlist(struct fqlselect*, struct inlist*);
int fqlselect_resolve_type_from_subquery(struct expression*);
int fqlselect_resolve_final_types(struct fqlselect*);
int fqlselect_next_union(struct fqlselect*);
void fqlselect_preop(struct fqlselect*, struct query*);

#endif /* SELECT_H */
