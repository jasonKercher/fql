#ifndef TABLE_H
#define TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "util/stack.h"
#include "util/hmap.h"
#include "util/pmap.h"
#include "util/stringy.h"
#include "schema.h"
//#include "reader.h"
#include "fqlimits.h"


/** Table **/
struct table {
        struct reader* reader;
        struct schema* schema;
        String name;
};
typedef struct table Table;

struct table* table_new();
struct table* table_construct(struct table*);
void table_free(struct table*);


/** Source **/
enum source_type {
        SOURCE_TABLE,
        SOURCE_SUBQUERY,
};

enum join_type {
        JOIN_FROM,
        JOIN_INNER,
        JOIN_LEFT,
        JOIN_RIGHT,
        JOIN_FULL,
        JOIN_CROSS,
};

struct source {
        struct table* table;
        struct logicgroup* condition;
        struct vec* validation_list;
        struct process* read_proc;
        void* join_data;
        String alias;
        size_t idx;
        enum source_type source_type;
        enum join_type join_type;
};
typedef struct source Source;

struct source* source_new(struct table*,
                          const char* alias,
                          size_t idx,
                          enum source_type,
                          enum join_type);
struct source* source_construct(struct source*,
                           struct table*,
                           const char* alias,
                           size_t idx,
                           enum source_type,
                           enum join_type);
void source_free(struct source*);
void source_destroy(struct source*);

#define HASH_JOIN_MIN_SIZE 128

struct hashjoin {
        Pmap hash_data;
        struct column* left_col;
        struct column* right_col;
        Vec* recs;
        enum join_side state;
        unsigned rec_idx;
};

struct hashjoin* hashjoin_new();
void hashjoin_free(struct hashjoin*);
void source_hash_join_init(struct source*);


#ifdef __cplusplus
}
#endif

#endif /* TABLE_H */


