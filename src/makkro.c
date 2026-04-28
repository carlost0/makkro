#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#include "io.h"
#include "util.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("usage: makkro [file]\n");
        return 0;
    }

    char *source_code;
    source_code = file_to_str(argv[1]);

    struct token_node *tokens = tokenize(source_code);

    free(source_code);
    free_tokens(tokens);

    return 0;
}

