#ifndef TABLE_H
#define TABLE_H

#define TABLE_NAME_MAX 129

#include "reader.h"

#ifdef __cplusplus
extern "C" {
#endif

struct table_t {
        reader_t* source;
        char name[TABLE_NAME_MAX];
        char alias[TABLE_NAME_MAX];
};

typedef struct table_t table_t;

#ifdef __cplusplus
}
#endif

#endif /* TABLE_H */
