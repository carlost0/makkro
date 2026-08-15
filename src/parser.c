#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sv.h"
#include "io.h"
#include "lexer.h"
#include "parser.h"

#define ERROR_EXIT(s, e) do {                                                         \
            fprintf(stderr, (s));                                                            \
            exit(e);                                                               \
} while (0)

static int arg_push(arg_da_t *arr, arg_t arg) {
    if (arr->size >= arr->cap) {
        if (arr->cap == 0) arr->cap = 10;
        arr->cap *= 2u;
        arr->data = (arg_t*)realloc(arr->data, arr->cap * sizeof(arg_t));

        if (!arr->data) {
            fprintf(stderr, "unable to allocate memory :(\n");
            return -1;
        }
    }
    memcpy(&arr->data[arr->size++], &arg, sizeof(arg_t));
    return 0;
}

static int call_push(callexpr_da_t *arr, callexpr_t call) {
    if (arr->size >= arr->cap) {
        if (arr->cap == 0) arr->cap = 10;

        arr->cap *= 2u;
        arr->data = (callexpr_t*)realloc(arr->data, arr->cap * sizeof(callexpr_t));

        if (!arr->data) {
            fprintf(stderr, "unable to allocate memory :(\n");
            return -1;
        }
    }
    memcpy(&arr->data[arr->size++], &call, sizeof(callexpr_t));
    return 0;
}

function_e iden_to_fun(token_t id) {
    printf("identifier: "SV_FMT"\n", SV_ARG(id.val));
    if (sv_cmp(sv("mov"), id.val))
        return FUN_MOV;
    if (sv_cmp(sv("key"), id.val))
        return FUN_KEY;
    if (sv_cmp(sv("hkey"), id.val))
        return FUN_HKEY;
    if (sv_cmp(sv("rkey"), id.val))
        return FUN_RKEY;
    if (sv_cmp(sv("str"), id.val))
        return FUN_STR;
    if (sv_cmp(sv("sleep"), id.val))
        return FUN_SLEEP;
    if (sv_cmp(sv("delay"), id.val))
        return FUN_DELAY;
    if (sv_cmp(sv("but"), id.val))
        return FUN_BUT;
    if (sv_cmp(sv("hbut"), id.val))
        return FUN_HBUT;
    if (sv_cmp(sv("rbut"), id.val))
        return FUN_RBUT;
    if (sv_cmp(sv("get"), id.val))
        return FUN_GET;
    if (sv_cmp(sv("#"), id.val))
        return COMMENT;
    return FUN_NULL;
}

callexpr_da_t parse(token_da_t toks) {
    token_da_t tokens = toks;

    callexpr_da_t calls = (callexpr_da_t){0};

    size_t i = 0;

    while (i < tokens.size) {
        token_t t = tokens.data[i];

        if (t.type != TK_IDENTIFIER) {
            i++;
            continue;
        }

        callexpr_t call = (callexpr_t){0};
        call.type = iden_to_fun(t);

        i++;
        if (i >= tokens.size) break;

        t = tokens.data[i];
        if (t.type != TK_ARROW) {
            continue;
        }

        i++;
        while (i < tokens.size) {
            t = tokens.data[i];

            if (i > 0) {
                if (t.type == TK_IDENTIFIER && tokens.data[i + 1].type != TK_STR_END && tokens.data[i - 1].type != TK_STR) {
                   break;
                }
            }

                    
            switch (t.type) {
                case TK_IDENTIFIER:
                    if (i > 0 && i < tokens.size) {
                        t.type = TK_STR;
                        if (call.type > FUN_NULL && call.type < FUN_MOV) {
                            arg_t arg = {0};
                            arg.type = ARG_STR;
                            arg.as.str = t.val;
                            arg_push(&call.args, arg);
                        }
                    }
                    break;

                case TK_NUM:
                    if (call.type > FUN_STR) {
                        arg_t arg;
                        arg.type = ARG_NUM;
                        arg.as.num = sv_to_i(t.val);
                        arg_push(&call.args, arg);
                    }
                    break;
                case TK_STR:
                    if (call.type > FUN_NULL && call.type < FUN_MOV) {
                        arg_t arg = {0};
                        arg.type = ARG_STR;
                        arg.as.str = t.val;
                        arg_push(&call.args, arg);
                    }
                    break;
                case TK_STR_END:
                    if (call.type > FUN_NULL && call.type < FUN_MOV) {
                        arg_t arg = {0};
                        arg.type = ARG_STR;
                        arg.as.str = t.val;
                        arg_push(&call.args, arg);
                    }
                    break;
                default:
                    goto outside;
            }

            i++;
        }

        call_push(&calls, call);

        continue;

    outside:
        call.type = FUN_NULL;
        call_push(&calls, call);
    }

    return calls;
}


int eval(callexpr_t call) {
    switch (call.type) {
        case FUN_KEY: {
            str_view_t tmp_sv = call.args.data[0].as.str;
            sv_trim(&tmp_sv);
            sv_chop_left(&tmp_sv, 1);
            sv_chop_right(&tmp_sv, 1);

            char *buf = (char *)malloc(tmp_sv.size * sizeof(char) + 1);

            if (!buf) {
                fprintf(stderr, "error: unable to allocate memory :(\n");
                return -1;
            }

            buf[tmp_sv.size] = '\0';
            snprintf(buf, tmp_sv.size + 1, SV_FMT, SV_ARG(tmp_sv));


            if (send_key(buf) < 0) {
                free(buf);
                return -1;
            }
            
            free(buf);
            break;
        } case FUN_HKEY: {
            str_view_t tmp_sv = call.args.data[0].as.str;
            sv_trim(&tmp_sv);
            sv_chop_left(&tmp_sv, 1);
            sv_chop_right(&tmp_sv, 1);

            char *buf = (char *)malloc(tmp_sv.size * sizeof(char) + 1);

            if (!buf) {
                fprintf(stderr, "error: unable to allocate memory :(\n");
                return -1;
            }
            buf[tmp_sv.size] = '\0';
            snprintf(buf, tmp_sv.size + 1, SV_FMT, SV_ARG(tmp_sv));


            int res = 0;
            res = toggle_key(buf, 1);

            free(buf);
            
            if (res < 0) {
                return -1;
            }

            break;
        } case FUN_RKEY: {
            str_view_t tmp_sv = call.args.data[0].as.str;
            sv_trim(&tmp_sv);
            sv_chop_left(&tmp_sv, 1);
            sv_chop_right(&tmp_sv, 1);

            char *buf = (char *)malloc(tmp_sv.size * sizeof(char) + 1);

            if (!buf) {
                fprintf(stderr, "error: unable to allocate memory :(\n");
                return -1;
            }

            buf[tmp_sv.size] = '\0';
            snprintf(buf, tmp_sv.size + 1, SV_FMT, SV_ARG(tmp_sv));


            int res = 0;
            res = toggle_key(buf, 0);

            free(buf);
            
            if (res < 0) {
                return -1;
            }
            break;
        } case FUN_STR: {
            size_t i;
            for (i = 0; i < call.args.size; ++i) {
                str_view_t tmp_sv = call.args.data[i].as.str;
                char *tmp_buf = (char *)malloc(tmp_sv.size * sizeof(char) + 1);

                if (!tmp_buf) {
                    fprintf(stderr, "error: unable to allocate memory :(\n");
                    return -1;
                }
                snprintf(tmp_buf, tmp_sv.size * sizeof(char) + 1, SV_FMT, SV_ARG(tmp_sv));

                int res = 0;
                res |= send_str(tmp_buf);
                res |= send_key("space");
                free(tmp_buf);
                if (res < 0) return -1;
            }
            break;
        } case FUN_MOV: {
            int x = call.args.data[0].as.num;
            int y = call.args.data[1].as.num;
            int res = 0;
            res = move_cursor(x, y);
            if (res < 0) return -1;
            break;
        } case FUN_SLEEP: {
            int n = call.args.data[0].as.num;
            delay(n * 1000);
            break;
        } case FUN_DELAY: {
            int n = call.args.data[0].as.num;
            delay(n);
            break;
        } case FUN_BUT: {
            int b = call.args.data[0].as.num;
            int res = 0;
            res = send_mb(b);

            if (res < 0) return -1;
            break;
        } case FUN_HBUT: {
            int b = call.args.data[0].as.num;
            int res = 0;
            res = toggle_mb(b, 1);

            if (res < 0) return -1;
            break;
        } case FUN_RBUT: {
            int b = call.args.data[0].as.num;
            int res = 0;
            res = toggle_mb(b, 0);

            if (res < 0) return -1;
            break;
        } case FUN_GET: {
            vec2_res pos = get_pointer_pos();
            if (pos.err >= 0) {
                printf("\nx => %d\t y => %d\n", pos.vec.x, pos.vec.y);
                fflush(stdout);
            }
            break;
        } default: {
            return -1;
        }
    }
    return 0;
}
