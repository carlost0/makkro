#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "util.h"
#include "sv.h"

unsigned int str_to_uint(const char *str) {
    if (!is_num(str)) {
        fprintf(stderr, "error: unable to convert string to int :(\n");
        return -1;
    }

    unsigned int sum;
    size_t i;
    sum = 0;

    for (i = 0; i < strlen(str); ++i) {
        sum += (str[i] - '0') * pow(10, strlen(str) - i - 1);
    }

    return sum;
}

int is_num(const char *str) {
    size_t i;
    bool ret;

    for (i = 0; i < strlen(str); ++i) {
        if (str[i] >= '0' && str[i] <= '9')
            ret = true;
        else
            ret = false;
    }

    return ret;
}

str_view_t file_to_sv(const char *filename) {
    FILE *file;
    size_t file_size;
    char *buf;

    file = fopen(filename, "rb");

    if (!file) {
        fprintf(stderr, "error: unable to open file :(\n");
        return (str_view_t) {
            .size = 0,
        };
    }

    fseek(file, 0, SEEK_END);

    file_size = ftell(file);

    fseek(file, 0, SEEK_SET);

    buf = (char*)malloc(file_size * sizeof(char) + 2);

    if (!buf) {
        fprintf(stderr, "error: unable to allocate memory for file buffer :(\n");
        fclose(file);
        return (str_view_t) {
            .size = 0,
        };
    }

    fread(buf, 1, file_size, file);
    buf[file_size] = '\0';

    fclose(file);

    return sv(buf);
}
