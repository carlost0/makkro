#include <stdio.h>
#include <stdlib.h>

#include "../lib/io.h"

char *file_to_str(char *filename) {
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

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("usage: makkro [file]\n");
        return 0;
    }

    char *filebuf;
    filebuf = file_to_str(argv[1]);

    printf("%s", filebuf);

    free(filebuf);

    return 0;
}

