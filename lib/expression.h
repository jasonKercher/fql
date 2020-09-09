#ifndef EXPRESSION_H
#define EXPRESSION_H

#define EXPR_NONE               0
#define EXPR_COLUMN             1
#define EXPR_CONST              2
#define EXPR_SUBQUERY           3
#define EXPR_SUBQUERY_CONST     4

#ifdef __cplusplus
extern "C" {
#endif

struct expression_t {
        void* data;
        int type;
};

typedef struct expression_t expression_t;

#ifdef __cplusplus
}
#endif

#endif /* EXPRESSION_H */
