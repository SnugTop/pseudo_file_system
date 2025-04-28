#include "disk.h"
#include "inode.h"
#include "dir.h"    
#include <stdio.h>
#include <stdlib.h> 

int main() {
    pdos_mkdisk();
    pdos_mkfs("MYFS2025");

    int ino = inode_allocate();
    if (ino >= 0) {
        printf("Allocated inode number: %d\n", ino);

        if (dir_add("file1.txt", ino, 2) == 0) {
            printf("file1.txt added successfully!\n");
        }
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
