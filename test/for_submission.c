// test/for_submission.c
// What we are supposed to submit
#include "fs.h"
#include <stdio.h>
#include <stdbool.h>

int main() {
    // Part A: Create disk
    printf("\nShort Program A\n");
    if (pdos_mkdisk(true) == 0) {
        printf("Disk created successfully.\n");
    } else {
        printf("Disk creation failed.\n");
        return 1;
    }

    // Part B: Format file system
    printf("\nShort Program B\n");
    if (pdos_mkfs("MYFS2025") == 0) {
        printf("Filesystem formatted successfully.\n");
    } else {
        printf("Filesystem format failed.\n");
        return 1;
    }

    // Part C: Write 1023 chars to file1.txt
    printf("\nShort Program C\n");
    PDOS_FILE *pf1 = pdos_open("file1.txt", "w");
    for (int i = 0; i < 1023; i++) {
        int c = 'A' + (i % 26);
        pdos_fputc(c, pf1);
    }
    pdos_fclose(pf1);

    // Part D: Read file1.txt and verify
    printf("\nShort Program D\n");
    PDOS_FILE *pf2 = pdos_open("file1.txt", "r");
    for (int i = 0; i < 1023; i++) {
        int expected = 'A' + (i % 26);
        int c = pdos_fgetc(pf2);
        if (c != expected) {
            printf("Mismatch at position %d\n", i + 1);
        }
    }
    int eof_check = pdos_fgetc(pf2);
    if (eof_check != EOF) {
        printf("Expected EOF at position 1024\n");
    }
    pdos_fclose(pf2);

    // Part E: Repeat with file2.txt
    printf("\nShort Program E\n");
    PDOS_FILE *pf3 = pdos_open("file2.txt", "w");
    for (int i = 0; i < 1023; i++) {
        int c = 'A' + (i % 26);
        pdos_fputc(c, pf3);
    }
    pdos_fclose(pf3);

    PDOS_FILE *pf4 = pdos_open("file2.txt", "r");
    for (int i = 0; i < 1023; i++) {
        int expected = 'A' + (i % 26);
        int c = pdos_fgetc(pf4);
        if (c != expected) {
            printf("Mismatch at file2.txt pos %d\n", i + 1);
        }
    }
    eof_check = pdos_fgetc(pf4);
    if (eof_check != EOF) {
        printf("Expected EOF at file2.txt pos 1024\n");
    }
    pdos_fclose(pf4);

    // Part F: Extend file1.txt with 1024 more chars
    printf("\nShort Program F\n");
    PDOS_FILE *pf5 = pdos_open("file1.txt", "rw");
    for (int i = 0; i < 1023; i++) pdos_fgetc(pf5); // Skip existing
    for (int i = 0; i < 1024; i++) {
        int c = 'A' + (i % 26);
        pdos_fputc(c, pf5);
    }
    pdos_fclose(pf5);

    // Verify extended file1.txt
    printf("\nVerify Extended file1.txt\n");
    PDOS_FILE *pf6 = pdos_open("file1.txt", "r");
    for (int i = 0; i < 2047; i++) {
        int expected = 'A' + (i % 26);
        int c = pdos_fgetc(pf6);
        if (c != expected) {
            printf("Mismatch at file1.txt pos %d\n", i + 1);
        }
    }
    eof_check = pdos_fgetc(pf6);
    if (eof_check != EOF) {
        printf("Expected EOF at file1.txt pos 2048\n");
    }
    pdos_fclose(pf6);

    // Part G: Extend file2.txt with 1024 more chars
    printf("\nShort Program G\n");
    PDOS_FILE *pf7 = pdos_open("file2.txt", "rw");
    for (int i = 0; i < 1023; i++) pdos_fgetc(pf7);
    for (int i = 0; i < 1024; i++) {
        int c = 'A' + (i % 26);
        pdos_fputc(c, pf7);
    }
    pdos_fclose(pf7);

    // Verify extended file2.txt
    printf("\nVerify Extended file2.txt\n");
    PDOS_FILE *pf8 = pdos_open("file2.txt", "r");
    for (int i = 0; i < 2047; i++) {
        int expected = 'A' + (i % 26);
        int c = pdos_fgetc(pf8);
        if (c != expected) {
            printf("Mismatch at file2.txt pos %d\n", i + 1);
        }
    }
    eof_check = pdos_fgetc(pf8);
    if (eof_check != EOF) {
        printf("Expected EOF at file2.txt pos 2048\n");
    }
    pdos_fclose(pf8);

    // Part H: List contents of root directory
    printf("\nShort Program H\n");
    char **files = pdos_dir();
    if (files) {
        for (int i = 0; files[i]; i++) {
            printf("File: %s\n", files[i]);
        }
    }

    return 0;
}
