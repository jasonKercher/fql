#ifndef EXPRESSION_H
#define EXPRESSION_H


#ifdef __cplusplus
extern "C" {
#endif

/** Expression **/
enum expr_e {
        EXPR_NONE,
        EXPR_COLUMN_NAME,
        EXPR_CONST,
        EXPR_SOURCE,
        EXPR_SUBQUERY,
        EXPR_SUBQUERY_CONST,
};

struct expression {
        enum expr_e type;
        void* expr;
};
typedef struct expression Expression;

struct expression* expression_new(enum expr_e type, void*);
void expression_free(struct expression*);

#ifdef __cplusplus
}
#endif

#endif /* EXPRESSION_H */


