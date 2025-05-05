// test/test_print_files.c
#include "disk.h"
#include "dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

static bool file_exists(const char *path) {
    struct stat buf;
    return stat(path, &buf) == 0;
}

int main(void) {
    bool fresh = !file_exists("/dev/shm/pdosfs");

    if (pdos_mkdisk(fresh) != 0) {
        fprintf(stderr, "Failed to map or create disk.\n");
        return 1;
    }

    if (fresh) {
        if (pdos_mkfs("MYFS2025") != 0) {
            fprintf(stderr, "Failed to format fresh disk.\n");
            return 1;
        }
        printf("Fresh disk formatted.\n");
    } else {
        printf("Reusing existing disk.\n");
    }

    char **files = pdos_dir();
    if (!files) {
        fprintf(stderr, "pdos_dir() failed.\n");
        return 1;
    }

    printf("Files in root directory:\n");
    for (int i = 0; files[i] != NULL; ++i) {
        printf(" - %s\n", files[i]);
        free(files[i]);
    }
    free(files);
    return 0;
}
