#ifndef _SV_H
#define _SV_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *data;
    size_t size;
    size_t cap;
} str_view_t;


str_view_t sv(char *str);
int sv_chop_right(str_view_t *sv, size_t n);
int sv_chop_left(str_view_t *sv, size_t n);
int sv_trim_left(str_view_t *sv);
int sv_trim_right(str_view_t *sv);
int sv_trim(str_view_t *sv);
int sv_append(str_view_t *sv, char c);
int sv_cmp(str_view_t a, str_view_t b);
int sv_isnum(str_view_t sv);
int sv_to_i(str_view_t sv);
str_view_t sv_tok(str_view_t *sv, const char *delims);
str_view_t sv_cpy(str_view_t *to, str_view_t from);


#ifdef SV_FMT
# undef SV_FMT
#endif /* SV_FMT */

#ifdef SV_ARG
# undef SV_ARG
#endif /* SV_ARG */

#ifdef SV_ARGP
# undef SV_ARGP
#endif /* SV_ARGP */

#define SV_FMT "%.*s"
#define SV_ARG(s) (int)(s).size, (s).data
#define SV_ARGP(p) (int)(p)->size, (p)->data


#ifdef SV_IMPLEMENTATION

str_view_t sv_cpy(str_view_t *to, str_view_t from) {
    to->size = from.size;
    to->cap  = from.cap;

    size_t i;

    for (i = 0; i < from.size; ++i) {
        to->data[i] = from.data[i];
    }


    return *to;
}

int sv_isnum(str_view_t sv) {
    size_t i;
    for (i = 0; i < sv.size; ++i) {
        if (!isdigit(sv.data[i])) return 0;
    }
    return 1;
}

int sv_to_i(str_view_t sv) {
    
    if (!sv_isnum(sv)) {
        fprintf(stderr, "error: unable to convert sv to int, sv data is not an int");
        return -1;
    }

    int sum = 0;

    size_t i;
    for (i = 0; i < sv.size; ++i) {
        sum = sum * 10 + (int) sv.data[i] - '0';
    }

    return sum;
}

int sv_cmp(str_view_t a, str_view_t b) {
    size_t i;

    if (a.size != b.size) return 0;
    for (i = 0; i < a.size; ++i) {
        if (a.data[i] != b.data[i]) return 0;
    }
    return 1;
}


str_view_t sv(char *str) {
    str_view_t res = {
        .size = strlen(str),
        .cap  = strlen(str),
        .data = strdup(str),
    };

    return res;
}

int sv_chop_right(str_view_t *sv, size_t n) {
    if (sv->size < n) sv->size = n;
    sv->size -= n;
    return 0;
}

int sv_chop_left(str_view_t *sv, size_t n) {
    if (sv->size < n) sv->size = n;
    sv->data += n;
    sv->size -= n;
    return 0;
}

int sv_trim_left(str_view_t *sv) {
    while (sv->size > 0 && isspace(sv->data[0])) {
        sv_chop_left(sv, 1);
    }
    return 0;
}

int sv_trim_right(str_view_t *sv) {
    while (sv->size > 0 && isspace(sv->data[sv->size - 1])) {
        sv_chop_right(sv, 1);
    }

    return 0;
}

int sv_trim(str_view_t *sv) {
    sv_trim_left(sv);
    sv_trim_right(sv);
    return 0;
}

int sv_append(str_view_t *sv , char c) {
    if (sv->size >= sv->cap) {
        if (sv->cap == 0) sv->cap = sv->size;

        sv->cap *= 2u;
        sv->data = (char*)realloc(sv->data, sv->cap * sizeof(char));

        if (!sv->data) {
            fprintf(stderr, "unable to allocate memory :(\n");
            return -1;
        }
    }

    sv->data[++sv->size] = c;
    return 0;
}


/* chops a sv by delimiters (like strtok), returns the chopped of sv and modifies the input sv */
str_view_t sv_tok(str_view_t *sv, const char *delims) {
    size_t i, j;
    size_t dlen;
    dlen = strlen(delims);

    for (i = 0; i < sv->size; i += 1) {
        for (j = 0; j < dlen; j++) {
            if (sv->data[i] == delims[j]) goto outside;
        }
    }
outside:
    if (i < sv->size) {
        str_view_t res = {
            .data = sv->data,
            .size = i,
        };

        sv_chop_left(sv, i + 1);
        return res;
    }
    str_view_t res = *sv;
    sv_chop_left(sv, sv->size);
    return res;
}

#endif /* SV_IMPLEMENTATION */
#endif /* _SV_H */
