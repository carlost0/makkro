#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"

typedef enum {
    mov,
    str,
    key,
    but,
    arg,
    next,
} token_e;

typedef struct {
    char    *val;
    token_e  type;
} token_t;

struct token_node {
    token_t            token;
    struct token_node *next;
};

void push(struct token_node *head, token_t token) {
    struct token_node *current;
    current = head;

    while (current->next != NULL)
        current = current->next;

    current->next = (struct token_node *) malloc(sizeof(struct token_node));
    current->next->token = token;

    current->next->next  = NULL;
}

char* file_to_str(char *filename) {
    FILE *file;
    file = fopen(filename, "rb");

    if (!file) {
        perror("unable to open file :(\nError");
        return NULL;
    }

    fseek(file, 0, SEEK_END);

    size_t file_size;
    file_size = ftell(file);

    fseek(file, 0, SEEK_SET);

    char *buf;
    buf = (char*)malloc(file_size * sizeof(char) + 1);

    if (!buf) {
        perror("unable to allocate memory for file buffer :(\nError");
        fclose(file);
        return NULL;
    }

    fread(buf, 1, file_size, file);
    buf[file_size + 1] = '\0';

    fclose(file);

    return buf;
}

token_e val_to_token(char *val) {
    if (strcmp(val, "mov"))
        return mov;
    else if (strcmp(val, "str"))
        return str;
    else if (strcmp(val, "key"))
        return key;
    else if (strcmp(val, "but"))
        return but;
    else if (strcmp(val, ";"))
        return next;
    else
        return arg;
}

struct token_node* tokenize(char *source_code) {
    struct token_node *node;
    token_t token;
    node = (struct token_node *) malloc(sizeof(struct token_node));

    token.val  = strtok(source_code, " ");
    token.type = val_to_token(token.val);

    while (token.val != NULL) {
        push(node, token);

        token.val = strtok(NULL, " ");
        token.type = val_to_token(token.val);

        node = node->next;
    }

    return node;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("usage: makkro [file]\n");
        return 0;
    }

    char *source_code;
    source_code = file_to_str(argv[1]);

    struct token_node *tokens = tokenize(source_code);

    while (tokens != NULL) {
        printf("%s", tokens->token.val);
        tokens = tokens->next;
    }

    free(source_code);

    return 0;
}

