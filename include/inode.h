#ifndef INODE_H
#define INODE_H

#include <time.h>

#define MAX_DATA_BLOCKS 15
#define INODE_ENTRIES_PER_INODE_BLOCK 16

typedef struct INODE_ENTRY {
    unsigned int size;
    time_t file_mod_time;
    unsigned short data_blocks_used;
    unsigned short data_blocks[MAX_DATA_BLOCKS];
    unsigned char fill[16]; // padding
} INODE_ENTRY;

typedef struct INODE_BLOCK {
    INODE_ENTRY inodes[INODE_ENTRIES_PER_INODE_BLOCK];
} INODE_BLOCK;

// Function prototypes
int inode_allocate(void);
int inode_read(int inode_number, INODE_ENTRY *inode);
int inode_write(int inode_number, const INODE_ENTRY *inode);

#endif
