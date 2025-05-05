//test_write_file2.c

#include "fs.h"
#include "disk.h"
#include <stdio.h>
#include <string.h>

int main() {
    pdos_mkdisk(true); // reuse existing disk
    PDOS_FILE *pf = pdos_open("file2.txt", "w");
    if (!pf) {
        printf("Failed to open file2.txt for writing\n");
        return 1;
    }

    const char *data = "Second test file content.\n";
    int written = pdos_write(pf, data, strlen(data));
    printf("Wrote %d bytes to file2.txt\n", written);

    pdos_fclose(pf);
    return 0;
}
