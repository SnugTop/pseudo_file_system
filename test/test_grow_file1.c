// test_grow_file1.c
#include "fs.h"
#include "disk.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    // remap but don’t reformat
    if (pdos_mkdisk(false) != 0) {
        fprintf(stderr, "Failed to remap disk\n");
        return 1;
    }

    // open file1.txt in read+write so we can append without truncating
    PDOS_FILE *pf = pdos_open("file1.txt", "rw");
    if (!pf) {
        fprintf(stderr, "Failed to open file1.txt for grow test\n");
        return 1;
    }

    // prepare 1024 bytes of ‘X’
    size_t grow = 1024;
    char *buf = malloc(grow);
    memset(buf, 'X', grow);

    int wrote = pdos_write(pf, buf, grow);
    printf("Appended %d bytes to file1.txt\n", wrote);
    pdos_fclose(pf);
    free(buf);

    // now reopen for reading and confirm total size
    PDOS_FILE *pr = pdos_open("file1.txt", "r");
    if (!pr) {
        fprintf(stderr, "Failed to reopen file1.txt for verify\n");
        return 1;
    }
    // read into a big buffer
    size_t max = 3000;
    char *rbuf = malloc(max);
    int got = pdos_read(pr, rbuf, max);
    printf("Read back %d bytes total from file1.txt\n", got);
    pdos_fclose(pr);
    free(rbuf);

    return 0;
}
