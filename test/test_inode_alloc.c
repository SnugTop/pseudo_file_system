// test/test_inode_alloc.c
#include "disk.h"
#include "inode.h"
#include <stdio.h>
#include <stdbool.h>

int main() {
    // brand‐new disk
    pdos_mkdisk(true);
    pdos_mkfs("MYFS2025");

    int inode_num = inode_allocate();
    if (inode_num >= 0) {
        printf("Allocated inode number: %d\n", inode_num);
    } else {
        printf("Failed to allocate inode\n");
    }
    return 0;
}
