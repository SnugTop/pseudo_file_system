#include "disk.h"
#include <stdio.h>

int main() {
    if (pdos_mkdisk() == 0) {
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
