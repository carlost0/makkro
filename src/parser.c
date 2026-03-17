#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "lexer.h"
#include "parser.h"
#include "io.h"

void eval(struct token_node *node) {
    if (node->token.type == identifier) {
        if (strcmp(node->token.val, "mov") == 0) {
            int x, y;
            x = str_to_uint(node->next->next->token.val);
            y = str_to_uint(node->next->next->next->token.val);
            move_cursor(x, y);

            node = node->next->next->next->next;
            return;
        }
    }
}
