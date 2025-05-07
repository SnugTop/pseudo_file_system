//dir.h
#ifndef DIR_H
#define DIR_H

#include "defs.h"
#include <stddef.h>

#define MAX_NAME_LENGTH 12

typedef struct {
    char d_name[MAX_NAME_LENGTH];
    unsigned short d_ino;
    short d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char padding[13];
} DIR_ENTRY; //size 51 bytes

enum { 
    MAX_DIR_ENTRIES = (int)(BLOCK_SIZE / sizeof(DIR_ENTRY))
};

typedef struct {
    int num_dir_entries; //size 4 bytes
    DIR_ENTRY dir_entries[MAX_DIR_ENTRIES]; //size 51*20 = 1020
} DIR_BLOCK;

typedef struct {
    char data[BLOCK_SIZE];
} DATA_BLOCK;

int dir_lookup(const char *filename);
int dir_add(const char *filename, int inode_number, int type);
char **pdos_dir(void);

#endif
