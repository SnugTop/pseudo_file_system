#include "disk.h"
#include <stdio.h>
#include <stdbool.h>

int main(void) {
    if (pdos_mkdisk(true) == 0) {
        puts("Disk created successfully.");
        return 0;
    } else {
        fputs("Disk creation failed.\n", stderr);
        return 1;
    }
}
