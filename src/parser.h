#ifndef PARSER_H_
#define PARSER_H_

typedef enum {
    mov,
    key,
    str,
    sleep,
    put,
    but,
} identifier_e;

typedef union {
    char *str;
    int   num;
} arg_t;

struct syntax_node {
    identifier_e identifier;   
    int argc;
    arg_t *args;
    struct syntax_node *next;
};

struct syntax_node *eval(struct token_node *node);

#endif /* PARSER_H_ */
