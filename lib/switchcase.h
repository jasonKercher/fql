#ifndef SWITCHCASE_H
#define SWITCHCASE_H

#include "query.h"
#include "util/vec.h"

struct logicgroup;
struct expression;
struct stringview;
struct recgroup;

enum switch_state {
	SWITCH_STATIC,
	SWITCH_STATIC_CMP,
	SWITCH_LOGIC_GROUP,
	SWITCH_VALUE,
};

struct switchcase {
	struct vec tests;
	struct vec values;
	struct expression* static_expr;
	struct stack* return_logic_stack;
	struct vec* return_joinable;
	enum switch_state state;
	enum mode return_mode;
	enum logic_mode return_logic_mode;
};
typedef struct switchcase switchcase;

struct switchcase* switchcase_construct(struct switchcase*);
void switchcase_destroy(struct switchcase*);

void switchcase_add_value(struct switchcase*, const struct expression*);
void switchcase_add_logicgroup(struct switchcase*, struct logicgroup*);
int switchcase_resolve_type(struct switchcase*, struct expression*);

int switchcase_eval_to_int(struct switchcase*, long* ret, struct recgroup*);
int switchcase_eval_to_float(struct switchcase*, double* ret, struct recgroup*);
int switchcase_eval_to_stringview(struct switchcase*,
                                  struct stringview*,
                                  struct recgroup*);

#endif  /* SWITCHCASE_H */
