#ifndef ORDER_H
#define ORDER_H

#include "util/vec.h"

struct order {
	struct vec columns;
};
typedef struct order order;

struct order* order_construct(order*);
void order_destroy(order*);


#endif  /* ORDER_H */
