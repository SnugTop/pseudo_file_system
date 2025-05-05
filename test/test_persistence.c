// test/test_persistence.c
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "disk.h"
#include "dir.h"

static int file_exists(const char *path) {
    struct stat buf;
    return stat(path, &buf) == 0;
}

int main() {
    bool fresh = !file_exists("/dev/shm/pdosfs");

    if (pdos_mkdisk(fresh) != 0) {
        fprintf(stderr, "Failed to map/create disk.\n");
        return 1;
    }
    if (fresh) {
        pdos_mkfs("MYFS2025");
        printf("Fresh disk created and formatted.\n");
    } else {
        printf("Reusing existing disk.\n");
    }

    char **files = pdos_dir();
    if (!files) {
        fprintf(stderr, "Failed to read directory.\n");
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
