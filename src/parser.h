#ifndef PARSER_H_
#define PARSER_H_

typedef enum {
    mov,
    key,
    str,
    sleep,
    put,
    but,
} function_e;

typedef union {
    char *str;
    int   num;
} arg_t;

typedef struct {
    function_e function;
    arg_t *args;
    int argc;
} callexpr_t;

struct call_node {
    callexpr_t callexpr;
    struct call_node *next;
};

struct call_node *eval(struct token_node *node);
struct call_node *tokens_to_snode(struct token_node *node);

#endif /* PARSER_H_ */
