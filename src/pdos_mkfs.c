// src/pdos_mkfs.c
#include "disk.h"
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ID-string>\n", argv[0]);
        return 1;
    }

    // Map the disk (but don’t unlink/truncate; assume it already exists)
    if (pdos_mkdisk(false) != 0) {
        fputs("ERROR: failed to map existing disk. Run pdos_mkdisk first?\n", stderr);
        return 1;
    }

    // Now format it
    if (pdos_mkfs(argv[1]) == 0) {
        puts("Filesystem formatted successfully.");
        return 0;
    } else {
        fputs("Filesystem format failed.\n", stderr);
        return 1;
    }
}
