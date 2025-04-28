#include "inode.h"
#include "defs.h"
#include "disk.h"
#include <string.h>
#include <stdio.h>

// Inode Bitmap is stored in Block 1

int inode_allocate(void) {
    unsigned short bitmap[BLOCK_SIZE / 2];
    block_read(1, bitmap); // Read inode bitmap block

    for (int word = 0; word < (BLOCK_SIZE / 2); ++word) {
        if (bitmap[word] != 0xFFFF) { // If not all bits used
            for (int bit = 0; bit < 16; ++bit) {
                if ((bitmap[word] & (1 << bit)) == 0) {
                    bitmap[word] |= (1 << bit); // Mark inode as used
                    block_write(1, bitmap);
                    return (word * 16) + bit;   // Inode number
                }
            }
        }
    }

    return -1; // No free inodes
}

int inode_read(int inode_number, INODE_ENTRY *inode) {
    if (inode_number < 0 || inode_number >= 1024) {
        fprintf(stderr, "Invalid inode number: %d\n", inode_number);
        return -1;
    }

    int block_number = 3 + (inode_number / 16);
    INODE_BLOCK inode_block;
    block_read(block_number, &inode_block);

    *inode = inode_block.inodes[inode_number % 16];
    return 0;
}

int inode_write(int inode_number, const INODE_ENTRY *inode) {
    if (inode_number < 0 || inode_number >= 1024) {
        fprintf(stderr, "Invalid inode number: %d\n", inode_number);
        return -1;
    }

    int block_number = 3 + (inode_number / 16);
    INODE_BLOCK inode_block;
    block_read(block_number, &inode_block);

    inode_block.inodes[inode_number % 16] = *inode;
    block_write(block_number, &inode_block);
    return 0;
}
