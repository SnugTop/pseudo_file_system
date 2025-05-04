#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "disk.h"
#include "dir.h"

int file_exists(const char *path) {
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

int main() {
    if (!file_exists("/dev/shm/pdosfs")) {
        pdos_mkdisk();
        pdos_mkfs("MYFS2025");
        printf("Fresh disk created and formatted.\n");
    } else {
        pdos_mkdisk();  // just remap
        printf("Reusing existing disk.\n");
    }

    // TODO: add logic to list files or open one
    char **files = pdos_dir();
    for (int i = 0; files[i] != NULL; ++i) {
        printf("File: %s\n", files[i]);
        free(files[i]);
    }
    free(files);

    return 0;
}
