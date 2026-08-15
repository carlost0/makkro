#ifndef _DA_H
#define _DA_H

#include <stdlib.h>
#include "sv.h"

typedef struct {
    size_t cap;
    size_t size;
    str_view_t *data;
} sv_da_t;

static const size_t DA_START_CAP = 256;

int da_push(sv_da_t *arr, str_view_t str);

#ifdef DA_IMPLEMENTATION

int da_push(sv_da_t *arr, str_view_t str) {
    if (arr->size >= arr->cap) {
        if (arr->cap == 0) arr->cap = DA_START_CAP;

        arr->cap = (size_t) (arr->cap * 1.5);
        arr->data = (str_view_t*)realloc(arr->data, arr->cap * sizeof(str_view_t));

        if (!arr->data) {
            fprintf(stderr, "unable to allocate memory :(\n");
            return -1;
        }
    }
    arr->data[arr->size++] = str;
    return 0;
}

#define da_pop(arr, x) \
    do { \
        if (arr->size == 0) break; \
 \
        x = arr.data[--arr.size]; \
    } while (0)

#endif /* DA_IMPLEMENTATION */
#endif /* _DA_H */
