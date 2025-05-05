//test_read_file1.c

#include "fs.h"
#include "disk.h"
#include <stdio.h>
#include <string.h>

int main() {
    pdos_mkdisk(true);

    if (!pdos_exists("file1.txt")) {
        printf("file1.txt does not exist.\n");
        return 1;
    }

    PDOS_FILE *pf = pdos_open("file1.txt", "r");
    if (!pf) {
        printf("Failed to open file1.txt for reading\n");
        return 1;
    }

    char buffer[1024];
    int read_bytes = pdos_read(pf, buffer, sizeof(buffer) - 1);
    buffer[read_bytes] = '\0'; // null-terminate

    printf("Read %d bytes from file1.txt:\n%s", read_bytes, buffer);
    pdos_fclose(pf);
    return 0;
}
