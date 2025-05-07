// src/pdos_mkfs.c
#include "disk.h"
#include <stdio.h>
#include <string.h>

#define DEFAULT_ID  "MYFS2025"

int main(int argc, char **argv) {
    const char *id;

    if (argc == 1) {
        // no argument  use the built-in default
        id = DEFAULT_ID;
    }
    else if (argc == 2) {
        // one argument use that instead
        id = argv[1];
    }
    else {
        fprintf(stderr, "Usage: %s [ID-string]\n", argv[0]);
        return 1;
    }

    // 1) make sure the disk is mapped
    if (pdos_mkdisk(false) != 0) {
        fputs("ERROR: failed to map existing disk; run pdos_mkdisk first\n",
              stderr);
        return 1;
    }

    // 2) format with the chosen ID
    if (pdos_mkfs((char*)id) == 0) {
        printf("Filesystem formatted (ID=\"%s\").\n", id);
        return 0;
    } else {
        fputs("Filesystem format failed.\n", stderr);
        return 1;
    }
}
