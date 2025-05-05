// test/test_dir_ops.c
#include "disk.h"
#include "inode.h"
#include "dir.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    // brand‐new disk + root directory
    pdos_mkdisk(true);
    pdos_mkfs("MYFS2025");

    int ino = inode_allocate();
    if (dir_add("file1.txt", ino, FILE_TYPE) == 0) {
        printf("file1.txt added successfully!\n");
    }

    int found = dir_lookup("file1.txt");
    if (found >= 0) {
        printf("Found file1.txt at inode %d\n", found);
    } else {
        printf("file1.txt not found\n");
    }

    char **files = pdos_dir();
    for (int i = 0; files[i] != NULL; ++i) {
        printf("File: %s\n", files[i]);
        free(files[i]);
    }
    free(files);
    return 0;
}
