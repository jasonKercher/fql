#ifndef SELECT_H
#define SELECT_H

#include "util/queue.h"
#include "table.h"

#ifdef __cplusplus
extern "C" {
#endif

struct select_t {
        queue_t* select_list;  /* queue of expression_t */
        table_t* output_table; 
        queue_t* table_list;   /* queue of table_t */
};

typedef struct select_t select_t;

#ifdef __cplusplus
}
#endif

#endif /* SELECT_H */
