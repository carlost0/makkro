#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "lexer.h"
#include "parser.h"
#include "io.h"

#define PERROR_EXIT(s) do {                                                         \
            perror((s));                                                            \
            exit(-1);                                                               \
} while (0);

int push();

/* push node to the end of the linked list */
int push(struct call_node *head, callexpr_t callexpr) {
    struct call_node *current;
    current = head;

    while (current->next != NULL)
        current = current->next;

    current->next = (struct call_node *) malloc(sizeof(struct call_node));

    if (current->next == NULL) {
        perror("unable to allocate memroy :(\nerror");
        return -1;
    }

    current->next->callexpr = callexpr;

    current->next->next  = NULL;
    return 0;
}

struct call_node *tokens_to_snode(struct token_node *node) {
    struct call_node *cnode;
    cnode = (struct call_node *) malloc(sizeof(struct call_node));

    for (;node; node = node->next) {
        /* check if node is a valid call */
        if (node->token.type != identifier) {
            PERROR_EXIT("error in script: line doesn't start with a function call")
        }

        if (node->next->token.type != arrow) {
            PERROR_EXIT("error in script: function call requires a '->' to pass parameters")
        }

        if (node->token.val == "mov")
            if (push(cnode, (callexpr_t) {
                }) != 0)
    }
}
