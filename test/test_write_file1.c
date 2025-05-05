//test_write_file1.c

#include "fs.h"
#include "disk.h"
#include <stdio.h>
#include <string.h>

int main() {
    pdos_mkdisk(); // maps existing disk
    PDOS_FILE *pf = pdos_open("file1.txt", "w");
    if (!pf) {
        printf("Failed to open file1.txt for writing\n");
        return 1;
    }

    const char *msg = "Hello from test_write_file1!\n";
    int written = pdos_write(pf, msg, strlen(msg));
    printf("Wrote %d bytes to file1.txt\n", written);

    pdos_fclose(pf);
    return 0;
}
