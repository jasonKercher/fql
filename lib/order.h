#ifndef ORDER_H
#define ORDER_H

#include <stdio.h>
#include "util/vec.h"
#include "util/flex.h"

struct query;
struct column;
struct process;

struct order {
	struct vec columns;
	struct vec entries;
	struct flex order_data;
	const char* in_filename;
	FILE* out_file;
};
typedef struct order order;

struct order* order_construct(struct order*, const char* in, char* out);
void order_destroy(struct order*);

int order_add_column(struct order*, struct column*);
void order_cat_description(struct order*, struct process*);
int order_add_record(struct order*, struct vec*);
int order_sort(struct order*);

#endif  /* ORDER_H */
