// test_subdir.c
#include "fs.h"     // for pdos_mkdir()
#include "disk.h"
#include "dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    // always start fresh for this test
    if (pdos_mkdisk(true) != 0) {
        fprintf(stderr, "pdos_mkdisk(true) failed\n");
        return 1;
    }
    if (pdos_mkfs("MYFS2025") != 0) {
        fprintf(stderr, "pdos_mkfs failed\n");
        return 1;
    }

    if (pdos_mkdir("subdir1") != 0) {
        fprintf(stderr, "pdos_mkdir(subdir1) failed\n");
        return 1;
    }

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
