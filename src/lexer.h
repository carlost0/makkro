#ifndef LEXER_H_
#define LEXER_H_

#include "sv.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TK_NULL,
    TK_ARROW,
    TK_STR,
    TK_STR_END,
    TK_NUM,
    TK_IDENTIFIER,
} token_e;

typedef struct {
    str_view_t val;
    token_e    type;
} token_t;

typedef struct {
    size_t cap;
    size_t size;
    token_t *data;
} token_da_t;



token_da_t tokenize(const str_view_t source_code);


#endif /* LEXER_H_ */
