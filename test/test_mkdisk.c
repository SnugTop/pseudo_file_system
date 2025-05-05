// test/test_mkdisk.c
//Testing
#include "disk.h"
#include <stdio.h>
#include <stdbool.h>

int main() {
    // create and format a fresh disk
    if (pdos_mkdisk(true) == 0) {
        printf("Disk created successfully.\n");
    } else {
        printf("Disk creation failed.\n");
        return 1;
    }
    if (pdos_mkfs("MYFS2025") == 0) {
        printf("Filesystem formatted successfully.\n");
    } else {
        printf("Filesystem format failed.\n");
        return 1;
    }
    return 0;
}
