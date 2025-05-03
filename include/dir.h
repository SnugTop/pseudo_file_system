//dir.h
#ifndef DIR_H
#define DIR_H

#include "defs.h"  // Ensure BLOCK_SIZE is defined

#define MAX_NAME_LENGTH 12

// Directory entry: should be ~32 bytes total
typedef struct {
    char d_name[MAX_NAME_LENGTH];   // 12 bytes
    unsigned short d_ino;           // 2 bytes
    short d_off;                    // 2 bytes
    unsigned short d_reclen;        // 2 bytes
    unsigned char d_type;           // 1 byte
    char padding[13];               // Padding to reach 32 bytes
} DIR_ENTRY;

#define MAX_DIR_ENTRIES (BLOCK_SIZE / sizeof(DIR_ENTRY))

// Directory block: holds multiple directory entries
typedef struct {
    int num_dir_entries;                        // 4 bytes
    DIR_ENTRY dir_entries[MAX_DIR_ENTRIES];    // e.g. 32 entries
} DIR_BLOCK;

// Data block (for file content, usually declared in defs.h or a separate file)
typedef struct {
    char data[BLOCK_SIZE];                      // 1024 bytes
} DATA_BLOCK;

// Function prototypes
int dir_lookup(const char *filename);
int dir_add(const char *filename, int inode_number, int type);
char **pdos_dir(void);

#endif
