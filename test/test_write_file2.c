// test/test_write_file2.c
#include "fs.h"
#include "disk.h"
#include <stdio.h>
#include <string.h>

int main() {
    // remap existing disk
    if (pdos_mkdisk(false) != 0) {
        fprintf(stderr, "Failed to map disk\n");
        return 1;
    }
    PDOS_FILE *pf = pdos_open("file2.txt", "w");
    if (!pf) {
        printf("Failed to open file2.txt for writing\n");
        return 1;
    }
    const char *msg = "Second test file content.\n";
    int n = pdos_write(pf, msg, strlen(msg));
    printf("Wrote %d bytes to file2.txt\n", n);
    pdos_fclose(pf);
    return 0;
}
