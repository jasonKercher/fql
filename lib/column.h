#ifndef COLUMN_H
#define COLUMN_H

#define COLUMN_NAME_MAX 129

#include "table.h"

#ifdef __cplusplus
extern "C" {
#endif

struct column_t {
        table_t* table;
        char name[COLUMN_NAME_MAX];
        char alias[COLUMN_NAME_MAX];
        unsigned location;
        unsigned width;
};

typedef struct column_t column_t;

#ifdef __cplusplus
}
#endif

#endif /* COLUMN_H */
