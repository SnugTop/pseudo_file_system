// src/pdos_dir.c
#include "disk.h"
#include "dir.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // 1) map the already‐created disk into memory
    if (pdos_mkdisk(false) != 0) {
        fputs("ERROR: failed to map existing disk. Did you run pdos_mkdisk?\n", stderr);
        return 1;
    }

    // 2) read the directory entries
    char **listing = pdos_dir();
    if (!listing) {
        fputs("Failed to read directory.\n", stderr);
        return 1;
    }

    // 3) print them
    for (int i = 0; listing[i]; ++i) {
        printf("%s\n", listing[i]);
        free(listing[i]);
    }
    free(listing);
    return 0;
}
