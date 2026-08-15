#include "sv.h"

#include "lexer.h"

static int token_push(token_da_t *arr, token_t tok) {
    if (arr->size >= arr->cap) {
        if (arr->cap == 0) arr->cap = 10u;

        arr->cap *= 2u;
        arr->data = (token_t*)realloc(arr->data, arr->cap * sizeof(token_t));

        if (!arr->data) {
            fprintf(stderr, "unable to allocate memory :(\n");
            return -1;
        }
    }
    memcpy(&arr->data[arr->size++], &tok, sizeof(token_t));
    return 0;
}

token_e val_to_token(const str_view_t val) {
    str_view_t tmp = val;
    if (sv_isnum(tmp)) {
        return TK_NUM;
    }
    if (tmp.data[tmp.size - 1] == '"') 
        return TK_STR_END;
    if (tmp.data[0] == '"')
        return TK_STR;
    if (sv_cmp(sv("->"), tmp))
        return TK_ARROW;
    if ((tmp.data[0] >= 'A' && tmp.data[0] <= 'Z')
          || (tmp.data[0] >= 'a' && tmp.data[0] <= 'z'))
        return TK_IDENTIFIER;
    else
        return TK_NULL;
}


token_da_t tokenize(const str_view_t source_code) {
    token_da_t tokens;
    token_t    current;
    str_view_t sc;
    tokens  = (token_da_t) {0};
    current = (token_t) {0};

    sc = source_code;
    
    while (sc.size > 0) {
        current = (token_t) {0};
        str_view_t token_str = {0};
        token_str = sv_tok(&sc, " ;");
        sv_trim(&token_str);

        current.type = val_to_token(token_str);
        current.val  = token_str;

        token_push(&tokens, current);
    }

    return tokens;
}
