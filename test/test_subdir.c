// test/test_subdir.c
#include "fs.h"     // for pdos_mkdir()
#include "disk.h"   // for pdos_mkdisk(), pdos_mkfs()
#include "dir.h"    // for pdos_dir()
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(void) {
    // 1) Create and format a fresh disk
    if (pdos_mkdisk(true) != 0) {
        fprintf(stderr, "pdos_mkdisk(true) failed\n");
        return 1;
    }
    if (pdos_mkfs("MYFS2025") != 0) {
        fprintf(stderr, "pdos_mkfs failed\n");
        return 1;
    }

    // 2) Make a new subdirectory named "subdir1"
    pdos_mkdir("subdir1");

    // 3) List root directory contents
    char **files = pdos_dir();
    if (!files) {
        fprintf(stderr, "pdos_dir() failed\n");
        return 1;
    }

    printf("After mkdir:\n");
    for (int i = 0; files[i] != NULL; ++i) {
        printf("  %s\n", files[i]);
        free(files[i]);
    }
    free(files);

    return 0;
}
