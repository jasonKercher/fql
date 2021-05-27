#ifndef SELECT_H
#define SELECT_H

#include "query.h"
#include "operation.h"
#include "writer.h"
#include "schema.h"
#include "column.h"

struct record;
struct fqlselect;
typedef int(*select_fn)(struct fqlselect*, struct vec*);

struct fqlselect {
	enum op oper_type;
	struct vec* api;
	struct schema* schema;
	struct writer* writer;
	select_fn select__;
	size_t offset;
};
typedef struct fqlselect fqlselect;

struct fqlselect* fqlselect_construct(struct fqlselect*);
void fqlselect_destroy(struct fqlselect*);

_Bool fqlselect_has_delim(struct fqlselect*);
void fqlselect_set_delim(struct fqlselect*, const char*);
void fqlselect_add_column(struct fqlselect*, struct column*);
int fqlselect_connect_api(struct query*, struct vec*);
void fqlselect_apply_process(struct query*, struct fql_plan*);
void fqlselect_apply_column_alias(struct fqlselect*, const char* alias);
int fqlselect_writer_open(struct fqlselect*, const char* file_name);
void fqlselect_preflight(struct fqlselect*, struct query*);
int fqlselect_close(struct fqlselect*);
char* fqlselect_take_filename(struct fqlselect*);
const char* fqlselect_get_tempname(struct fqlselect* self);

void fqlselect_preop(struct fqlselect*, struct query*);
int fqlselect_subquery_record(struct reader*, struct record*);
int fqlselect_subquery_reset(struct reader*);

#endif  /* SELECT_H */
