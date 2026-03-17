#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "util.h"

unsigned int str_to_uint(char *str) {
    if (!is_num(str)) {
        perror("unable to convert string to int :(\nError");
        return -1;
    }

    unsigned int sum;
    size_t i;
    sum = 0;

    for (i = 0; i < strlen(str); ++i) {
        sum += (str[i] - 48) * pow(10, strlen(str) - i - 1);
    }

    return sum;
}

bool is_num(char *str) {
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

char* trim_str(char *str) {
    if (str == NULL) return NULL;
 
    char *start;
    char* end;
    start = str;

    while (isspace((unsigned char)*start)) {
        start++;
    }
 
    if (*start == '\0') {
        *str = '\0'; 
        return str;
    }
 
    end = start;
    while (*end != '\0') {
        end++;
    }
    end--; 
 
    while (end >= start && isspace((unsigned char)*end)) {
        end--;
    }
    end++;  
    *end = '\0';  
 
    return start;
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

