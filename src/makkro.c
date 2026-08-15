/*
 * makkro: basic interpreted language for writing macros on the X window system
 *
 * usage: makkro file.makkro
 *
*/

/*
 * Copyright (C) 2026 Carlos G. S.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Contact: Instagram DMs @cgs.dev
*/

#include <stdio.h>
#include <stdbool.h>

#include "util.h"
#include "lexer.h"
#include "parser.h"

#include "sv.h"

#define MAKKRO_PRINT_DEBUG_INFO

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "usage: makkro [file]\n");
        return 0;
    }

    str_view_t    sc     = {0};
    token_da_t    tokens = {0};
    callexpr_da_t calls  = {0};

    sc = file_to_sv(argv[1]);
    printf(SV_FMT, SV_ARG(sc));

    if (sc.size <= 0) {
        return -1;
    }

    tokens = tokenize(sc);


    /* check for invalid tokens */
    
    for (size_t i = 0; i < tokens.size; ++i) {
        if (tokens.data[i].type == TK_NULL) {
            fprintf(stderr, "error: unable to recognize token [%lu]:"SV_FMT"\n", i, SV_ARG(tokens.data[i].val));
            //return -1;
        }
        printf("token [%lu] => "SV_FMT"\t", i, SV_ARG(tokens.data[i].val));
        printf("type => TK_");
        switch (tokens.data[i].type) {
            case TK_ARROW:
                printf("ARROW");
                break;
            case TK_STR:
                printf("STR");
                break;
            case TK_STR_END:
                printf("STR_END");
                break;
            case TK_IDENTIFIER:
                printf("IDENTIFIER");
                break;
            case TK_NUM:
                printf("ARROW");
                break;
            default: break;
        }
        printf("\n");
    }

    calls = parse(tokens);

    /* evaluate function calls */
    for (size_t i = 0; i < calls.size; ++i) {
        if (calls.data[i].type == FUN_NULL) {
            fprintf(stderr, "error: unable to recognize call [%lu]\n", i);
            //return -1;
        }

        printf("id => %d\t args: ", calls.data[i].type);
        for (size_t j = 0; j < calls.data[i].args.size; ++j) {
            if (calls.data[i].args.data[j].type == ARG_STR) {
                printf(SV_FMT" ", SV_ARG(calls.data[i].args.data[j].as.str));
            } else if (calls.data[i].args.data[j].type == ARG_NUM) {
                printf("%d ", calls.data[i].args.data[j].as.num);
            }
        }
        printf("\n");
        if (eval(calls.data[i]) < 0) {
            fprintf(stderr, "runtime error: unable to execute call [%lu]\n", i);
            return -1;
        }

    }

    free(sc.data);
    return 0;
}

