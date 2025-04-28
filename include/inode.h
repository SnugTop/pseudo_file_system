#ifndef INODE_H
#define INODE_H

#include <time.h>

#define MAX_DATA_BLOCKS 15
#define INODE_ENTRIES_PER_INODE_BLOCK 16

typedef struct {
    unsigned int size; //4 bytes
    time_t file_mod_time; //8 bytes
    unsigned short data_blocks_used; //2 bytes
    unsigned short data_blocks[MAX_DATA_BLOCKS]; //30 bytes
    unsigned char fill[20]; // padding //20 bytes
} INODE_ENTRY; //64 bytes

typedef struct {
    INODE_ENTRY inodes[INODE_ENTRIES_PER_INODE_BLOCK];
} INODE_BLOCK; //1024 bytes

// Function prototypes
int inode_allocate(void);
int inode_read(int inode_number, INODE_ENTRY *inode);
int inode_write(int inode_number, const INODE_ENTRY *inode);

#endif
