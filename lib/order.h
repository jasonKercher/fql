#ifndef ORDER_H
#define ORDER_H

#include "column.h"
#include "util/vec.h"

struct order {
	struct vec columns;
	struct vec _index_pairs;
	struct vec _raw;
	struct vec* _views;
};
typedef struct order order;

struct order* order_construct(struct order*);
void order_destroy(struct order*);

int order_add_column(struct order*, struct column*);
void order_cat_description(struct order*, struct process*);
int order_add_record(struct order*, struct vec*);

#endif  /* ORDER_H */
