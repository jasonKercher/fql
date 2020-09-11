#ifndef EXPRESSION_H
#define EXPRESSION_H


#ifdef __cplusplus
extern "C" {
#endif

/** Expression **/
#define EXPR_NONE               0
#define EXPR_COLUMN_NAME        1
#define EXPR_CONST              2
#define EXPR_SUBQUERY           3
#define EXPR_SUBQUERY_CONST     4

struct expression_t {
        void* expr;
        int type;
};
typedef struct expression_t expression_t;

expression_t* expression_new(int, void*);



#ifdef __cplusplus
}
#endif

#endif /* EXPRESSION_H */


