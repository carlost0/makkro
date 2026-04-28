#ifndef LEXER_H_
#define LEXER_H_

#include <stdbool.h>
#include "util.h"

typedef enum {
    string,
    number,
    identifier,
    arrow,
} token_e;

typedef struct {
    char    *val;
    token_e  type;
} token_t;

struct token_node {
    token_t            token;
    struct token_node *next;
};

void               free_tokens(struct token_node *head);
struct token_node* tokenize(char *source_code);

#endif /* LEXER_H_ */
