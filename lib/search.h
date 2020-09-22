#ifndef SEARCH_H
#define SEARCH_H

#include "expression.h"

struct condition {
        struct expression* expr[2];
        int data_type;
        int comparison;
};

#endif /* SEARCH_H */
