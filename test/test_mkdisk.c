//test_mkdisk.c
#include "disk.h"
#include <stdio.h>
#include <stdbool.h>  // required for 'true'

int main() {
    if (pdos_mkdisk(true) == 0) {  // Pass 'true' to create a fresh disk
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
