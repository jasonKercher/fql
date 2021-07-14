#ifndef SWITCHCASE_H
#define SWITCHCASE_H

#include "util/vec.h"

struct expression;
struct recgroup;
union field;

enum switch_state {
	SWITCH_STATIC,
	SWITCH_STATIC_CMP,
	SWITCH_LOGIC_GROUP,
	SWITCH_VALUE,
};

struct switchcase {
	struct vec tests;
	struct vec values;
	struct vec test_expr_refs;
	struct expression* static_expr;
	enum switch_state state;
};
typedef struct switchcase switchcase;

struct switchcase* switchcase_construct(struct switchcase*);
void switchcase_destroy(struct switchcase*);

int switchcase_add_expression(struct switchcase*, struct expression*);
int switchcase_eval(struct switchcase*, union field* ret, struct recgroup*);

#endif  /* SWITCHCASE_H */
