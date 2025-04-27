#ifndef INODE_H
#define INODE_H

#include <time.h> // for time_t

#define MAX_DATA_BLOCKS 15
#define INODE_ENTRIES_PER_INODE_BLOCK 16

typedef struct INODE_ENTRY {
    unsigned int size;               // total size of file
    time_t file_mod_time;             // file modification time
    unsigned short data_blocks_used;  // number of data blocks
    unsigned short data_blocks[MAX_DATA_BLOCKS];
    unsigned char fill[16];           // padding to align to 1024
} INODE_ENTRY;

typedef struct INODE_BLOCK {
    INODE_ENTRY inodes[INODE_ENTRIES_PER_INODE_BLOCK];
} INODE_BLOCK;

#endif
