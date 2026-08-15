#ifndef PARSER_H_
#define PARSER_H_

#include "sv_da.h"
#include "sv.h"
#include "lexer.h"

#include <stdlib.h>

typedef enum {
    FUN_NULL = 0,
    FUN_KEY,
    FUN_HKEY,
    FUN_RKEY,
    FUN_STR,
    FUN_MOV,
    FUN_DELAY,
    FUN_SLEEP,
    FUN_BUT,
    FUN_RBUT,
    FUN_HBUT,
    FUN_GET,
} function_e;

typedef union {
    str_view_t str;
    int        num;
} arg_u;

typedef struct {
    enum {
        ARG_STR,
        ARG_NUM
    } type;
    union {
        str_view_t str;
        int        num;
    } as;
} arg_t;

typedef struct {
    arg_t *data;
    size_t size;
    size_t cap;
} arg_da_t;

typedef struct {
    function_e type;
    arg_da_t   args;
} callexpr_t;

typedef struct {
    callexpr_t *data;
    size_t size;
    size_t cap;
} callexpr_da_t;

callexpr_da_t parse(token_da_t t);
int eval(callexpr_t call);

#endif /* PARSER_H_ */
