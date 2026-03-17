#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

/* push node to the end of the linked list */
int push(struct token_node *head, token_t token) {
    struct token_node *current;
    current = head;

    while (current->next != NULL)
        current = current->next;

    current->next = (struct token_node *) malloc(sizeof(struct token_node));

    if (current->next == NULL) {
        perror("unable to allocate memroy :(\nError");
        return -1;
    }

    current->next->token = token;

    current->next->next  = NULL;
    return 0;
}

void free_tokens(struct token_node *head) {
    struct token_node *tmp;

    while (head->next != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

token_e val_to_token(char *val) {
    if (val[0] == '"' && val[strlen(val) - 1] == '"')
        return string;
    else if (is_num(val))
        return number;
    else if (strcmp("->", val) == 0)
        return arrow;
    else if ((val[0] >= 'A' && val[0] <= 'Z')
          || (val[0] >= 'a' && val[0] <= 'z'))
        return identifier;
    else
        return -1;
}

struct token_node* tokenize(char *source_code) {
    struct token_node *head;
    token_t token;
    char *token_val;

    head = (struct token_node *) malloc(sizeof(struct token_node));

    if (head == NULL) {
        perror("unable to allocate memory :(\nError");
        exit(1);
    }

    head->next = NULL;

    token_val = trim_str(strtok(source_code, " ;"));

    while (token_val != NULL) {
        token.val = token_val;
        token.type = val_to_token(token.val);

        if (push(head, token) != 0)
            return NULL;

        token_val = trim_str(strtok(NULL, " ;"));
    }

    return head->next != NULL ? head->next : head;
}

