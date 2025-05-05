// test/test_read_file1.c
#include "fs.h"
#include "disk.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main() {
    // remap existing disk
    if (pdos_mkdisk(false) != 0) {
        fprintf(stderr, "Failed to map disk\n");
        return 1;
    }
    if (!pdos_exists("file1.txt")) {
        printf("file1.txt does not exist.\n");
        return 1;
    }
    PDOS_FILE *pf = pdos_open("file1.txt", "r");
    if (!pf) {
        printf("Failed to open file1.txt for reading\n");
        return 1;
    }
    char buf[1024];
    int n = pdos_read(pf, buf, sizeof(buf)-1);
    buf[n] = '\0';
    printf("Read %d bytes from file1.txt:\n%s", n, buf);
    pdos_fclose(pf);
    return 0;
}
